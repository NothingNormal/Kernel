#include <linux/compiler.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
#include <linux/sched/signal.h>
#endif
#include <linux/slab.h>
#include <linux/task_work.h>
#include <linux/thread_info.h>
#include <linux/seccomp.h>
#include <linux/printk.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/uidgid.h>
#include <linux/workqueue.h>

#include "policy/app_profile.h"
#include "policy/allowlist.h"
#include "policy/app_profile.h"
#include "hook/setuid_hook.h"
#include "klog.h" // IWYU pragma: keep
#include "manager/manager_identity.h"
#include "infra/seccomp_cache.h"
#include "supercall/supercall.h"
#include "hook/hook_manager.h"
#include "feature/kernel_umount.h"
#include "selinux/selinux.h"
#include "compat/kernel_compat.h"

#ifdef CONFIG_KSU_SUSFS
/*
 * Needed for susfs_is_current_proc_umounted() (static inline, susfs_def.h)
 * regardless of whether TRY_UMOUNT is on - the sus_path_loop work is gated
 * on that flag.
 */
#include <linux/susfs.h>
#endif

int ksu_handle_setresuid(uid_t old_uid, uid_t new_uid)
{
    // we rely on the fact that zygote always call setresuid(3) with same uids

    pr_info("handle_setresuid from %d to %d\n", old_uid, new_uid);

    if (unlikely(is_uid_manager(new_uid))) {

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
        if (current->seccomp.mode == SECCOMP_MODE_FILTER && current->seccomp.filter) {
            ksu_seccomp_allow_cache(current->seccomp.filter, __NR_reboot);
        }
#else
		disable_seccomp();
#endif

#ifdef KSU_KPROBES_HOOK
        ksu_set_task_tracepoint_flag(current);
#endif

        pr_info("install fd for manager: %d\n", new_uid);
        ksu_install_fd();
        return 0;
    }

    if (ksu_is_allow_uid_for_current(new_uid)) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
        if (current->seccomp.mode == SECCOMP_MODE_FILTER && current->seccomp.filter) {
            ksu_seccomp_allow_cache(current->seccomp.filter, __NR_reboot);
        }
#else
		disable_seccomp();
#endif

#ifdef KSU_KPROBES_HOOK
		ksu_set_task_tracepoint_flag(current);
#endif
	} else {
#ifdef KSU_KPROBES_HOOK
		ksu_clear_task_tracepoint_flag_if_needed(current);
#endif
    }

#ifdef CONFIG_KSU_SUSFS_SUS_PATH
extern struct work_struct susfs_extra_works;
#endif

    // Handle kernel umount
    ksu_handle_umount(old_uid, new_uid);

#ifdef CONFIG_KSU_SUSFS_TRY_UMOUNT
    // let SUSFS umount its own user-defined paths in the new mnt ns
    susfs_try_umount(new_uid);
#endif

#ifdef CONFIG_KSU_SUSFS
    /*
     * Mark the freshly spawned app here rather than relying on
     * ksu_handle_umount() to have done it.
     *
     * ksu_handle_umount() returns early when ksu_module_mounted is false,
     * when the kernel_umount feature is disabled, when the uid is not on the
     * umount list, or when the caller is not a zygote child. Only the last
     * case is a real precondition. SUS_PATH, SUS_MAP and the sus_path_loop
     * all gate on susfs_is_current_proc_umounted_app(), so whenever any of
     * those early exits is taken the flag is never set and every hiding rule
     * silently does nothing for that process - root and shell still see the
     * paths, apps see them too.
     *
     * The reference trees set this in the setuid hook for exactly this
     * reason, independent of the umount machinery.
     */
    if (susfs_zygote_sid != 0 && is_zygote(current_cred()) &&
        !susfs_is_current_proc_umounted() &&
        is_appuid(new_uid) && ksu_uid_should_umount(new_uid)) {
        susfs_set_current_proc_umounted();

#ifdef CONFIG_KSU_SUSFS_SUS_PATH
        /*
         * sus_path_loop rules are keyed by pathname, not pinned to an inode,
         * so they have to be re-applied once the target inode may have been
         * re-instantiated. fs/susfs.c arms susfs_extra_works with INIT_WORK
         * and susfs_run_sus_path_loop() is its only consumer - nothing ever
         * queued it, so CMD_SUSFS_ADD_SUS_PATH_LOOP rules were inert.
         * Deferred to the workqueue so the spawned process is never blocked.
         */
        if (!work_pending(&susfs_extra_works))
            schedule_work(&susfs_extra_works);
#endif
    }
#endif

    return 0;
}

void __init ksu_setuid_hook_init(void)
{
	ksu_kernel_umount_init();
}

void __exit ksu_setuid_hook_exit(void)
{
	pr_info("ksu_core_exit\n");
	ksu_kernel_umount_exit();
}