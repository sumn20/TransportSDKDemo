// Copyright (c) 2021 Tencent. All rights reserved.

#ifndef SDK_SERVER_TRANSPORT_TRTC_CLOUD_DEFINES_H_
#define SDK_SERVER_TRANSPORT_TRTC_CLOUD_DEFINES_H_

#include <stdint.h>

#include <cstddef>

namespace liteav {

#if defined(_WIN32)

#if defined(NOT_BUILD_TRTC_SDK)
#define TRTC_API
#else
#if defined(BUILD_TRTC_SDK)
#define TRTC_API __declspec(dllexport)
#else
#define TRTC_API __declspec(dllimport)
#endif  // defined(BUILD_TRTC_SDK)
#endif

#else  // defined(_WIN32)
#if defined(BUILD_TRTC_SDK)
#define TRTC_API __attribute__((visibility("default")))
#else
#define TRTC_API
#endif
#endif

// 错误码
enum Error {
  // Okay
  ERR_OK = 0,

  // 通用错误，可根据日志文件排查问题
  ERR_FAILED = -1,

  // 业务层逻辑错误：非法调用
  ERR_INVALID_OPERATION = -100,

  // 业务层逻辑错误：非法参数
  ERR_INVALID_PARAMETER = -101,

  // 当前无法读取，请重试
  // 比如：GetAudioFrame() 返回该值时表明当前没有数据可读，请在下一轮重试
  ERR_READ_TRY_AGAIN = -102,

  // 无法连接网络
  ERR_NETWORK_UNREACHABLE = -103,

  // 进房失败
  ERR_ENTER_ROOM_FAILED = -200,

  // 进房参数 user_sig 无效
  ERR_INVALID_USER_SIG = -201,

  // 进房参数 房间号格式错误
  ERR_INVALID_ROOM_ID = -202,

  // 进房参数 sdk_app_id 无效
  ERR_INVALID_SDK_APP_ID = -203,

  // 服务不可用。请检查：套餐包剩余分钟数是否大于0，腾讯云账号是否欠费
  ERR_SERVICE_SUSPENDED = -204,

  // 当前用户被服务器踢出房间
  ERR_BANNED_BY_SERVER = -205,
};

// TRTC 应用场景
enum TRTCScene {
  // 实时视频通话
  TRTC_SCENE_VIDEO_CALL,

  // 实时语音通话
  TRTC_SCENE_AUDIO_CALL,

  // 视频互动直播
  TRTC_SCENE_LIVE,

  // 语音互动直播
  TRTC_SCENE_VOICE_CHAT,

  // 录制模式，用于服务器端音视频录制场景
  TRTC_SCENE_RECORD,
};

// TRTC 用户角色
enum TRTCRole {
  // 主播
  TRTC_ROLE_ANCHOR,
  // 观众
  TRTC_ROLE_AUDIENCE,
};

// 用户信息
struct TRTC_API UserInfo {
  // 用户字符串 ID，由用户在进房时设置
  const char* user_id;
};

// 连接状态
enum ConnectionState {
  // 初始状态
  CONNECTION_STATE_INIT = 0,

  // 连接断开
  CONNECTION_STATE_DISCONNECTED = 1,

  // 正在连接中
  // 网络断开时，SDK 内部会做自动重连
  CONNECTION_STATE_CONNECTING = 2,

  // 已连接上
  CONNECTION_STATE_CONNECTED = 3,
};

// 网络质量
enum NetworkQuality {
  // 未定义
  NETWORK_QUALITY_UNKNOWN = 0,

  // 当前网络非常好
  NETWORK_QUALITY_EXCELLENT = 1,

  // 当前网络比较好
  NETWORK_QUALITY_GOOD = 2,

  // 当前网络一般
  NETWORK_QUALITY_POOR = 3,

  // 当前网络较差
  NETWORK_QUALITY_BAD = 4,

  // 当前网络很差
  NETWORK_QUALITY_VERY_BAD = 5,

  // 当前网络不满足 TRTC 的最低要求
  NETWORK_QUALITY_DOWN = 6,
};

// 全局初始化参数
struct TRTC_API InitParams {
  InitParams()
      : argc(0), argv(NULL), cache_dir(NULL), disable_internal_av_sync(false) {}

  // |argc| & |argv| 为 main() 函数入参
  // 主要用于调试、测试等命令行参数解析。
  int argc;
  const char* const* argv;

  // 文件缓存路径，默认：文件缓存在当前目录下
  // 该路径主要用来存放本地日志文件和进退房 token 缓存。
  const char* cache_dir;

  // 禁用 SDK 内部的 A/V Sync
  // 默认情况下，SDK 内部的音视频 JitterBuffer 会自动进行 A/V 同步
  bool disable_internal_av_sync;
};

// TRTC_SCENE_RECORD 录制模式配置参数
struct TRTC_API RecordConfig {
  RecordConfig()
      : enable_remote_audio_mix(false),
        output_sample_rate(16000),
        output_channels(1),
        output_frame_length_ms(20) {}

  // 是否将远端音频混音后通过 TRTCCloudDelegate::OnRemoteMixedAudioReceived()
  // 输出；
  // 默认是 false，每一路远端音频通过 TRTCCloudDelegate::OnRemoteAudioReceived()
  // 输出
  bool enable_remote_audio_mix;

  // 输出采样率，默认是 16khz
  int output_sample_rate;

  // 输出声道数，默认是单声道
  int output_channels;

  // 输出的音频帧长，要求是 20 的整数倍，默认是 20ms，取值范围 [20, 1000]
  // 若输出的音频是用于语音识别，推荐帧长为 100 ms
  int output_frame_length_ms;
};

// 进房参数
struct TRTC_API EnterRoomParams {
  EnterRoomParams();
  EnterRoomParams(const EnterRoomParams& other);
  ~EnterRoomParams();

  // |sdk_app_id| 为从腾讯云官网申请的 sdkAppId，默认 0
  uint32_t sdk_app_id;

  // |user_id| 用户 ID， 默认 nullptr
  const char* user_id;

  // |user_sig| 用户签名
  // 如何计算UserSig: https://cloud.tencent.com/document/product/647/17275
  // 默认 nullptr
  const char* user_sig;

  // 数字房间号，取值范围：[1, 0xfffffffe]
  // |room_id| 和 |str_room_id| 只能设置其中一个
  uint32_t room_id;

  // 字符串房间号
  // 限制长度最大为64字节。以下为支持的字符集（共 89 个字符）:
  //   -大小写英文字母（a-zA-Z）；
  //   -数字（0-9）；
  //   -空格、"!"、"#"、"$"、"%"、"&"、"("、")"、"+"、"-"、":"、";"、"<"、"="、"."、">"、"?"、
  //    "@"、"["、"]"、"^"、"_"、"{"、"}"、"|"、"~"、","。
  // 各端都必须使用同类型房间号才能实现互通
  // |room_id| 和 |str_room_id| 只需设置其中一个
  // 默认 nullptr
  const char* str_room_id;

  // TRTC 应用场景, 默认 TRTC_SCENE_VIDEO_CALL
  TRTCScene scene;

  // TRTC 用户角色， 默认 TRTC_ROLE_ANCHOR
  TRTCRole role;

  // 房间签名。当您不希望所有用户都能进入房间时，
  // 通过设置房间签名的方式，只允许有相同签名的用户进入房间
  // 详细说明:https://cloud.tencent.com/document/product/647/32240
  // 默认 nullptr
  const char* private_map_key;

  // 录制配置参数，|scene| 为 TRTC_SCENE_RECORD 时有效
  RecordConfig record_config;

  // 指定视频格式 |use_pixel_frame_input| 为 true 则视频发送为 YUV 数据。
  // 发送接口是 SendVideoFrame(StreamType type, const PixelFrame& frame)
  // 默认 false
  bool use_pixel_frame_input;

  // 指定视频格式 |use_pixel_frame_input| 为 true 则视频回调为 YUV 数据。
  // 回调接口是 OnRemoteVideoReceived(const char* user_id,
  //                                   StreamType type,
  //                                   const PixelFrame& frame)
  // 默认 false
  bool use_pixel_frame_output;
};

// 音视频流类型
enum StreamType {
  // 音频流
  STREAM_TYPE_AUDIO = 1,

  // 大流，即高分辨率高码率视频
  STREAM_TYPE_VIDEO_HIGH = 2,

  // 小流，即低分辨率低码率视频
  STREAM_TYPE_VIDEO_LOW = 3,

  // 辅流，主要用于屏幕分享。同一时刻，一个房间内只允许有一个辅流（屏幕分享）。
  STREAM_TYPE_VIDEO_AUX = 7,
};

// 音频 codec 类型
enum AudioCodecType {
  // 音频裸数据 PCM
  AUDIO_CODEC_TYPE_PCM,

  // OPUS 编码
  AUDIO_CODEC_TYPE_OPUS,
};

// 音频帧
class TRTC_API AudioFrame {
 public:
  AudioFrame();
  AudioFrame(const AudioFrame& other);
  AudioFrame& operator=(const AudioFrame& other);
  virtual ~AudioFrame();

  // 设置音频帧数据
  // |data| - 音频数据缓冲起始地址
  // |size| - 音频数据长度，单位 bytes
  // 主要用于 SendAudioFrame(const AudioFrame& frame) 前写入音频数据。
  void SetData(const uint8_t* data, size_t size);

  // 音频数据缓冲起始地址 （只读）
  const uint8_t* data() const;
  // 音频数据长度，单位 bytes
  size_t size() const;

  // 自定义数据缓冲的起始地址（只读）
  const uint8_t* extra_data() const;

  // 自定义数据长度 单位 bytes
  size_t extra_data_size() const;

  // 采样率
  int sample_rate;

  // 声道个数
  int channels;

  // 每个采样的位宽，目前仅支持 16bits
  int bits_per_sample;

  // 音频编码类型
  AudioCodecType codec;

  // PTS，单位：毫秒
  uint32_t pts;

  // SDK internal use only
  const void* handle() const { return handle_; }

 private:
  void* handle_;
};

// 顺时针旋转角度
enum VideoRotation {
  VIDEO_ROTATION_0 = 0,
  VIDEO_ROTATION_90 = 1,
  VIDEO_ROTATION_180 = 2,
  VIDEO_ROTATION_270 = 3,
};

// 视频编码类型
enum VideoCodecType {
  // H264/AVC 编码
  VIDEO_CODEC_TYPE_H264,

  // H265/HEVC 编码
  VIDEO_CODEC_TYPE_H265,
};

// 视频帧
class TRTC_API VideoFrame {
 public:
  VideoFrame();
  VideoFrame(const VideoFrame& other);
  VideoFrame& operator=(const VideoFrame& other);
  virtual ~VideoFrame();

  // 设置视频帧数据
  // |data| - 视频数据缓冲起始地址
  // |size| - 视频数据长度，单位 bytes
  // 主要用于 SendVideoFrame(const VideoFrame& frame) 前写入视频数据。
  void SetData(const uint8_t* data, size_t size);

  // 视频数据缓冲起始地址 (只读)
  const uint8_t* data() const;
  // 视频帧长度，单位 bytes
  size_t size() const;

  // PTS 单位：毫秒
  uint32_t pts;

  // DTS 单位：毫秒
  uint32_t dts;

  // 是否为关键帧
  bool is_key_frame;

  VideoCodecType codec;

  // 视频顺时针旋转角度
  VideoRotation rotation;

  // 参考距离 - 当前帧与它的参考帧之间的距离
  // 要求 `SendVideoFrame(const VideoFrame& frame)` 时需要填写该字段。
  uint8_t reference_interval;

  // SDK internal use only
  const void* handle() const { return handle_; }

 private:
  void* handle_;
};

// YUV类型
enum VideoPixelFormat {
  // yuv420p
  VIDEO_PIXEL_FORMAT_YUV420p,
};

class TRTC_API PixelFrame {
 public:
  PixelFrame();
  PixelFrame(const PixelFrame& other);
  PixelFrame& operator=(const PixelFrame& other);
  virtual ~PixelFrame();

  // 设置视频帧数据
  // |data| - 视频数据缓冲起始地址
  // |size| - 视频数据长度，单位 bytes
  // 主要用于 SendVideoFrame(const PixelFrame& frame) 前写入视频数据。
  void SetData(const uint8_t* data, size_t size);

  // 视频数据缓冲起始地址 (只读)
  const uint8_t* data() const;
  // 视频帧长度，单位 bytes
  size_t size() const;

  // PTS 单位：毫秒
  uint32_t pts;

  // 视频宽度 单位：像素
  uint32_t width;

  // 视频高度 单位：像素
  uint32_t height;

  VideoPixelFormat format;

  // 视频顺时针旋转角度
  VideoRotation rotation;

  // SDK internal use only
  const void* handle() const { return handle_; }

 private:
  void* handle_;
};

// 音频编码参数
struct TRTC_API AudioEncodeParams {
  AudioEncodeParams()
      : channels(1),
        sample_rate(48000),
        frame_length_ms(20),
        bitrate_bps(51200),
        need_encode(true) {}

  // 声道个数
  int channels;

  // 采样率 Hz
  int sample_rate;

  // 帧长 ms
  int frame_length_ms;

  // 码率
  int bitrate_bps;

  // 是否需要 Opus 编码
  // 当发送 PCM 数据时，|need_encode| 为 true，SDK 内部会进行 Opus 编码
  // 当发送 Opus 数据时，需将 |need_encode| 设置为 false，SDK 内部将直接打包发送
  // opus 数据
  bool need_encode;
};

// TRTCCloud 上行统计信息
struct TRTC_API TRTCLocalStatistics {
  TRTCLocalStatistics()
      : video_bitrate_sent(0),
        audio_bitrate_sent(0),
        video_frame_rate_sent(0),
        video_frame_rate_received(0),
        stream_type(STREAM_TYPE_VIDEO_HIGH) {}
  // 视频发送码率 Kbps
  uint32_t video_bitrate_sent;

  // 音频发送码率 Kbps
  uint32_t audio_bitrate_sent;

  // 视频发送帧率 fps
  uint32_t video_frame_rate_sent;
  // 服务器视频接收帧率 fps
  uint32_t video_frame_rate_received;

  // 流类型
  StreamType stream_type;
};

// TRTCCloud 下行统计信息
struct TRTC_API TRTCRemoteStatistics {
  TRTCRemoteStatistics();

  // 远端用户id 默认 nullptr
  const char* user_id;

  // 视频下行丢包率 百分比 默认 0
  uint32_t video_packet_loss;
  // 视频 宽 单位 pixel 默认 0
  uint32_t width;
  // 视频 高 单位 pixel 默认 0
  uint32_t height;
  // 视频帧率 fps 默认 0
  uint32_t video_frame_rate;
  // 视频码率 Kbps 默认 0
  uint32_t video_bitrate;

  // 音频下行丢包率 百分比 默认 0
  uint32_t audio_packet_loss;
  // 音频采样率 Kbps 默认 0
  uint32_t audio_sample_rate;
  // 音频码率 Kbps 默认 0
  uint32_t audio_bitrate;

  // 流类型
  StreamType stream_type;
};

// TRTCCloud统计信息
struct TRTC_API TRTCStatistics {
  TRTCStatistics()
      : up_loss(0),
        down_loss(0),
        rtt(0),
        local_statistic_array(nullptr),
        local_statistic_array_size(0),
        remote_statistic_array(nullptr),
        remote_statistic_array_size(0) {}

  // 上行丢包率，百分比
  uint32_t up_loss;
  // 下行丢包率 百分比
  uint32_t down_loss;
  // 上行 rtt 单位 ms
  uint32_t rtt;

  // 上行统计
  TRTCLocalStatistics* local_statistic_array;
  size_t local_statistic_array_size;

  // 下行统计
  TRTCRemoteStatistics* remote_statistic_array;
  size_t remote_statistic_array_size;
};

}  // namespace liteav

#endif  // SDK_SERVER_TRANSPORT_TRTC_CLOUD_DEFINES_H_
