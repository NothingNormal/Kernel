#!/bin/bash
function compile()
{
[ -f ~/.bashrc ] && source ~/.bashrc >/dev/null 2>&1
[ -f ~/.profile ] && source ~/.profile >/dev/null 2>&1
export LC_ALL=C
TANGGAL=$(date +"%Y%m%d-%H")
export ARCH=arm64
export KBUILD_BUILD_HOST=android-build
export KBUILD_BUILD_USER="unknown"

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

# ccache compiler cache (no-op when ccache is not installed)
CCACHE_BIN=""
if command -v ccache >/dev/null 2>&1; then
CCACHE_BIN="ccache "
export CCACHE_DIR="${CCACHE_DIR:-$HOME/.ccache}"
ccache -M "${CCACHE_MAXSIZE:-8G}" >/dev/null 2>&1
# CCACHE_DIR is restored from the Actions cache, and ccache keeps its own
# counters in there, so without this the summary printed after the build
# accumulates across runs and does not describe the build that just happened.
if ! ccache -z >/dev/null 2>&1; then
    echo "WARNING: ccache statistics could not be reset" >&2
fi
echo "ccache enabled: $(ccache --version 2>/dev/null | head -1)"
fi

rm -rf AnyKernel
make O=out ARCH=arm64 vendor/xiaomi/miatoll_defconfig || { echo " Failed to generate .config"; exit 1; }
export PATH="${TOOLCHAIN_BIN}:/usr/bin:/bin:$PATH"
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
zip -r9 Sixfaced-V5-KSU-miatoll-${TANGGAL}.zip *
cd ../
fi
}
compile
zupload
