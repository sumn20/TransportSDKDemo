//
// Created by Sumn on 2022/8/4.
//

#ifndef TRANSPORTSDKDEMO_YUVDUMP_H
#define TRANSPORTSDKDEMO_YUVDUMP_H
extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavutil/frame.h"
}
/**
* 存放YUV数据
*/
typedef struct YuvData{
    uint8_t *y_buffer;
    uint8_t *u_buffer;
    uint8_t *v_buffer;
} YuvVideo;

class YUVDump {
public:
    YUVDump();
    ~YUVDump();
    int dumpYUV2AVFrame(AVFrame *frame);
    int dumpYUV2Buffer(YuvData *yuvData);

private:
    FILE *pYuvOut = NULL;

};


#endif //TRANSPORTSDKDEMO_YUVDUMP_H
