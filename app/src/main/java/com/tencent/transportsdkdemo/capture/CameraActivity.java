package com.tencent.transportsdkdemo.capture;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.tencent.transportsdkdemo.BaseActivity;
import com.tencent.transportsdkdemo.R;
import com.tencent.transportsdkdemo.TRTCManager;
import com.tencent.transportsdkdemo.databinding.ActivityCameraBinding;

public class CameraActivity extends BaseActivity {
    ActivityCameraBinding mBinding;
    private SurfaceView mSurfaceView;
    private SurfaceHolder mSurfaceHolder;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mBinding = ActivityCameraBinding.inflate(getLayoutInflater());
        setContentView(mBinding.getRoot());
        bindToolbarWithBack(mBinding.title);
        mSurfaceView = mBinding.surface;
        mSurfaceHolder = mSurfaceView.getHolder();

        mSurfaceHolder.addCallback(new SurfaceHolder.Callback() {

            @Override
            public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

            }

            @Override
            public void surfaceCreated(SurfaceHolder holder) {
                TRTCManager.startPreview(mSurfaceView.getHolder().getSurface());
            }

            @Override
            public void surfaceDestroyed(SurfaceHolder holder) {

            }

        });
        mBinding.click.setOnClickListener(view -> {

            TRTCManager.stopPreview();
        });
        TRTCManager.enterRoom(2008,"334",null);

    }
}