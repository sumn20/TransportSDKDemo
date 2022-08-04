//
// Created by Sumn on 2022/8/1.
//

#ifndef TRANSPORTSDKDEMO_VIDEODATALISTENER_H
#define TRANSPORTSDKDEMO_VIDEODATALISTENER_H
#include "trtc_cloud.h"

class VideoDataListener {
public:
    VideoDataListener(){};

    virtual ~VideoDataListener(){};

    virtual void actionVideoData(liteav::VideoFrame frame) = 0;
};


#endif //TRANSPORTSDKDEMO_VIDEODATALISTENER_H
