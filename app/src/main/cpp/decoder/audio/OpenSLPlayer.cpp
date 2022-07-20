//
// Created by Sumn on 2022/7/20.
//

#include <log_util.h>
#include "OpenSLPlayer.h"

OpenSLPlayer::OpenSLPlayer() {
    LOGCATI("OpenSLPlayer()");
    //第一步，创建引擎对象------------------------------------------
    //1.创建
    slCreateEngine(&engineObject, 0, 0, 0, 0, 0);
    //2.实现
    (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    //3.得到具体的接口
    (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);

    //第二步，创建混音器-------------------------------------------
    const SLInterfaceID slitfids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean slitfreqs[1] = {SL_BOOLEAN_FALSE};
    //创建混音器
    (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, slitfids, slitfreqs);
    //实现混音器
    (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    //得到具体的接口
    (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB,
                                     &outputEnvironmentalReverbItf);
    //设置环境属性
    (*outputEnvironmentalReverbItf)->SetEnvironmentalReverbProperties(outputEnvironmentalReverbItf,
                                                                      &reverbSettings);

    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};

    // 第三步--------------------------------------------
    // 创建播放器
    SLDataLocator_AndroidSimpleBufferQueue android_queue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
                                                            2};
    //pcm格式信息
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,//播放pcm格式的数据
            2,//2个声道（立体声）
            SL_SAMPLINGRATE_48,//16000hz的频率
            SL_PCMSAMPLEFORMAT_FIXED_16,//位数 16位
            SL_PCMSAMPLEFORMAT_FIXED_16,//和位数一致就行
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,//立体声（前左前右）
            SL_BYTEORDER_LITTLEENDIAN//结束标志
    };
    //数据源，队列，pcm
    SLDataSource slDataSource = {&android_queue, &pcm};
    SLDataSink audioSnk = {&outputMix, NULL};
    const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND, SL_IID_VOLUME};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
    (*engineEngine)->CreateAudioPlayer(engineEngine, &pcmPlayerObject, &slDataSource, &audioSnk, 3,
                                       ids, req);
    (*pcmPlayerObject)->Realize(pcmPlayerObject, SL_BOOLEAN_FALSE);
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_PLAY, &pcmPlayerPlay);

    //第四步，创建缓冲区和回调函数---------------------------------------
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_BUFFERQUEUE, &pcmBufferQueue);
    //缓冲接口回调
    //  (*pcmBufferQueue)->RegisterCallback(pcmBufferQueue, pcmBufferCallBack, NULL);
    //获取音量接口
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_VOLUME, &pcmPlayerVolume);

    //第五步，设置播放状态----------------------------------------
    (*pcmPlayerPlay)->SetPlayState(pcmPlayerPlay, SL_PLAYSTATE_STOPPED);


}

OpenSLPlayer::~OpenSLPlayer() {
    LOGCATI("~OpenSLPlayer()");
    if (pcmPlayerObject != NULL) {
        (*pcmPlayerObject)->Destroy(pcmPlayerObject);
        pcmPlayerObject = NULL;
        pcmBufferQueue = NULL;
    }

    if (outputMixObject != NULL) {
        (*outputMixObject)->Destroy(outputMixObject);
        outputMixObject = NULL;
        outputEnvironmentalReverbItf = NULL;
    }

    if (engineObject != NULL) {
        (*engineObject)->Destroy(engineObject);
        engineObject = NULL;
        engineEngine = NULL;
    }
}

void OpenSLPlayer::Play(const uint8_t *pcmData, size_t len) {
    SLuint32 pState;
    (*pcmPlayerPlay)->GetPlayState(pcmPlayerPlay, &pState);
    if (pState == SL_PLAYSTATE_STOPPED) {
        (*pcmPlayerPlay)->SetPlayState(pcmPlayerPlay, SL_PLAYSTATE_PLAYING);
    }
    (*pcmBufferQueue)->Enqueue(pcmBufferQueue, pcmData, (int)len);
}
