#include <android/log.h>

#define LOG_TAGS "CAMERA"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAGS, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAGS, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAGS, __VA_ARGS__)
#define ASSERT(cond, fmt, ...)                                \
  if (!(cond)) {                                              \
    __android_log_assert(#cond, LOG_TAGS, fmt, ##__VA_ARGS__); \
  }
