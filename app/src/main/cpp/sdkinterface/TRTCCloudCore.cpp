//
// Created by Sumn on 2022/7/5.
//

#include "TRTCCloudCore.h"
#include "log_util.h"

TRTCCloudCore *TRTCCloudCore::m_instance = nullptr;

TRTCCloudCore *TRTCCloudCore::GetInstance() {
    if (m_instance == nullptr) {
        m_instance = new TRTCCloudCore();
    }
    return m_instance;
}

void TRTCCloudCore::Destroy() {
    if (m_instance) {
        delete m_instance;
        m_instance = nullptr;
    }
}

TRTCCloudCore::TRTCCloudCore() {
    LOGCATI("TRTCCloudCore()");
    liteav::InitParams init_params;
    TRTCCloud::Initialize(init_params);
    pCloud = TRTCCloud::Create(this);
    pCameraPreview = new CameraPreview();


}

TRTCCloudCore::~TRTCCloudCore() {
    LOGCATI("~TRTCCloudCore()");
    TRTCCloud::Destroy(pCloud);
    pCloud = nullptr;
    delete pDecoder;
    pDecoder = nullptr;
    delete pAudioPlayer;
    pAudioPlayer = nullptr;
    delete pCameraPreview;
    pCameraPreview = nullptr;
}

TRTCCloud *TRTCCloudCore::getTRTCCloud() {
    return pCloud;
}

void TRTCCloudCore::OnError(Error error) {
    LOGCATE("onError errorCode[%d]", error);
}

void TRTCCloudCore::OnConnectionStateChanged(ConnectionState old_state, ConnectionState new_state) {
    LOGCATI("OnConnectionStateChanged old_state[%d] new_state[%d]", old_state, new_state);
}

void TRTCCloudCore::OnEnterRoom() {
    LOGCATI("onEnterRoom");
}

void TRTCCloudCore::OnExitRoom() {
    LOGCATI("OnExitRoom");
}

void TRTCCloudCore::OnLocalAudioChannelCreated() {
    LOGCATI("OnLocalAudioChannelCreated");
}

void TRTCCloudCore::OnLocalAudioChannelDestroyed() {
    LOGCATI("OnLocalAudioChannelDestroyed");
}

void TRTCCloudCore::OnLocalVideoChannelCreated(StreamType type) {
    LOGCATI("OnLocalVideoChannelCreated StreamType[%d]", type);
}

void TRTCCloudCore::OnLocalVideoChannelDestroyed(StreamType type) {
    LOGCATI("OnLocalVideoChannelDestroyed StreamType[%d]", type);
}

void TRTCCloudCore::OnRequestChangeVideoEncodeBitrate(StreamType type, int bitrate_bps) {
    LOGCATI("OnRequestChangeVideoEncodeBitrate StreamType[%d] bitrate_bps[%d]", type, bitrate_bps);
}

void TRTCCloudCore::OnRemoteUserEnterRoom(const UserInfo &info) {
    LOGCATI("OnRemoteUserEnterRoom UserInfo[%s]", info.user_id);
}

void TRTCCloudCore::OnRemoteUserExitRoom(const UserInfo &info) {
    LOGCATI("OnRemoteUserExitRoom UserInfo[%s]", info.user_id);
}

void TRTCCloudCore::OnRemoteAudioAvailable(const char *user_id, bool available) {
    LOGCATI("OnRemoteAudioAvailable user_id[%s] available[%d]", user_id, available);
}

void TRTCCloudCore::OnRemoteVideoAvailable(const char *user_id, bool available, StreamType type) {
    LOGCATI("OnRemoteVideoAvailable user_id[%s] available[%d] StreamType[%d]", user_id, available,
            type);
}

void TRTCCloudCore::OnRemoteVideoReceived(const char *user_id, StreamType type,
                                          const VideoFrame &frame) {
    pDecoder->decode_video(frame.data(), frame.size(), frame.pts, frame.dts);

}

void TRTCCloudCore::OnRemoteVideoReceived(const char *user_id, StreamType type,
                                          const PixelFrame &frame) {

}

void TRTCCloudCore::OnRemoteAudioReceived(const char *user_id, const AudioFrame &frame) {

}

void TRTCCloudCore::OnRemoteMixedAudioReceived(const AudioFrame &frame) {
    pAudioPlayer->Play(frame.data(), frame.size());

}

void TRTCCloudCore::createDecoder(ANativeWindow *nwin) {
    pDecoder = new FFmpegVideoDecoder(nwin);
    pAudioPlayer = new OpenSLPlayer();
}

void TRTCCloudCore::startPreview(ANativeWindow *window) {
    pCameraPreview->startPreview(window);
    pCameraPreview->setVideoDataListener(this);
}

void TRTCCloudCore::stopPreview() {
    pCameraPreview->stopPreview();
}

void TRTCCloudCore::actionVideoData(VideoFrame frame) {
    pCloud->SendVideoFrame(STREAM_TYPE_VIDEO_HIGH, frame);

}

