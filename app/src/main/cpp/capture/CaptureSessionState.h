//
// Created by Sumn on 2022/7/25.
//

#ifndef TRANSPORTSDKDEMO_CAPTURESESSIONSTATE_H
#define TRANSPORTSDKDEMO_CAPTURESESSIONSTATE_H
enum class CaptureSessionState : int32_t {
    READY = 0,  // session is ready
    ACTIVE,     // session is busy
    CLOSED,     // session is closed(by itself or a new session evicts)
    MAX_STATE
};
#endif //TRANSPORTSDKDEMO_CAPTURESESSIONSTATE_H
