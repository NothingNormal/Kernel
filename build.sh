#!/bin/bash
function compile()
{
source ~/.bashrc && source ~/.profile
export LC_ALL=C && export USE_CCACHE=1
ccache -M 25G
TANGGAL=$(date +"%Y%m%d-%H")
export ARCH=arm64
export KBUILD_BUILD_HOST=android-build
export KBUILD_BUILD_USER="kim_chi_96"
clangbin=clang/bin/clang
if ! [ -a $clangbin ]; then git clone --depth=1 https://bitbucket.org/nothinghere69/neutron.git clang
fi
gcc64bin=gcc64/bin/aarch64-linux-android-as
if ! [ -a $gcc64bin ]; then git clone --depth=1 https://github.com/LineageOS/android_prebuilts_gcc_linux-x86_aarch64_aarch64-linux-android-4.9 gcc64
fi
gcc32bin=gcc32/bin/arm-linux-androideabi-as
if ! [ -a $gcc32bin ]; then git clone --depth=1 https://github.com/LineageOS/android_prebuilts_gcc_linux-x86_arm_arm-linux-androideabi-4.9 gcc32
fi
rm -rf AnyKernel
make O=out ARCH=arm64 vendor/xiaomi/miatoll_defconfig
export PATH="/usr/bin:/bin:${PWD}/clang/bin:${PWD}/gcc64/bin:${PWD}/gcc32/bin:$PATH"
make -j$(nproc --all) O=out \
                      ARCH=arm64 \
                      LLVM="/usr/bin/" \
                      LLVM_IAS=1 \
                      CC="/usr/bin/clang" \
                      CLANG_TRIPLE=aarch64-linux-gnu- \
                      CROSS_COMPILE="${PWD}/gcc64/bin/aarch64-linux-android-" \
                      CROSS_COMPILE_ARM32="${PWD}/gcc32/bin/arm-linux-androideabi-" \
                      LD="/usr/bin/ld.lld" \
                      NM="/usr/bin/llvm-nm" \
                      OBJCOPY="/usr/bin/llvm-objcopy" \
                      CONFIG_NO_ERROR_ON_MISMATCH=y
}
function zupload()
{
zimage=out/arch/arm64/boot/Image.gz
if ! [ -a $zimage ];
then
echo  " Failed To Compile Kernel"
else
echo -e " Kernel Compile Successful"
git clone --depth=1 https://github.com/arshad-jamil33/AnyKernel3.git -b Black AnyKernel
cp out/arch/arm64/boot/Image.gz AnyKernel
cd AnyKernel
zip -r9 BlackHole-V5-ABYSS-NON-KSU-miatoll-${TANGGAL}.zip *
cd ../
fi
}
compile
zupload
