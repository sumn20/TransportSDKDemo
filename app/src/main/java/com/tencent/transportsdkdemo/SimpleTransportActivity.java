package com.tencent.transportsdkdemo;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.tencent.transportsdkdemo.databinding.ActivitySimpleTransportBinding;

public class SimpleTransportActivity extends BaseActivity {
    ActivitySimpleTransportBinding binding;
    private SurfaceView mSurfaceView;
    private SurfaceHolder mSurfaceHolder;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivitySimpleTransportBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        bindToolbarWithBack(binding.toolbar);
        String userID = getIntent().getStringExtra("userID");
        int roomID = getIntent().getIntExtra("roomID", 0);
        binding.toolbar.setTitle("房间号:" + roomID);
        mSurfaceView = binding.surface;
        mSurfaceHolder = mSurfaceView.getHolder();

        mSurfaceHolder.addCallback(new SurfaceHolder.Callback() {

            @Override
            public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

            }

            @Override
            public void surfaceCreated(SurfaceHolder holder) {
                TRTCManager.enterRoom(roomID, userID, holder.getSurface());
            }

            @Override
            public void surfaceDestroyed(SurfaceHolder holder) {

            }

        });
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        TRTCManager.exitRoom();
    }
}