#!/bin/bash
export NDK=/Users/sumn/Library/Android/sdk/ndk/21.1.6352462
# 当前系统
export HOST_TAG=darwin-x86_64
# 支持的 Android CUP 架构
export ARCH=aarch64
export CPU=armv8-a
#export ARCH=armv7a
#export CPU=armv7-a
# 支持的 Android 最低系统版本
export MIN=21
export ANDROID_NDK_PLATFORM=android-21

export PREFIX=$(pwd)/android/$CPU
export MIN_PLATFORM=$NDK/platforms/android-$MIN
export SYSROOT=$NDK/sysroot
export TOOLCHAIN=$NDK/toolchains/llvm/prebuilt/$HOST_TAG
if [ "$ARCH" = "armv7a" ]
then
  #armv7-a配置
  export AR=$TOOLCHAIN/bin/arm-linux-androideabi-ar
  export AS=$TOOLCHAIN/bin/arm-linux-androideabi-as
  export CC=$TOOLCHAIN/bin/$ARCH-linux-androideabi$MIN-clang
  echo "-----------------------------"
  echo $CC
  export CXX=$TOOLCHAIN/bin/$ARCH-linux-androideabi$MIN-clang++
  export LD=$TOOLCHAIN/bin/arm-linux-androideabi-ld
  export NM=$TOOLCHAIN/bin/arm-linux-androideabi-nm
  export RANLIB=$TOOLCHAIN/bin/arm-linux-androideabi-ranlib
  export STRIP=$TOOLCHAIN/bin/arm-linux-androideabi-strip
  OPTIMIZE_CFLAGS="-I/Users/sumn/third_party/ffmpeg-4.2.7/x264-master/android/arm/include -DANDROID -I$NDK/sysroot/usr/include/arm-linux-androideabi/"
  ADDI_LDFLAGS="-Wl,-rpath-link=$MIN_PLATFORM/arch-arm/usr/lib -L/Users/sumn/third_party/ffmpeg-4.2.7/x264-master/android/arm/lib -L$MIN_PLATFORM/arch-arm/usr/lib -nostdlib"
else
  #armv8-a配置
  export AR=$TOOLCHAIN/bin/aarch64-linux-android-ar
  export AS=$TOOLCHAIN/bin/aarch64-linux-android-as
  export CC=$TOOLCHAIN/bin/$ARCH-linux-android$MIN-clang
  echo "-----------------------------"
  echo $CC
  export CXX=$TOOLCHAIN/bin/$ARCH-linux-android$MIN-clang++
  export LD=$TOOLCHAIN/bin/aarch64-linux-android-ld
  export NM=$TOOLCHAIN/bin/aarch64-linux-android-nm
  export RANLIB=$TOOLCHAIN/bin/aarch64-linux-android-ranlib
  export STRIP=$TOOLCHAIN/bin/aarch64-linux-androideabi-strip
  OPTIMIZE_CFLAGS="-I/Users/sumn/third_party/ffmpeg-4.2.7/x264-master/android/arm64/include -DANDROID -I$NDK/sysroot/usr/include/arm64-linux-android/"
  ADDI_LDFLAGS="-Wl,-rpath-link=$MIN_PLATFORM/arch-arm64/usr/lib -L/Users/sumn/third_party/ffmpeg-4.2.7/x264-master/android/arm64/lib -L$MIN_PLATFORM/arch-arm64/usr/lib -nostdlib"
fi

sed  -i "" "s/SLIBNAME_WITH_MAJOR='\$(SLIBNAME).\$(LIBMAJOR)'/SLIBNAME_WITH_MAJOR='\$(SLIBPREF)\$(FULLNAME)-\$(LIBMAJOR)\$(SLIBSUF)'/" configure
sed  -i "" "s/LIB_INSTALL_EXTRA_CMD='\$\$(RANLIB) \"\$(LIBDIR)\\/\$(LIBNAME)\"'/LIB_INSTALL_EXTRA_CMD='\$\$(RANLIB) \"\$(LIBDIR)\\/\$(LIBNAME)\"'/" configure
sed  -i "" "s/SLIB_INSTALL_NAME='\$(SLIBNAME_WITH_VERSION)'/SLIB_INSTALL_NAME='\$(SLIBNAME_WITH_MAJOR)'/" configure
sed  -i "" "s/SLIB_INSTALL_LINKS='\$(SLIBNAME_WITH_MAJOR) \$(SLIBNAME)'/SLIB_INSTALL_LINKS='\$(SLIBNAME)'/" configure
sed -i -e 's/#define getenv(x) NULL/\/\*#define getenv(x) NULL\*\//g' config.h
# sed  -i "" "s/SHFLAGS='-shared -Wl,-soname,\$(SLIBNAME)'/SHFLAGS='-shared -soname \$(SLIBNAME)'/" configure
# sed  -i "" "s/-Wl//g" configure

./configure \
--prefix=$PREFIX \
--ar=$AR \
--as=$AS \
--cc=$CC \
--cxx=$CXX \
--nm=$NM \
--ranlib=$RANLIB \
--strip=$STRIP \
--arch=$ARCH \
--target-os=android \
--enable-cross-compile \
--disable-asm \
--enable-gpl \
--enable-libx264 \
--enable-encoder=libx264 \
--enable-jni \
--enable-neon \
--enable-mediacodec \
--enable-shared \
--disable-static \
--disable-ffprobe \
--disable-ffplay \
--disable-ffmpeg \
--disable-debug \
--disable-symver \
--disable-stripping \
--extra-cflags="-I/Users/sumn/third_party/ffmpeg-4.2.7/x264-master/android/arm64/include" \
--extra-ldflags="-L/Users/sumn/third_party/ffmpeg-4.2.7/x264-master/android/arm64/lib" \

sed  -i "" "s/#define HAVE_TRUNC 0/#define HAVE_TRUNC 1/" config.h
sed  -i "" "s/#define HAVE_TRUNCF 0/#define HAVE_TRUNCF 1/" config.h
sed  -i "" "s/#define HAVE_RINT 0/#define HAVE_RINT 1/" config.h
sed  -i "" "s/#define HAVE_LRINT 0/#define HAVE_LRINT 1/" config.h
sed  -i "" "s/#define HAVE_LRINTF 0/#define HAVE_LRINTF 1/" config.h
sed  -i "" "s/#define HAVE_ROUND 0/#define HAVE_ROUND 1/" config.h
sed  -i "" "s/#define HAVE_ROUNDF 0/#define HAVE_ROUNDF 1/" config.h
sed  -i "" "s/#define HAVE_CBRT 0/#define HAVE_CBRT 1/" config.h
sed  -i "" "s/#define HAVE_CBRTF 0/#define HAVE_CBRTF 1/" config.h
sed  -i "" "s/#define HAVE_COPYSIGN 0/#define HAVE_COPYSIGN 1/" config.h
sed  -i "" "s/#define HAVE_ERF 0/#define HAVE_ERF 1/" config.h
sed  -i "" "s/#define HAVE_HYPOT 0/#define HAVE_HYPOT 1/" config.h
sed  -i "" "s/#define HAVE_ISNAN 0/#define HAVE_ISNAN 1/" config.h
sed  -i "" "s/#define HAVE_ISFINITE 0/#define HAVE_ISFINITE 1/" config.h
sed  -i "" "s/#define HAVE_INET_ATON 0/#define HAVE_INET_ATON 1/" config.h
sed  -i "" "s/#define getenv(x) NULL/\\/\\/ #define getenv(x) NULL/" config.h
