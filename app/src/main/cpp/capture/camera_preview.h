//
// Created by Sumn on 2022/7/22.
//

#ifndef TRANSPORTSDKDEMO_CAMERA_PREVIEW_H
#define TRANSPORTSDKDEMO_CAMERA_PREVIEW_H

#include <android/native_window.h>
#include "camera_manager.h"
#include "state_listener.h"
#include "CaptureSessionState.h"

class CameraPreview :public StateListener{
public:
    CameraPreview();

    ~CameraPreview();

    void startPreview(ANativeWindow *window);
    void  onDraw();
    void stopPreview();

    void DrawFrame(void);

    // Manage NDKCamera Object
    void CreateCamera(void);

    void DeleteCamera(void);
    void setVideoDataListener(VideoDataListener *videoDataListener);
private:

    void OnPhotoTaken(const char *fileName);
    virtual void actionPerformed(CaptureSessionState state);
    ANativeWindow *window_;
    ImageFormat savedNativeWinRes_;
    int rotation_;
    volatile bool cameraReady_;
    volatile bool doDraw_;
    NDKCamera *camera_;
    ImageReader *yuvReader_;
    ImageReader *jpgReader_;

};


#endif //TRANSPORTSDKDEMO_CAMERA_PREVIEW_H
