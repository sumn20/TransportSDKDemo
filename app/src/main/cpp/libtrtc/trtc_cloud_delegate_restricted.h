// Copyright (c) 2021 Tencent. All rights reserved.

#ifndef SDK_SERVER_TRANSPORT_TRTC_CLOUD_DELEGATE_RESTRICTED_H_
#define SDK_SERVER_TRANSPORT_TRTC_CLOUD_DELEGATE_RESTRICTED_H_

#include <cstddef>

#include "trtc_cloud_defines.h"   // NOLINT(build/include_directory)
#include "trtc_cloud_delegate.h"  // NOLINT(build/include_directory)

namespace liteav {

// 传输协议类型
enum TransportProtocol {
  // UDP 传输
  TRANSPORT_PROTOCOL_UDP,

  // TCP 传输
  TRANSPORT_PROTOCOL_TCP,
};

class TRTC_API TRTCCloudDelegateRestricted : public TRTCCloudDelegate {
 public:
  // SDK 请求委托方向指定 IP/Port 服务器地址发送数据包
  // 委托方在收到服务器端下发的数据包后，通过
  // TRTCCloudRestricted::DeliverPacket() 递交给 SDK 处理。
  //
  // |data| - 数据包缓冲起始地址
  // |size| - 数据包大小
  // |ip| - 数据包来源 IP 地址，地址格式为："192.168.0.1" or "::1"
  // |port| - 数据包来源端口号
  // |protocol| - 指明使用哪种传输协议发送该数据包
  virtual void SendPacket(const void* data,
                          size_t size,
                          const char* ip,
                          int port,
                          TransportProtocol protocol) = 0;

  // 收到远端用户的音频原始 UDT 数据包
  // 该方法在 SDK 网络线程中调用，请勿在该方法中执行任何耗时或引起阻塞的操作
  //
  // |user_id| - 远端用户 ID
  // |data| - 音频原始 UDT 数据包缓冲起始地址
  // |size| - 数据包大小
  virtual void OnRemoteAudioUDTPacketReceived(const char* user_id,
                                              const void* data,
                                              size_t size) = 0;

  // 收到远端用户的视频原始 UDT 数据包
  // 该方法在 SDK 网络线程中调用，请勿在该方法中执行任何耗时或引起阻塞的操作
  //
  // |user_id| - 远端用户 ID
  // |type| - 视频流类型
  // |data| - 视频原始 UDT 数据包缓冲起始地址
  // |size| - 数据包大小
  virtual void OnRemoteVideoUDTPacketReceived(const char* user_id,
                                              StreamType type,
                                              const void* data,
                                              size_t size) = 0;

 protected:
  ~TRTCCloudDelegateRestricted() override {}
};

}  // namespace liteav

#endif  // SDK_SERVER_TRANSPORT_TRTC_CLOUD_DELEGATE_RESTRICTED_H_
