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
    pFrame = av_frame_alloc();
    // 1、找解码器
    pCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!pCodec) {
        LOGCATE("pCodec not found.");
    }
    // 2、初始化parser
    pCodecParserCtx = av_parser_init(pCodec->id);
    if (!pCodecParserCtx) {
        LOGCATE("pCodecParserCtx not found.");
    }
    // 3、分配解码上下文 alloc pCodec context
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if (!pCodecCtx) {
        LOGCATE("could not allocate pCodecCtx.");
    }
    // 4、打开解码器
    int ret = avcodec_open2(pCodecCtx, pCodec, NULL);
    if (ret < 0) {
        LOGCATE("could not open pCodec,ret=%d", ret);
    }
    pGlRender = new OpenGlRender(nwin);

}

FFmpegVideoDecoder::~FFmpegVideoDecoder() {
    LOGCATI("~FFmpegVideoDecoder()");
    av_parser_close(pCodecParserCtx);
    avcodec_free_context(&pCodecCtx);
    av_frame_free(&pFrame);
    av_packet_free(&pkt);
    delete pGlRender;
    pGlRender = NULL;
}


int FFmpegVideoDecoder::decode_video(const uint8_t *data, size_t len, int pts, int dts) {
    //逐帧解析H264,I帧首次解析成功
    int ret = av_parser_parse2(pCodecParserCtx, pCodecCtx, &pkt->data, &pkt->size, data, (int) len, pts,
                               dts, 0);
    if (ret < 0) {
        LOGCATE("av_parser_parse2 error:%d,%d", ret, pkt->size);
        return -1;
    }
/*    LOGCATD("av_parser_parse2 success:%d,size:%d,%dx%d,format:%d", ret, pkt->size,
            pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt);*/
    if (pkt->size == 0) {
        //再次解析,这次是解B和P帧
        ret = av_parser_parse2(pCodecParserCtx, pCodecCtx, &pkt->data, &pkt->size, data, (int) len, pts,
                               dts, 0);
        if (ret < 0) {
            LOGCATE("av_parser_parse2 error:%d,%d", ret, pkt->size);
            return -1;
        }
    }
    int got_picture = 0;
    //获取解码数据,赋值到frame中
    ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, pkt);
    if (ret < 0) {
        LOGCATE("avcodec_decode_video2 error:%d,%d", ret, got_picture);
        return -1;
    }

    if (pGlRender) {
        //交给openGL渲染
        return pGlRender->render(pFrame);
    }
    return 0;
}

