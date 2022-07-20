//
// Created by Sumn on 2022/7/19.
//

#include "FFmpegVideoDecoder.h"
#include "log_util.h"
#include <android/log.h>




FFmpegVideoDecoder::FFmpegVideoDecoder(ANativeWindow *nwin) {
    LOGCATI("FFmpegVideoDecoder()");
    //初始化ffmpeg 数据包
    pkt = av_packet_alloc();
    //初始化ffmpeg 帧对象
    frame = av_frame_alloc();
    // 1、找解码器
    codec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!codec) {
        LOGCATE("codec not found.");
    }
    // 2、初始化parser
    parser = av_parser_init(codec->id);
    if (!parser) {
        LOGCATE("parser not found.");
    }
    // 3、分配解码上下文 alloc codec context
    avcodec_ctx = avcodec_alloc_context3(codec);
    if (!avcodec_ctx) {
        LOGCATE("could not allocate avcodec_ctx.");
    }
    // 4、打开解码器
    int ret = avcodec_open2(avcodec_ctx, codec, NULL);
    if (ret < 0) {
        LOGCATE("could not open codec,ret=%d", ret);
    }
    glRender = new OpenGlRender(nwin);
    f_out = fopen("/storage/emulated/0/Android/data/com.tencent.transportsdkdemo/cache/test.yuv",
                  "wb+");
}

FFmpegVideoDecoder::~FFmpegVideoDecoder() {
    LOGCATI("~FFmpegVideoDecoder()");
    av_parser_close(parser);
    avcodec_free_context(&avcodec_ctx);
    av_frame_free(&frame);
    av_packet_free(&pkt);
    delete glRender;
    glRender = NULL;
}

int FFmpegVideoDecoder::dumpYUV(AVFrame *frame, FILE *f) {
    LOGCATE("1 [%s]", frame->data[0]);
    // 写Y分量
    for (size_t i = 0; i < frame->height; i++) {
        fwrite(frame->data[0] + frame->linesize[0] * i, 1, frame->width, f_out);
    }

    // 写U分量
    for (size_t i = 0; i < frame->height / 2; i++) {
        fwrite(frame->data[1] + frame->linesize[1] * i, 1, frame->width / 2, f);
    }

    // 写V分量
    for (size_t i = 0; i < frame->height / 2; i++) {
        fwrite(frame->data[2] + frame->linesize[2] * i, 1, frame->width / 2, f);
    }
    return 0;
}

int FFmpegVideoDecoder::decode_video(const uint8_t *data, size_t len, int pts, int dts) {
    //逐帧解析H264,I帧首次解析成功
    int ret = av_parser_parse2(parser, avcodec_ctx, &pkt->data, &pkt->size, data, (int) len, pts,
                               dts, 0);
    if (ret < 0) {
        LOGCATE("av_parser_parse2 error:%d,%d", ret, pkt->size);
        return -1;
    }
/*    LOGCATD("av_parser_parse2 success:%d,size:%d,%dx%d,format:%d", ret, pkt->size,
            avcodec_ctx->width, avcodec_ctx->height, avcodec_ctx->pix_fmt);*/
    if (pkt->size == 0) {
        //再次解析,这次是解B和P帧
        ret = av_parser_parse2(parser, avcodec_ctx, &pkt->data, &pkt->size, data, (int) len, pts,
                               dts, 0);
        if (ret < 0) {
            LOGCATE("av_parser_parse2 error:%d,%d", ret, pkt->size);
            return -1;
        }
    }
    int got_picture = 0;
    //获取解码数据,赋值到frame中
    ret = avcodec_decode_video2(avcodec_ctx, frame, &got_picture, pkt);
    if (ret < 0) {
        LOGCATE("avcodec_decode_video2 error:%d,%d", ret, got_picture);
        return -1;
    }

    if (glRender) {
        //交给openGL渲染
        return glRender->render(frame);
    }
    return 0;
}

