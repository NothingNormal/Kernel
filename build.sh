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
# Prefer system clang (CI / apt.llvm.org); fall back to the Neutron
# prebuilt clone for local builds without a system toolchain.
if [ -x /usr/bin/clang ] && [ -x /usr/bin/ld.lld ]; then
TOOLCHAIN_BIN=/usr/bin
else
clangbin=clang/bin/clang
if ! [ -a $clangbin ]; then git clone --depth=1 https://bitbucket.org/nothinghere69/neutron.git clang
fi
TOOLCHAIN_BIN="${PWD}/clang/bin"
fi
echo "Using LLVM toolchain at: ${TOOLCHAIN_BIN}"
"${TOOLCHAIN_BIN}/clang" --version | head -2

# ccache compiler cache (no-op when ccache is not installed)
CCACHE_BIN=""
if command -v ccache >/dev/null 2>&1; then
CCACHE_BIN="ccache "
export CCACHE_DIR="${CCACHE_DIR:-$HOME/.ccache}"
ccache -M "${CCACHE_MAXSIZE:-8G}" >/dev/null 2>&1
echo "ccache enabled: $(ccache --version 2>/dev/null | head -1)"
fi

rm -rf AnyKernel
make O=out ARCH=arm64 vendor/xiaomi/miatoll_defconfig || { echo " Failed to generate .config"; exit 1; }
export PATH="${TOOLCHAIN_BIN}:/usr/bin:/bin:$PATH"
make -j$(nproc --all) O=out \
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
