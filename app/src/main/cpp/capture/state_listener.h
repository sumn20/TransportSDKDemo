//
// Created by Sumn on 2022/7/25.
//

#ifndef TRANSPORTSDKDEMO_STATE_LISTENER_H
#define TRANSPORTSDKDEMO_STATE_LISTENER_H

#include "CaptureSessionState.h"



//listener基类(抽象类)
class StateListener
{
public:
    StateListener(){};
    virtual ~StateListener(){};
    virtual void actionPerformed(CaptureSessionState state) = 0;
};
#endif //TRANSPORTSDKDEMO_STATE_LISTENER_H
