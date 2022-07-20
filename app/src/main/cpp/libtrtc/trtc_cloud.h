// Copyright (c) 2021 Tencent. All rights reserved.

#ifndef SDK_SERVER_TRANSPORT_TRTC_CLOUD_H_
#define SDK_SERVER_TRANSPORT_TRTC_CLOUD_H_

#include "trtc_cloud_defines.h"   // NOLINT(build/include_directory)
#include "trtc_cloud_delegate.h"  // NOLINT(build/include_directory)

namespace liteav {

// TRTCCloud 用于连接腾讯云 TRTC 服务器，提供实时音视频流的发送和接收服务。
// 它支持音频 PCM 数据的发送和接收，以及视频 H264 编码数据的发送和接收。
// - 对于接收到的音视频数据，SDK 内部已做好抖动缓冲处理和音画同步，使用者只需要
//   对音频数据进行播放和视频解码渲染。
// - 对于视频发送，SDK 会根据当前网络状况指导发送方进行码率调控。

class TRTC_API TRTCCloud {
 public:
  // TRTCCloud 全局初始化函数，通常需要在 main() 开始时调用
  // |params| - 初始化参数
  //
  // 返回值：
  // - 0：方法调用成功
  // - <0：方法调用失败，具体错误码参见 Error 定义。
  static int Initialize(const InitParams& params);

  // TRTCCloud 全局销毁函数，通常在程序退出前调用
  // 调用该函数前请确保已调用 Destroy() 销毁所有 TRTCCloud 实例。
  static void Terminate();

  // 创建一个 TRTCCloud 实例
  // |delegate| - TRTCCloudDelegate 委托实现。
  //
  // TRTCCloud 支持多实例，每个实例的创建和销毁需要在 Initialize() 和
  // Terminate() 之间调用。
  static TRTCCloud* Create(TRTCCloudDelegate* delegate);

  // 销毁 TRTCCloud 实例，调用该函数前，请确保已调用 ExitRoom() 退出房间。
  static void Destroy(TRTCCloud* cloud);

  // 进入 TRTC 房间
  // 进房成功会触发 TRTCCloudDelegate::OnEnterRoom() 回调。
  //
  // |param| - 进房参数
  // 返回值：
  // - 0：方法调用成功
  // - <0：方法调用失败，具体错误码参见 Error 定义。
  virtual int EnterRoom(const EnterRoomParams& params) = 0;

  // 退出 TRTC 房间
  // 退出房间成功会触发 TRTCCloudDelegate::OnExitRoom() 回调。
  //
  // 返回值：
  // - 0：方法调用成功
  // - <0：方法调用失败，具体错误码参见 Error 定义。
  virtual int ExitRoom() = 0;

  ///////////////////////////////////////////////////////////////////////////
  //                                                                      //
  // 以下接口必须在 TRTCCloudDelegate::OnEnterRoom() 进房回调触发后再调用 //
  //                                                                      //
  /////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  //                      远端音视频流控制接口                        //
  /////////////////////////////////////////////////////////////////////

  // 订阅指定远端用户的音频或视频流
  //
  // |user_id| - 远端用户 ID
  // |type| - 音频或视频流类型
  //
  // 默认情况下，用户进房后 SDK 会自动订阅所有远端用户的音视频流。
  //
  // 返回值：
  // - 0：方法调用成功
  // - <0：方法调用失败，具体错误码参见 Error。
  virtual int Subscribe(const char* user_id, StreamType type) = 0;

  // 取消订阅指定远端用户的音频或视频流
  // 取消订阅后，服务器将会停止下发该远端用户的音频或视频数据。App 可通过调用
  // Subscribe() 方法重新订阅。
  //
  // |user_id| - 远端用户 ID
  // |type| - 音频或视频流类型
  //
  // 返回值：
  // - 0：方法调用成功
  // - <0：方法调用失败，具体错误码参见 Error 定义。
  virtual int Unsubscribe(const char* user_id, StreamType type) = 0;

  // 读取指定远端用户的音频 PCM 数据，默认每次读取 20ms 数据。
  // 为了最小化音频播放延迟，请通过系统高优先级线程定时调用该接口读取数据进行播放。
  //
  // 注意：该接口仅在非录制模式下生效，即进房参数 |scene| 不等于
  // TRTC_SCENE_RECORD 时。
  //
  // |user_id| - 远端用户 ID
  // |frame| - 音频帧
  //
  // 返回值：
  //  - >0：为成功读取到的字节数
  //  - <0：为读取失败，具体错误码参见 Error 定义
  virtual int GetAudioFrame(const char* user_id, AudioFrame* frame) = 0;

  ///////////////////////////////////////////////////////////////////////
  //                     本地音频发送控制接口                         //
  /////////////////////////////////////////////////////////////////////

  // 创建本地音频发送通道
  // 音频发送通道创建成功后会触发
  // TRTCCloudDelegate::OnLocalAudioChannelCreated()，此时可以调用
  // SendAudioFrame() 发送数据。
  //
  // |params| - 音频编码参数
  // 若需要发送 opus 编码数据，需将 |params.need_encode| 设置为 false。
  //
  // 返回值：
  // - 0：方法调用成功
  // - <0：方法调用失败，具体错误码参见 Error 定义。
  virtual int CreateLocalAudioChannel(const AudioEncodeParams& params) = 0;

  // 在 TRTCCloudDelegate::OnLocalAudioChannelCreated()
  // 回调触发后开始调用该接口发送音频数据包。
  //
  // - 若 CreateLocalAudioChannel() 时传入的 AudioEncodeParams::need_encode 为
  //   true 时，发送 PCM 数据包。建议每次发送 20MS 长度的 PCM 数据包。
  // - 若 CreateLocalAudioChannel() 时传入的 AudioEncodeParams::need_encode 为
  //   false 时，发送 Opus 编码数据。
  //
  // 返回值：
  // - 0：方法调用成功
  // - <0：方法调用失败，具体错误码参见 Error 定义。
  virtual int SendAudioFrame(const AudioFrame& frame) = 0;

  // 销毁本地音频发送通道
  // 销毁成功后会触发 TRTCCloudDelegate::OnLocalAudioChannelDestroyed() 回调
  //
  // 返回值：
  // - 0：方法调用成功
  // - <0：方法调用失败，具体错误码参见 Error 定义。
  virtual int DestroyLocalAudioChannel() = 0;

  ///////////////////////////////////////////////////////////////////////
  //                     本地视频发送控制接口                         //
  /////////////////////////////////////////////////////////////////////

  // 创建本地视频发送通道
  // |type|:
  // - STREAM_TYPE_VIDEO_HIGH：大流 - 高分辨率高码率视频流
  // - STREAM_TYPE_VIDEO_LOW：小流 - 低分辨率低码率视频流
  // - STREAM_TYPE_VIDEO_AUX：辅流 - 通常用于屏幕分享，一个房间内只能有一个辅流
  //
  // 视频发送通道创建成功后会触发
  // TRTCCloudDelegate::OnLocalVideoChannelCreated() 回调。 此时可以调用
  // SendVideoFrame() 发送视频编码数据。
  //
  // 返回值：
  // - 0：方法调用成功
  // - <0：方法调用失败，具体错误码参见 Error 定义。
  virtual int CreateLocalVideoChannel(StreamType type) = 0;

  // 发送视频编码数据
  // 在 TRTCCloudDelegate::OnVideoChannelCreated()
  // 回调成功后调用该接口进行发送视频编码数据
  //
  // |type| - 在指定视频流发送通道上发送。
  //
  // 返回值：
  // - 0：方法调用成功
  // - <0：方法调用失败，具体错误码参见 Error 定义。
  virtual int SendVideoFrame(StreamType type, const VideoFrame& frame) = 0;

  // 发送视频 YUV 数据
  // 在 TRTCCloudDelegate::OnVideoChannelCreated()
  // 回调成功后调用该接口进行发送视频 YUV 数据
  //
  // |type| - 在指定视频流发送通道上发送。
  //
  // 返回值：
  // - 0：方法调用成功
  // - <0：方法调用失败，具体错误码参见 Error 定义。
  virtual int SendVideoFrame(StreamType type, const PixelFrame& frame) = 0;

  // 发送视频 SEI 数据
  // 在 TRTCCloudDelegate::OnVideoChannelCreated()
  // 回调成功后调用该接口进行发送视频 SEI 数据
  //
  // |message_type| - 可取值 5 和 242， 推荐 242，和其它端取值要相同才可以互通。
  // |message| - 消息体
  // |length| - 消息字节长度
  //
  // 返回值：
  // - 0: 方法调用成功
  // - <0: 方法调用失败，具体错误码参见 Error 定义。
  //
  // 注意（任何一条不满足，返回失败）：
  // - 1: 单条消息数据量限制，不超过 1000 字节
  // - 2: 发送频率限制，每秒不超过 30 条
  // - 3: 发送码率限制，每秒不超过 8000 字节
  virtual int SendSeiMessage(int message_type,
                             const uint8_t* message,
                             int length) = 0;

  // 销毁本地视频发送通道
  // 成功销毁后会触发 TRTCCloudDelegate::OnLocalVideoChannelDestroyed() 回调。
  // |type| 为指定的视频流通道类型
  //
  // 返回值：
  // - 0：方法调用成功
  // - <0：方法调用失败，具体错误码参见 Error 定义。
  virtual int DestroyLocalVideoChannel(StreamType type) = 0;

 protected:
  virtual ~TRTCCloud() {}
};

}  // namespace liteav

#endif  // SDK_SERVER_TRANSPORT_TRTC_CLOUD_H_
