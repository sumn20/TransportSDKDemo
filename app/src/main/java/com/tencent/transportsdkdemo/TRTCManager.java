package com.tencent.transportsdkdemo;

import android.view.Surface;

public class TRTCManager {
    static {
        System.loadLibrary("transportsdkdemo");
    }
    /**
     * 获取sdk版本
     * @return sdk版本
     */
    public static native String getSDKVersion();

    /**
     * 进房
     */
    public static native void enterRoom(int roomID, String userID, Surface surface);
    public static native void exitRoom();



}
