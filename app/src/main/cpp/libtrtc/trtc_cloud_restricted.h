// Copyright (c) 2021 Tencent. All rights reserved.

#ifndef SDK_SERVER_TRANSPORT_TRTC_CLOUD_RESTRICTED_H_
#define SDK_SERVER_TRANSPORT_TRTC_CLOUD_RESTRICTED_H_

#include "trtc_cloud.h"                      // NOLINT(build/include_directory)
#include "trtc_cloud_delegate_restricted.h"  // NOLINT(build/include_directory)

namespace liteav {

// TRTCCloud 受限 API，仅供特定客户使用。
class TRTC_API TRTCCloudRestricted : public TRTCCloud {
 public:
  // 创建一个 TRTCCloudRestricted 实例
  //
  // |delegate| - TRTCCloudDelegateRestricted 委托实现。
  // |use_transport_delegate| - 是否使用传输委托:
  //  - true，|delegate| 需要实现 SendPacket() 接口来接管 SDK 的数据包发送功能，
  //    委托方在收到服务器端下发的数据包后需要调用 DeliverPacket()
  //    将数据包递交给 SDK 处理。
  //  - false， 使用 SDK 内部的 socket 收发实现，它依赖于平台 socket
  //    non-blocking send/recv 实现
  // |intercept_raw_packet| - 是否截获音频和视频原始 UDT 数据包
  //  - true，截获音频和视频原始 UDT 数据包，SDK 内部不再处理接收到的音视频 UDT
  //    数据包。而是通过 |delegate| 的 OnRemoteAudioUDTPacketReceived() 和
  //    OnRemoteVideoUDTPacketReceived() 将 UDT 数据包转交给上层。
  //
  // TRTCCloudRestricted 支持多实例，每个实例的创建和销毁需要在 Initialize() 和
  // Terminate() 之间调用。
  static TRTCCloudRestricted* Create(TRTCCloudDelegateRestricted* delegate,
                                     bool use_transport_delegate,
                                     bool intercept_raw_packet);

  // 应用层在接收到服务器端的数据包后，通过该接口将数据包递交给 TRTC 网络引擎
  //
  // |data| - 数据包缓冲起始地址
  // |size| - 数据包大小
  // |ip| - 数据包来源 IP 地址，地址格式为："192.168.0.1" or "::1"
  // |port| - 数据包来源端口号
  // |protocol| - 指明数据包源自哪种传输通道
  virtual void DeliverPacket(const void* data,
                             size_t size,
                             const char* ip,
                             int port,
                             TransportProtocol protocol) = 0;

 protected:
  ~TRTCCloudRestricted() override {}
};

}  // namespace liteav

#endif  // SDK_SERVER_TRANSPORT_TRTC_CLOUD_RESTRICTED_H_
