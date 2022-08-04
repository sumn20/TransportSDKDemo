//
// Created by Sumn on 2022/7/22.
//

#include <iostream>
#include "camera_preview.h"
#include "camera_debug.h"
#include "TRTCCloudCore.h"


CameraPreview::CameraPreview() : window_(nullptr),
                                 rotation_(0),
                                 cameraReady_(false),
                                 camera_(nullptr),
                                 yuvReader_(nullptr),
                                 jpgReader_(nullptr),
                                 doDraw_(false) {

}


CameraPreview::~CameraPreview() {
    cameraReady_ = false;
    DeleteCamera();
}

void CameraPreview::DrawFrame(void) {
    if (!cameraReady_ || !yuvReader_) return;
    AImage *image = yuvReader_->GetNextImage();
    if (!image) {
        return;
    }

    ANativeWindow_acquire(window_);
    ANativeWindow_Buffer buf;
    if (ANativeWindow_lock(window_, &buf, nullptr) < 0) {
        yuvReader_->DeleteImage(image);
        return;
    }

    yuvReader_->DisplayImage(&buf, image);
    ANativeWindow_unlockAndPost(window_);
    ANativeWindow_release(window_);
}

void CameraPreview::CreateCamera(void) {

    rotation_ = 0;

    camera_ = new NDKCamera();
    ASSERT(camera_, "Failed to Create CameraObject");

    int32_t facing = 0, angle = 0, imageRotation = 0;
    if (camera_->GetSensorOrientation(&facing, &angle)) {
        if (facing == ACAMERA_LENS_FACING_FRONT) {
            imageRotation = (angle + rotation_) % 360;
            imageRotation = (360 - imageRotation) % 360;
        } else {
            imageRotation = (angle - rotation_ + 360) % 360;
        }
    }
    LOGI("Phone Rotation: %d, Present Rotation Angle: %d", rotation_,
         imageRotation);
    ImageFormat view{0, 0, 0}, capture{0, 0, 0};
    camera_->MatchCaptureSizeRequest(window_, &view, &capture);

    ASSERT(view.width && view.height, "Could not find supportable resolution");

    // Request the necessary nativeWindow to OS
    bool portraitNativeWindow =
            (savedNativeWinRes_.width < savedNativeWinRes_.height);
    ANativeWindow_setBuffersGeometry(
            window_, portraitNativeWindow ? view.height : view.width,
            portraitNativeWindow ? view.width : view.height, WINDOW_FORMAT_RGBA_8888);

    yuvReader_ = new ImageReader(&view, AIMAGE_FORMAT_YUV_420_888);
    yuvReader_->SetPresentRotation(imageRotation);
    jpgReader_ = new ImageReader(&capture, AIMAGE_FORMAT_JPEG);
    jpgReader_->SetPresentRotation(imageRotation);
    jpgReader_->RegisterCallback(this, [this](void *ctx, const char *str) -> void {
        reinterpret_cast<CameraPreview * >(ctx)->OnPhotoTaken(str);
    });

    // now we could create session
    camera_->CreateSession(yuvReader_->GetNativeWindow(),
                           jpgReader_->GetNativeWindow(), imageRotation);
    camera_->setSateListener(this);
}

void CameraPreview::DeleteCamera(void) {
    if (camera_) {
        delete camera_;
        camera_ = nullptr;
    }
    if (yuvReader_) {
        delete yuvReader_;
        yuvReader_ = nullptr;
    }
    if (jpgReader_) {
        delete jpgReader_;
        jpgReader_ = nullptr;
    }
}

//暂时不需要拍照,空实现此函数
void CameraPreview::OnPhotoTaken(const char *fileName) {
    LOGI("OnPhotoTaken");

}

void CameraPreview::startPreview(ANativeWindow *window) {
    window_ = window;
    CreateCamera();
    ASSERT(camera_, "CameraCreation Failed");
    cameraReady_ = true;
    camera_->StartPreview(true);

}

void CameraPreview::stopPreview() {
    cameraReady_ = false;
    doDraw_ = false;
    DeleteCamera();
}

void CameraPreview::onDraw() {
    while (doDraw_) {
        DrawFrame();
    }
}

void CameraPreview::actionPerformed(CaptureSessionState state) {
    LOGI("actionPerformed state:%d", state);
    if (state == CaptureSessionState::ACTIVE) {
        camera_->UpdateCameraRequestParameter(ACAMERA_SENSOR_EXPOSURE_TIME, 150000000);
        doDraw_ = true;
        TRTCCloudCore::GetInstance()->getTRTCCloud()->CreateLocalVideoChannel(
                StreamType::STREAM_TYPE_VIDEO_HIGH);
        onDraw();
    } else {
        doDraw_ = false;
    }
}

void CameraPreview::setVideoDataListener(VideoDataListener *videoDataListener) {
    if (yuvReader_) {
        yuvReader_->setVideoDataListener(videoDataListener);
    }

}


