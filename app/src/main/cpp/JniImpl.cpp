#include <jni.h>
#include <trtc_cloud.h>
#include "user_sig_generator.h"
#include "TRTCCloudCore.h"
//
// Created by Sumn on 2022/7/20.
//
#ifdef __cplusplus
extern "C" {
#include "libavutil/ffversion.h"
#endif
JNIEXPORT jstring JNICALL
Java_com_tencent_transportsdkdemo_TRTCManager_getSDKVersion(JNIEnv *env, jclass clazz) {
    return env->NewStringUTF(FFMPEG_VERSION);
}
JNIEXPORT void JNICALL
Java_com_tencent_transportsdkdemo_TRTCManager_enterRoom(JNIEnv *env, jclass clazz, jint room_id,
                                                        jstring user_id, jobject surface) {

    const char *userId = env->GetStringUTFChars(user_id, nullptr);
    liteav::InitParams init_params;
    TRTCCloud::Initialize(init_params);
    liteav::EnterRoomParams params;
    RecordConfig recordConfig;
    recordConfig.output_sample_rate = 48000;
    recordConfig.output_channels = 2;
    recordConfig.enable_remote_audio_mix = true;
    params.sdk_app_id = SDKAPPID;
    params.user_id = userId;
    params.room_id = room_id;
    char sig[1024] = {0};
    size_t sig_size = 1024;
    GenerateUserSig(SDKAPPID, kSdkDemoKey, params.user_id,sig, &sig_size);
    params.user_sig = sig;
    params.scene = TRTC_SCENE_RECORD;
    params.role = TRTC_ROLE_ANCHOR;
    params.record_config = recordConfig;
    ANativeWindow *nwin = ANativeWindow_fromSurface(env, surface);
    TRTCCloudCore::GetInstance()->createDecoder(nwin);
    TRTCCloudCore::GetInstance()->getTRTCCloud()->EnterRoom(params);
    env->ReleaseStringUTFChars(user_id, userId);
}
JNIEXPORT void JNICALL
Java_com_tencent_transportsdkdemo_TRTCManager_exitRoom(JNIEnv *env, jclass clazz) {
    TRTCCloudCore::GetInstance()->getTRTCCloud()->ExitRoom();
    TRTCCloudCore::Destroy();
}

#ifdef __cplusplus
}
#endif
