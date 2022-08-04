//
// Created by Sumn on 2022/7/26.
//

#ifndef TRANSPORTSDKDEMO_FFMPEGVIDEOENCODE_H
#define TRANSPORTSDKDEMO_FFMPEGVIDEOENCODE_H

#include <cstdint>
#include <YUVDump.h>
#include "VideoDataListener.h"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavutil/frame.h"
#include "libavutil/error.h"
#include <libavutil/opt.h>
}


class FFmpegVideoEncode {
public:
    FFmpegVideoEncode();
    ~FFmpegVideoEncode();
    void encode(YuvVideo* yuvVideo);
    void setVideoDataListener(VideoDataListener *videoDataListener);
private:
    VideoDataListener *pVideoDataListener=NULL;
    AVCodecContext *pCodecCtx=NULL;
    AVCodec *pCodec=NULL;
    AVFrame* pFrame=NULL;
    AVPacket *pAvPacket=NULL;
    int width = 368;
    int height = 640;
    int video_frame_index=0;
    FILE *f_out = NULL;
};


#endif //TRANSPORTSDKDEMO_FFMPEGVIDEOENCODE_H
