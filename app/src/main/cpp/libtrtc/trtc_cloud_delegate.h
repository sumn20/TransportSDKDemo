// Copyright (c) 2021 Tencent. All rights reserved.

#ifndef SDK_SERVER_TRANSPORT_TRTC_CLOUD_DELEGATE_H_
#define SDK_SERVER_TRANSPORT_TRTC_CLOUD_DELEGATE_H_

#include "trtc_cloud_defines.h"  // NOLINT(build/include_directory)

namespace liteav {

// TRTCCloudDelegate 所有回调方法均在 SDK 内部线程上执行，
// 请勿在回调方法中执行任何可能会引起阻塞的操作。
class TRTC_API TRTCCloudDelegate {
 public:
  // SDK 内部不可恢复错误通知，需要 App 干预或提示用户。
  virtual void OnError(Error error) = 0;

  // 连接状态通知
  virtual void OnConnectionStateChanged(ConnectionState old_state,
                                        ConnectionState new_state) = 0;

  // 本地用户进房成功通知
  virtual void OnEnterRoom() = 0;

  // 本地用户退房成功通知
  virtual void OnExitRoom() = 0;

  ///////////////////////////////////////////////////////////////////////
  //                   本地音视频发送相关回调                         //
  /////////////////////////////////////////////////////////////////////

  // 本地音频发送通道创建成功，此后可以调用 TRTCCloud::SendAudioFrame()
  // 发送音频数据
  virtual void OnLocalAudioChannelCreated() = 0;

  // 本地音频发送通道已销毁
  virtual void OnLocalAudioChannelDestroyed() = 0;

  // 视频流类型为 |type| 的发送通道创建成功，此后可以调用
  // TRTCCloud::SendVideoFrame() 发送视频编码数据
  virtual void OnLocalVideoChannelCreated(StreamType type) = 0;

  // 视频流类型为 |type| 的发送通道已销毁
  virtual void OnLocalVideoChannelDestroyed(StreamType type) = 0;

  // 请求更改视频编码码率
  // SDK 会根据当前网络状况要求视频发送方调整编码码率。
  virtual void OnRequestChangeVideoEncodeBitrate(StreamType type,
                                                 int bitrate_bps) = 0;

  ///////////////////////////////////////////////////////////////////////
  //                     远端音视频接收相关回调                       //
  /////////////////////////////////////////////////////////////////////

  // 远端用户进房通知
  virtual void OnRemoteUserEnterRoom(const UserInfo& info) = 0;

  // 远端用户退房通知
  virtual void OnRemoteUserExitRoom(const UserInfo& info) = 0;

  // 远端用户是否有音频
  // |user_id| - 远端用户 ID
  // |available|：
  //   - true： 远端用户开启音频
  //   - false：远端用户关闭音频
  virtual void OnRemoteAudioAvailable(const char* user_id, bool available) = 0;

  // 远端用户是否有视频
  // |user_id| - 远端用户 ID
  // |available|：
  //   - true： 远端用户开启视频
  //   - false：远端用户关闭视频
  // |type|：视频流类型
  virtual void OnRemoteVideoAvailable(const char* user_id,
                                      bool available,
                                      StreamType type) = 0;

  // 远端视频数据接收回调
  // 默认情况下，SDK 内部已做好音画同步，App 在收到视频帧后可立即解码播放。
  //
  // |user_id| - 视频流所属的远端用户 ID
  // |type| - 视频流类型
  // |frame| - 视频编码数据
  virtual void OnRemoteVideoReceived(const char* user_id,
                                     StreamType type,
                                     const VideoFrame& frame) = 0;

  // 远端视频数据接收回调
  // 默认情况下，SDK 内部已做好音画同步，App 在收到视频帧后可立即播放。
  //
  // |user_id| - 视频流所属的远端用户 ID
  // |type| - 视频流类型
  // |frame| - 视频 YUV 数据
  virtual void OnRemoteVideoReceived(const char* user_id,
                                     StreamType type,
                                     const PixelFrame& frame) = 0;

  // TRTC_SCENE_RECORD 录制场景下，单路远端音频 PCM 数据回调接口
  //
  // |user_id| - 远端用户 ID
  // |frame| - 音频 PCM 数据
  //   帧长度为 20 ms，采样率和声道数由进房参数 RecordConfig 控制
  virtual void OnRemoteAudioReceived(const char* user_id,
                                     const AudioFrame& frame) = 0;

  // TRTC_SCENE_RECORD 录制场景下，远端音频混流后的 PCM 数据回调接口
  //
  // |frame| - 音频 PCM 数据
  //   帧长度为 20 ms，采样率和声道数由进房参数 RecordConfig 控制
  virtual void OnRemoteMixedAudioReceived(const AudioFrame& frame) = 0;

  // 远端 SEI 数据回调接口
  //
  // |user_id| - 远端用户 ID
  // |stream_type| - 流类型
  // |message_type| - 消息类型
  // |message| - SEI 消息体
  // |length| - SEI 消息长度
  virtual void OnSeiMessageReceived(const char* user_id,
                                    StreamType stream_type,
                                    int message_type,
                                    const uint8_t* message,
                                    int length) {}

  // 网络质量回调
  //
  // |user_id| - 空表示本地用户
  // |quality| - 当前用户网络质量
  virtual void OnNetworkQuality(const char* user_id, NetworkQuality quality) {}

  // 状态回调 2s 触发一次
  // 如果在回调中需要对 |stats| 进行异步处理，注意 请深拷贝 |stats|。
  virtual void OnStatsCollected(const TRTCStatistics& stats) {}

 protected:
  virtual ~TRTCCloudDelegate() {}
};

}  // namespace liteav

#endif  // SDK_SERVER_TRANSPORT_TRTC_CLOUD_DELEGATE_H_
