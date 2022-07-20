//
// Created by Sumn on 2022/7/20.
//

#ifndef TRANSPORTSDKDEMO_OPENSLPLAYER_H
#define TRANSPORTSDKDEMO_OPENSLPLAYER_H

#include "SLES/OpenSLES.h"
#include "SLES/OpenSLES_Android.h"

class OpenSLPlayer {
public:
    OpenSLPlayer();

    ~OpenSLPlayer();

    void Play(const uint8_t *pcmData,size_t len);

private:
    // 引擎接口
    SLObjectItf engineObject = NULL;
    SLEngineItf engineEngine = NULL;
    //混音器
    SLEnvironmentalReverbSettings reverbSettings =
            SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;
    SLObjectItf outputMixObject = NULL;
    SLEnvironmentalReverbItf outputEnvironmentalReverbItf = NULL;
    //player
    SLObjectItf pcmPlayerObject = NULL;
    SLPlayItf pcmPlayerPlay;
    SLVolumeItf pcmPlayerVolume = NULL;

    //缓冲器队列接口
    SLAndroidSimpleBufferQueueItf pcmBufferQueue;
};


#endif //TRANSPORTSDKDEMO_OPENSLPLAYER_H
