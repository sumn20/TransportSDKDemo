//
// Created by Sumn on 2022/7/19.
//

#ifndef TRANSPORTSDKDEMO_FFMPEGVIDEODECODER_H
#define TRANSPORTSDKDEMO_FFMPEGVIDEODECODER_H
#define INBUF_SIZE 4096

#include <jni.h>
#include "iostream"
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "OpenGlRender.h"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavutil/frame.h"
}

class FFmpegVideoDecoder {
public:
    FFmpegVideoDecoder(ANativeWindow *nwin);

    ~FFmpegVideoDecoder();

    int decode_video(const uint8_t *data, size_t len, int pts, int dts);

private:
    int dumpYUV(AVFrame *frame, FILE *f);

private:
    AVCodec *codec = NULL;
    AVCodecContext *avcodec_ctx = NULL;
    AVPacket *pkt = NULL;
    AVFrame *frame = NULL;
    // 解析器上下文
    AVCodecParserContext *parser = NULL;
    FILE *f_out = NULL;
    OpenGlRender *glRender=NULL;

};


#endif //TRANSPORTSDKDEMO_FFMPEGVIDEODECODER_H
