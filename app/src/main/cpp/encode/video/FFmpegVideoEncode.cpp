//
// Created by Sumn on 2022/7/26.
//


#include <memory>
#include "FFmpegVideoEncode.h"
#include "log_util.h"
#include "trtc_cloud.h"

using namespace liteav;

FFmpegVideoEncode::FFmpegVideoEncode() : video_frame_index(0) {
    //寻找编码器
    pCodec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!pCodec) {
        LOGCATE("pCodec not found.");
    }
    int result = 0;
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if (!pCodecCtx) {
        LOGCATE("pCodecCtx not found.");
    }
    pCodecCtx->width = 1280;
    pCodecCtx->height = 640;
    pCodecCtx->codec_id = AV_CODEC_ID_H264;
    pCodecCtx->time_base = {1, 15};
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    pCodecCtx->thread_count = 4;

    //编码器初始化
    result = avcodec_open2(pCodecCtx, pCodec, NULL);
    if (result != 0) {
        LOGCATE("avcodec_open2 Error:%s", av_err2str(result));
    }
    //初始化并设置AVFrame
    pFrame = av_frame_alloc();
    pFrame->width = pCodecCtx->width;
    pFrame->height = pCodecCtx->height;
    pFrame->format = pCodecCtx->pix_fmt;
    //为frame分配缓冲区
    result = av_frame_get_buffer(pFrame, 0);
    if (result != 0) {
        LOGCATE("av_frame_get_buffer Error:%s", av_err2str(result));
    }
    //初始化AVPacket
    pAvPacket = av_packet_alloc();
    f_out = fopen("/storage/emulated/0/Android/data/com.tencent.transportsdkdemo/cache/test.h264",
                  "wb+");
}

FFmpegVideoEncode::~FFmpegVideoEncode() {
    av_packet_free(&pAvPacket);
    av_frame_free(&pFrame);
    avcodec_free_context(&pCodecCtx);
    fclose(f_out);
    free(f_out);
}

void FFmpegVideoEncode::encode(YuvVideo *yuvVideo) {
    pFrame->data[0] = yuvVideo->y_buffer;
    pFrame->data[1] = yuvVideo->u_buffer;
    pFrame->data[2] = yuvVideo->v_buffer;

    /*  ++video_frame_index;
      pFrame->pts = video_frame_index;*/
    if (!pCodecCtx | !pFrame) {
        LOGCATI("not find codec");
        return;
    }
    //将生成出来的YUV帧发送至编码器
    int sendResult = avcodec_send_frame(pCodecCtx, pFrame);
    if (sendResult < 0) {
        LOGCATE("avcodec_send_frame Error:%s", av_err2str(sendResult));
    }
    //  LOGCATI("sendResult:%d,%d", sendResult, AVERROR(EAGAIN));
    if (sendResult >= 0) {
        //获取转为H264的packet数据
        int receiveResult = avcodec_receive_packet(pCodecCtx, pAvPacket);
        //如果暂无处理完成的packet数据，则继续发送其他YUV帧
        if (receiveResult == AVERROR(EAGAIN) || receiveResult == AVERROR_EOF) {
            //break;
            return;
        }

        std::unique_ptr<VideoFrame> frame =
                std::unique_ptr<VideoFrame>(new VideoFrame());
        frame->SetData(pAvPacket->data, pAvPacket->size);
        frame->codec = VideoCodecType::VIDEO_CODEC_TYPE_H264;
        frame->pts = pAvPacket->pts;
        frame->dts = pAvPacket->dts;
        //判断是否为关键帧
        frame->is_key_frame = pAvPacket->flags == AV_PKT_FLAG_KEY;
        frame->rotation = VideoRotation::VIDEO_ROTATION_0;
        frame->reference_interval = frame->is_key_frame ? 0 : 1;
        //此处是将数据传入到sdk，TRTC暂时不支持
    /*    if (pVideoDataListener) {
            pVideoDataListener->actionVideoData(*frame);
        }*/
         fwrite(pAvPacket->data, 1,pAvPacket->size,f_out);
        //释放packet中的数据
        av_packet_unref(pAvPacket);
    }
}

void FFmpegVideoEncode::setVideoDataListener(VideoDataListener *videoDataListener) {
    pVideoDataListener = videoDataListener;

}
