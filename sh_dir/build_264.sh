#!/bin/sh
ANDROID_NDK="/Users/sumn/Library/Android/sdk/ndk/android-ndk-r14b"
SDK_VERSION=21
ARCH=arm64
#ARCH=arm

if [ "$ARCH" = "arm64" ]
then
    PLATFORM_PREFIX="aarch64-linux-android-"
    HOST="aarch64"
    PLATFORM_VERSION=4.9
else
    PLATFORM_PREFIX="arm-linux-androideabi-"
    HOST="arm"
    PLATFORM_VERSION=4.9
fi

PREFIX=$(pwd)/android/${ARCH}
SYSROOT=$ANDROID_NDK/platforms/android-${SDK_VERSION}/arch-${ARCH}
TOOLCHAIN=$ANDROID_NDK/toolchains/${PLATFORM_PREFIX}${PLATFORM_VERSION}/prebuilt/darwin-x86_64
CROSS_PREFIX=$TOOLCHAIN/bin/${PLATFORM_PREFIX}


./configure \
    --prefix=$PREFIX \
    --cross-prefix=$CROSS_PREFIX \
    --sysroot=$SYSROOT \
    --enable-static \
    --enable-pic \
    --disable-asm \
    --host=arm-linux \
