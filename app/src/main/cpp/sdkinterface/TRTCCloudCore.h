//
// Created by Sumn on 2022/7/5.
//

#ifndef TRANSPORTSDKDEMO_TRTCCLOUDCORE_H
#define TRANSPORTSDKDEMO_TRTCCLOUDCORE_H

#include <VideoDataListener.h>
#include "trtc_cloud.h"
#include "FFmpegVideoDecoder.h"
#include "audio/OpenSLPlayer.h"
#include "camera_preview.h"

using namespace liteav;

class TRTCCloudCore : public TRTCCloudDelegate,VideoDataListener {
private:
    static TRTCCloudCore* m_instance;
    TRTCCloud* pCloud = nullptr;
    FFmpegVideoDecoder* pDecoder;
    OpenSLPlayer* pAudioPlayer;
    CameraPreview* pCameraPreview;

public:
    static TRTCCloudCore* GetInstance();
    static void Destroy();
    TRTCCloudCore();

    ~TRTCCloudCore();
    TRTCCloud*  getTRTCCloud();

    void createDecoder(ANativeWindow *nwin);

    void startPreview(ANativeWindow *window);
    void stopPreview();
    virtual void actionVideoData(VideoFrame frame);
public:
    // SDK 内部不可恢复错误通知，需要 App 干预或提示用户。
    virtual void OnError(Error error);

    // 连接状态通知
    virtual void OnConnectionStateChanged(ConnectionState old_state,
                                          ConnectionState new_state) ;

    // 本地用户进房成功通知
    virtual void OnEnterRoom() ;

    // 本地用户退房成功通知
    virtual void OnExitRoom() ;

    ///////////////////////////////////////////////////////////////////////
    //                   本地音视频发送相关回调                         //
    /////////////////////////////////////////////////////////////////////

    // 本地音频发送通道创建成功，此后可以调用 TRTCCloud::SendAudioFrame()
    // 发送音频数据
    virtual void OnLocalAudioChannelCreated() ;

    // 本地音频发送通道已销毁
    virtual void OnLocalAudioChannelDestroyed() ;

    // 视频流类型为 |type| 的发送通道创建成功，此后可以调用
    // TRTCCloud::SendVideoFrame() 发送视频编码数据
    virtual void OnLocalVideoChannelCreated(StreamType type);

    // 视频流类型为 |type| 的发送通道已销毁
    virtual void OnLocalVideoChannelDestroyed(StreamType type) ;

    // 请求更改视频编码码率
    // SDK 会根据当前网络状况要求视频发送方调整编码码率。
    virtual void OnRequestChangeVideoEncodeBitrate(StreamType type,
                                                   int bitrate_bps) ;

    ///////////////////////////////////////////////////////////////////////
    //                     远端音视频接收相关回调                       //
    /////////////////////////////////////////////////////////////////////

    // 远端用户进房通知
    virtual void OnRemoteUserEnterRoom(const UserInfo& info) ;

    // 远端用户退房通知
    virtual void OnRemoteUserExitRoom(const UserInfo& info) ;

    // 远端用户是否有音频
    // |user_id| - 远端用户 ID
    // |available|：
    //   - true： 远端用户开启音频
    //   - false：远端用户关闭音频
    virtual void OnRemoteAudioAvailable(const char* user_id, bool available) ;

    // 远端用户是否有视频
    // |user_id| - 远端用户 ID
    // |available|：
    //   - true： 远端用户开启视频
    //   - false：远端用户关闭视频
    // |type|：视频流类型
    virtual void OnRemoteVideoAvailable(const char* user_id,
                                        bool available,
                                        StreamType type);

    // 远端视频数据接收回调
    // 默认情况下，SDK 内部已做好音画同步，App 在收到视频帧后可立即解码播放。
    //
    // |user_id| - 视频流所属的远端用户 ID
    // |type| - 视频流类型
    // |frame| - 视频编码数据
    virtual void OnRemoteVideoReceived(const char* user_id,
                                       StreamType type,
                                       const VideoFrame& frame);

    // 远端视频数据接收回调
    // 默认情况下，SDK 内部已做好音画同步，App 在收到视频帧后可立即播放。
    //
    // |user_id| - 视频流所属的远端用户 ID
    // |type| - 视频流类型
    // |frame| - 视频 YUV 数据
    virtual void OnRemoteVideoReceived(const char* user_id,
                                       StreamType type,
                                       const PixelFrame& frame) ;

    // TRTC_SCENE_RECORD 录制场景下，单路远端音频 PCM 数据回调接口
    //
    // |user_id| - 远端用户 ID
    // |frame| - 音频 PCM 数据
    //   帧长度为 20 ms，采样率和声道数由进房参数 RecordConfig 控制
    virtual void OnRemoteAudioReceived(const char* user_id,
                                       const AudioFrame& frame) ;

    // TRTC_SCENE_RECORD 录制场景下，远端音频混流后的 PCM 数据回调接口
    //
    // |frame| - 音频 PCM 数据
    //   帧长度为 20 ms，采样率和声道数由进房参数 RecordConfig 控制
    virtual void OnRemoteMixedAudioReceived(const AudioFrame& frame) ;



};


#endif //TRANSPORTSDKDEMO_TRTCCLOUDCORE_H
