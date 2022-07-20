cmake_minimum_required(VERSION 3.18.1)
project("transportsdkdemo")
#头文件设置
include_directories(
        third_party
        third_party/libffmpeg
        third_party/boringssl
        util
        render
        libtrtc
        sdkinterface
        decoder
        decoder/video
)
#业务代码设置
file(GLOB src-files
        ${CMAKE_SOURCE_DIR}/*.cpp
        ${CMAKE_SOURCE_DIR}/*/*.cpp
        ${CMAKE_SOURCE_DIR}/*/*/*.cpp
        )
add_library(
        transportsdkdemo
        SHARED
        ${src-files})



find_library(
        log-lib
        log)
find_library(
        libz-lib
        z)

set(jnilibs "${CMAKE_SOURCE_DIR}/../jniLibs")
add_library(avcodec SHARED IMPORTED)
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION ${jnilibs}/${ANDROID_ABI}/libavcodec.so)

add_library(avdevice SHARED IMPORTED)
set_target_properties(avdevice PROPERTIES IMPORTED_LOCATION ${jnilibs}/${ANDROID_ABI}/libavdevice.so)

add_library(avfilter SHARED IMPORTED)
set_target_properties(avfilter PROPERTIES IMPORTED_LOCATION ${jnilibs}/${ANDROID_ABI}/libavfilter.so)


add_library(avformat SHARED IMPORTED)
set_target_properties(avformat PROPERTIES IMPORTED_LOCATION ${jnilibs}/${ANDROID_ABI}/libavformat.so)

add_library(avutil SHARED IMPORTED)
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION ${jnilibs}/${ANDROID_ABI}/libavutil.so)

add_library(swresample SHARED IMPORTED)
set_target_properties(swresample PROPERTIES IMPORTED_LOCATION ${jnilibs}/${ANDROID_ABI}/libswresample.so)

add_library(swscale SHARED IMPORTED)
set_target_properties(swscale PROPERTIES IMPORTED_LOCATION ${jnilibs}/${ANDROID_ABI}/libswscale.so)

add_library(trtc SHARED IMPORTED )
set_target_properties(trtc PROPERTIES IMPORTED_LOCATION ${jnilibs}/${ANDROID_ABI}/libtrtc.so)

add_library(ssl STATIC IMPORTED )
set_target_properties(ssl PROPERTIES IMPORTED_LOCATION ${jnilibs}/${ANDROID_ABI}/libssl.a)

add_library(crypto STATIC IMPORTED )
set_target_properties(crypto PROPERTIES IMPORTED_LOCATION ${jnilibs}/${ANDROID_ABI}/libcrypto.a)


set(third-party-libs
        avcodec
        avdevice
        avfilter
        avformat
        avutil
        swresample
        swscale
        ssl
        crypto
        trtc
        )


target_link_libraries(
        transportsdkdemo
        ${log-lib}
        ${libz-lib}
        ${third-party-libs}
        OpenSLES
        EGL
        GLESv2
        android
        atomic
)