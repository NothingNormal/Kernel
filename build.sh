#!/bin/bash
function compile()
{
[ -f ~/.bashrc ] && source ~/.bashrc >/dev/null 2>&1
[ -f ~/.profile ] && source ~/.profile >/dev/null 2>&1
# Always build from the top of the tree, so "bash /path/to/build.sh" works from
# any cwd: kbuild needs cwd to be the source dir even when O=out is given.
cd "$(dirname "$(readlink -f "$0")")" || exit 1
export LC_ALL=C
TANGGAL=$(date +"%Y%m%d-%H")
export ARCH=arm64
export KBUILD_BUILD_HOST=android-build
export KBUILD_BUILD_USER="unknown"

# ---------------------------------------------------------------------------
# Host prerequisites. Check them here, before anything is downloaded, so a
# missing tool says so instead of surfacing as "failed to extract Neutron" or
# a kernel error 20 minutes in.
# ---------------------------------------------------------------------------
APT_DEPS="clang lld llvm zip ccache pigz zstd bc bison flex libssl-dev libelf-dev lz4 lzop"
missing=""
for tool in make tar zip nproc; do
    command -v "$tool" >/dev/null 2>&1 || missing="$missing $tool"
done
# one of curl/wget is enough to fetch the toolchain
command -v curl >/dev/null 2>&1 || command -v wget >/dev/null 2>&1 || missing="$missing curl"
# kconfig needs these; kbuild only complains once it is already configuring
for tool in bc bison flex; do
    command -v "$tool" >/dev/null 2>&1 || missing="$missing $tool"
done
if [ -n "$missing" ]; then
    echo "ERROR: missing host tools:$missing" >&2
    echo "Install them (Debian/Ubuntu):" >&2
    echo "  sudo apt-get install -y --no-install-recommends $APT_DEPS" >&2
    exit 1
fi
# zstd is only needed to unpack the Neutron tarball; without it the script
# would silently fall back to system clang and then die on -mllvm -polly.
if ! command -v zstd >/dev/null 2>&1 && ! command -v unzstd >/dev/null 2>&1; then
    echo "WARNING: no zstd/unzstd - cannot unpack the Neutron .tar.zst, will try system LLVM"
fi

# Same flag set kbuild passes when CONFIG_LLVM_POLLY=y (see the Makefile).
# An LLVM without Polly accepts -mllvm -polly (it is only an unknown *pass*
# name, which LLVM tolerates) but rejects the individual -polly-* options, so
# the probe must use the full list to be meaningful.
POLLY_FLAGS="-mllvm -polly -mllvm -polly-run-inliner \
 -mllvm -polly-ast-use-context -mllvm -polly-detect-keep-going \
 -mllvm -polly-invariant-load-hoisting -mllvm -polly-vectorizer=stripmine \
 -mllvm -polly-loopfusion-greedy=1 -mllvm -polly-reschedule=1 \
 -mllvm -polly-postopts=1 -mllvm -polly-num-threads=0 \
 -mllvm -polly-omp-backend=LLVM -mllvm -polly-scheduling=dynamic \
 -mllvm -polly-scheduling-chunksize=1 -mllvm -polly-run-dce"

polly_probe()
{
    probe_dir=$(mktemp -d 2>/dev/null) || probe_dir=/tmp
    printf 'int probe(int a, int b) { return a * b + 1; }\n' > "${probe_dir}/polly-probe.c"
    # shellcheck disable=SC2086
    if "$1" -target aarch64-linux-gnu -O2 $POLLY_FLAGS \
         -c "${probe_dir}/polly-probe.c" -o "${probe_dir}/polly-probe.o" \
         >"${probe_dir}/polly-probe.log" 2>&1; then
        rm -rf "$probe_dir"
        return 0
    fi
    echo "ERROR: $1 does not accept the Polly flags that CONFIG_LLVM_POLLY=y requires:" >&2
    sed -n '1,15p' "${probe_dir}/polly-probe.log" >&2
    echo "Fix: use a Polly-enabled Neutron build, or set" >&2
    echo "  CONFIG_LLVM_POLLY=n  in arch/arm64/configs/vendor/xiaomi/miatoll_defconfig" >&2
    rm -rf "$probe_dir"
    return 1
}

# LLVM-only toolchain: NO GNU GCC / binutils anywhere in the target
# toolchain. Every binutils role (ar/nm/objcopy/objdump/readelf/strip/
# size/ld) is served by its llvm-* twin; assembly goes through clang's
# integrated assembler (LLVM_IAS=1). This tree has no 32-bit compat
# vDSO (vdso32 removed), so no arm32 GCC cross is needed either.
# CROSS_COMPILE is kept ONLY so kbuild derives the clang --target
# triple; no $(CROSS_COMPILE)* binary is ever invoked.
#
# Primary: Neutron Clang — LLVM built with PGO + BOLT + LTO + O3 and
# Polly bundled, specifically tuned for kernel compilation (the
# compiler binary itself is profile-optimized, so it also compiles
# faster than a generic distro clang). Pinned release tag for
# reproducible builds; override with NEUTRON_TAG / NEUTRON_DIR env.
# Fallback: system LLVM (apt / apt.llvm.org) if the download or the
# toolchain sanity check fails, so a Neutron outage can never break CI.
NEUTRON_TAG="${NEUTRON_TAG:-06092026}"
NEUTRON_DIR="${NEUTRON_DIR:-$HOME/neutron-clang}"
TOOLCHAIN_BIN=""
if [ -x "${NEUTRON_DIR}/bin/clang" ]; then
    TOOLCHAIN_BIN="${NEUTRON_DIR}/bin"
else
    rm -rf "${NEUTRON_DIR}"; mkdir -p "${NEUTRON_DIR}"
    NEUTRON_URL="https://github.com/Neutron-Toolchains/clang-build-catalogue/releases/download/${NEUTRON_TAG}/neutron-clang-${NEUTRON_TAG}.tar.zst"
    NEUTRON_TAR="/tmp/neutron-clang-${NEUTRON_TAG}.tar.zst"
    echo "Downloading Neutron Clang ${NEUTRON_TAG} ..."
    if { command -v curl >/dev/null 2>&1 && curl -fL --retry 3 --connect-timeout 20 -o "${NEUTRON_TAR}" "${NEUTRON_URL}"; } \
       || wget -q --tries=3 -O "${NEUTRON_TAR}" "${NEUTRON_URL}"; then
        if tar --use-compress-program=unzstd -xf "${NEUTRON_TAR}" -C "${NEUTRON_DIR}" 2>/dev/null \
           || tar -I zstd -xf "${NEUTRON_TAR}" -C "${NEUTRON_DIR}"; then
            rm -f "${NEUTRON_TAR}"
            if [ -x "${NEUTRON_DIR}/bin/clang" ]; then
                TOOLCHAIN_BIN="${NEUTRON_DIR}/bin"
            else
                # tarball may nest everything under a top-level dir
                NEUTRON_CC=$(find "${NEUTRON_DIR}" -maxdepth 4 -type f -name clang -path '*/bin/*' 2>/dev/null | head -1)
                [ -n "${NEUTRON_CC}" ] && TOOLCHAIN_BIN=$(dirname "${NEUTRON_CC}")
            fi
        else
            echo "WARNING: failed to extract Neutron tarball"
        fi
    else
        echo "WARNING: failed to download Neutron Clang"
    fi
fi
if [ -n "${TOOLCHAIN_BIN}" ] && [ -x "${TOOLCHAIN_BIN}/clang" ] && [ -x "${TOOLCHAIN_BIN}/ld.lld" ] && [ -x "${TOOLCHAIN_BIN}/llvm-ar" ]; then
    echo "Using Neutron Clang toolchain: ${TOOLCHAIN_BIN}"
elif [ -x /usr/bin/clang ] && [ -x /usr/bin/ld.lld ]; then
    TOOLCHAIN_BIN=/usr/bin
    echo "Neutron unavailable - falling back to system LLVM: ${TOOLCHAIN_BIN}"
else
    echo "ERROR: no usable LLVM toolchain found" >&2
    exit 1
fi
"${TOOLCHAIN_BIN}/clang" --version | head -2

# Fail here rather than 20 minutes into the build, on whichever toolchain the
# selection above actually picked.
polly_probe "${TOOLCHAIN_BIN}/clang" || exit 1

# ccache compiler cache (no-op when ccache is not installed)
CCACHE_BIN=""
if command -v ccache >/dev/null 2>&1; then
CCACHE_BIN="ccache "
export CCACHE_DIR="${CCACHE_DIR:-$HOME/.ccache}"
ccache -M "${CCACHE_MAXSIZE:-8G}" >/dev/null 2>&1
# Hash the compiler binary instead of stat'ing it. The toolchain arrives as
# an extracted tarball (or an Actions cache restore of one), so its mtime is
# not stable across runs - with the default mtime check that alone throws
# the whole restored cache away and every build starts cold.
export CCACHE_COMPILERCHECK=content
# The kernel embeds __DATE__/__TIME__ and build paths; letting those
# invalidate entries defeats the cache for no gain in correctness here.
export CCACHE_SLOPPINESS="time_macros,include_file_mtime,include_file_ctime,modules_install"
# Do not fold the build directory into the hash: the checkout path differs
# between the runner and a local build.
export CCACHE_NOHASHDIR=1
export CCACHE_BASEDIR="$(pwd)"
echo "ccache enabled: $(ccache --version 2>/dev/null | head -1)"
fi

rm -rf AnyKernel
make O=out ARCH=arm64 vendor/xiaomi/miatoll_defconfig || { echo " Failed to generate .config"; exit 1; }
# keep the caller PATH: only the toolchain dir needs to win, and hardcoding
# /usr/bin:/bin ahead of $PATH silently hid a caller-supplied tool dir
# (found during a stubbed dry run of this script)
export PATH="${TOOLCHAIN_BIN}:$PATH"
# multithreaded gzip for Image.gz when pigz is available
KGZIP_BIN=gzip
command -v pigz >/dev/null 2>&1 && KGZIP_BIN=pigz
# slight overcommit: clang jobs stall on I/O; +2 keeps the vCPUs saturated
make -j$(( $(nproc --all) + 2 )) O=out \
                      KGZIP=${KGZIP_BIN} \
                      ARCH=arm64 \
                      LLVM=1 \
                      LLVM_IAS=1 \
                      CC="${CCACHE_BIN}${TOOLCHAIN_BIN}/clang" \
                      HOSTCC="${CCACHE_BIN}${TOOLCHAIN_BIN}/clang" \
                      HOSTCXX="${TOOLCHAIN_BIN}/clang++" \
                      CLANG_TRIPLE=aarch64-linux-gnu- \
                      CROSS_COMPILE=aarch64-linux-gnu- \
                      LD="${TOOLCHAIN_BIN}/ld.lld" \
                      AR="${TOOLCHAIN_BIN}/llvm-ar" \
                      NM="${TOOLCHAIN_BIN}/llvm-nm" \
                      OBJCOPY="${TOOLCHAIN_BIN}/llvm-objcopy" \
                      OBJDUMP="${TOOLCHAIN_BIN}/llvm-objdump" \
                      READELF="${TOOLCHAIN_BIN}/llvm-readelf" \
                      STRIP="${TOOLCHAIN_BIN}/llvm-strip" \
                      OBJSIZE="${TOOLCHAIN_BIN}/llvm-size" \
                      CONFIG_NO_ERROR_ON_MISMATCH=y
rc=$?
if command -v ccache >/dev/null 2>&1; then ccache -s 2>/dev/null | head -15; fi
if [ $rc -ne 0 ]; then
echo " Kernel build failed (make exit code $rc)"
exit $rc
fi
}
function zupload()
{
zimage=out/arch/arm64/boot/Image.gz
if ! [ -a $zimage ];
then
echo  " Failed To Compile Kernel"
exit 1
else
echo -e " Kernel Compile Successful"
cp -a AnyKernel3 AnyKernel
cp out/arch/arm64/boot/Image.gz AnyKernel
cd AnyKernel
zip -r9 Sixfaced-V5-KSUN-miatoll-${TANGGAL}.zip *
cd ../
fi
}
compile
zupload
