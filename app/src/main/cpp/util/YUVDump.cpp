//
// Created by Sumn on 2022/8/4.
//

#include "YUVDump.h"

int YUVDump::dumpYUV2AVFrame(AVFrame *frame) {
    // 写Y分量
    for (size_t i = 0; i < frame->height; i++) {
        fwrite(frame->data[0] + frame->linesize[0] * i, 1, frame->width, pYuvOut);
    }

    // 写U分量
    for (size_t i = 0; i < frame->height / 2; i++) {
        fwrite(frame->data[1] + frame->linesize[1] * i, 1, frame->width / 2, pYuvOut);
    }

    // 写V分量
    for (size_t i = 0; i < frame->height / 2; i++) {
        fwrite(frame->data[2] + frame->linesize[2] * i, 1, frame->width / 2, pYuvOut);
    }
    return 0;
}
int YUVDump::dumpYUV2Buffer(YuvData *yuvData) {

    return 0;
}
YUVDump::YUVDump() {
    pYuvOut = fopen("/storage/emulated/0/Android/data/com.tencent.transportsdkdemo/cache/test.yuv",
                  "wb+");
}

YUVDump::~YUVDump() {
    fclose(pYuvOut);
    free(pYuvOut);
}


