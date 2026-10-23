#ifdef CONFIG_KSU_SUSFS
#include <linux/susfs_def.h>

static inline bool is_zygote_isolated_service_uid(uid_t uid)
{
	uid %= 100000;
	return (uid >= 99000 && uid < 100000);
}

static inline bool is_zygote_normal_app_uid(uid_t uid)
{
	uid %= 100000;
	return (uid >= 10000 && uid < 19999);
}

extern u32 susfs_zygote_sid;
extern struct cred *ksu_cred;
extern struct work_struct susfs_extra_works;

struct susfs_handle_setuid_tw {
	struct callback_head cb;
};

static void susfs_handle_setuid_tw_func(struct callback_head *cb)
{
	struct susfs_handle_setuid_tw *tw = container_of(cb, struct susfs_handle_setuid_tw, cb);

	/* ksu_cred must be non-NULL here (only set after KSU init, long before
	 * zygote spawns), but override_creds(NULL) would poison current->cred,
	 * so guard it anyway.
	 */
	if (ksu_cred) {
		const struct cred *saved = override_creds(ksu_cred);

		revert_creds(saved);
	}
	kfree(tw);
}

static void ksu_handle_extra_susfs_work(void)
{
	struct susfs_handle_setuid_tw *tw = kzalloc(sizeof(*tw), GFP_ATOMIC);

	if (work_pending(&susfs_extra_works)) {
		kfree(tw);
		return;
	}

	schedule_work(&susfs_extra_works);

	if (!tw) {
		pr_err("susfs: No enough memory\n");
		return;
	}

	tw->cb.func = susfs_handle_setuid_tw_func;

	int err = task_work_add(current, &tw->cb, TWA_RESUME);
	if (err) {
		kfree(tw);
		pr_err("susfs: Failed adding task_work 'susfs_handle_setuid_tw', err: %d\n", err);
	}
}

#ifdef CONFIG_KSU_SUSFS_TRY_UMOUNT
extern void susfs_try_umount(uid_t uid);
#endif

/* LSM cred-hook counterpart of the upstream do_umount flow:
 * only for zygote spawned processes; run the susfs umount list and
 * mark the process TIF_PROC_UMOUNTED for isolated services and for
 * normal app uids whose profile says umount is needed. SUS_MOUNT
 * hiding then happens via spoofing instead of real umounts.
 */
static void susfs_handle_umount_cred(struct cred *new, const struct cred *old)
{
	uid_t new_uid = ksu_get_uid_t(new->uid);

	// We only interest in process spawned by zygote
	if (!susfs_is_sid_equal(old, susfs_zygote_sid))
		return;

	// isolated service: always handle; normal app: per-app umount profile
	if (!is_zygote_isolated_service_uid(new_uid) &&
	    !(is_zygote_normal_app_uid(new_uid) && ksu_uid_should_umount(new_uid)))
		return;

#ifdef CONFIG_KSU_SUSFS_TRY_UMOUNT
	susfs_try_umount(new_uid);
#else
	ksu_handle_umount(new, old);
#endif

	ksu_handle_extra_susfs_work();
	susfs_set_current_proc_umounted();
}
#endif // #ifdef CONFIG_KSU_SUSFS

static __always_inline void ksu_handle_setresuid_cred(struct cred *new, const struct cred *old)
{
	if (!new || !old)
		return;

	uid_t new_uid = ksu_get_uid_t(new->uid);
	uid_t old_uid = ksu_get_uid_t(old->uid);

	// old process is not root, ignore it.
	if (unlikely(!!old_uid))
		return;

	if (IS_ENABLED(CONFIG_KSU_DEBUG))
		pr_info("handle_setresuid from %d to %d\n", old_uid, new_uid);

	// we dont have those new fancy things upstream has
	// lets just do the original thing where we disable seccomp
	if (unlikely(is_uid_manager(new_uid)))
		goto install_ksu_fd;

	if (ksu_is_allow_uid_for_current(new_uid))
		goto kill_seccomp;

	// Handle kernel umount
#ifdef CONFIG_KSU_SUSFS
	susfs_handle_umount_cred(new, old);
#else
	ksu_handle_umount(new, old);
#endif
	return;

install_ksu_fd:
	pr_info("install fd for manager: %d\n", new_uid);
	ksu_install_fd();

kill_seccomp:
	disable_seccomp();
	set_thread_flag(TIF_KSU_MANAGED); // sucompat fast-path
	return;
}
