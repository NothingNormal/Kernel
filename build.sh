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

# Toolchain: prefer system clang (CI / apt.llvm.org installs), fall back to
# the Neutron prebuilt clone for local builds without a system toolchain.
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

gcc64bin=gcc64/bin/aarch64-linux-android-as
if ! [ -a $gcc64bin ]; then git clone --depth=1 https://github.com/LineageOS/android_prebuilts_gcc_linux-x86_aarch64_aarch64-linux-android-4.9 gcc64
fi
gcc32bin=gcc32/bin/arm-linux-androideabi-as
if ! [ -a $gcc32bin ]; then git clone --depth=1 https://github.com/LineageOS/android_prebuilts_gcc_linux-x86_arm_arm-linux-androideabi-4.9 gcc32
fi
rm -rf AnyKernel
make O=out ARCH=arm64 vendor/xiaomi/miatoll_defconfig || { echo " Failed to generate .config"; exit 1; }
export PATH="${TOOLCHAIN_BIN}:/usr/bin:/bin:${PWD}/gcc64/bin:${PWD}/gcc32/bin:$PATH"
make -j$(nproc --all) O=out \
                      ARCH=arm64 \
                      LLVM="${TOOLCHAIN_BIN}/" \
                      LLVM_IAS=1 \
                      CC="${TOOLCHAIN_BIN}/clang" \
                      CLANG_TRIPLE=aarch64-linux-gnu- \
                      CROSS_COMPILE="${PWD}/gcc64/bin/aarch64-linux-android-" \
                      CROSS_COMPILE_ARM32="${PWD}/gcc32/bin/arm-linux-androideabi-" \
                      LD="${TOOLCHAIN_BIN}/ld.lld" \
                      NM="${TOOLCHAIN_BIN}/llvm-nm" \
                      OBJCOPY="${TOOLCHAIN_BIN}/llvm-objcopy" \
                      CONFIG_NO_ERROR_ON_MISMATCH=y
rc=$?
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
