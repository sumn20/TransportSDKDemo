package com.tencent.transportsdkdemo;

import android.content.Intent;
import android.os.Bundle;
import android.text.TextUtils;
import android.widget.Toast;

import com.tencent.transportsdkdemo.databinding.ActivitySimpleEnterBinding;


public class SimpleEnterActivity extends BaseActivity {
    ActivitySimpleEnterBinding binding;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivitySimpleEnterBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        bindToolbarWithBack(binding.toolbar);
        binding.enterRoom.setOnClickListener(view -> {
            String roomID=binding.roomId.getText().toString();
            String userID=binding.userId.getText().toString();
            if (TextUtils.isEmpty(roomID)||TextUtils.isEmpty(userID)){
                Toast.makeText(SimpleEnterActivity.this,"请输入",Toast.LENGTH_SHORT).show();
            }else {
                Intent intent=new Intent(SimpleEnterActivity.this,SimpleTransportActivity.class);
                intent.putExtra("userID",userID);
                intent.putExtra("roomID",Integer.valueOf(roomID));
                startActivity(intent);
            }

        });
    }
}