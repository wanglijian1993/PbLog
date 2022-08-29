package com.wlj.bplog;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.widget.TextView;

import com.wlj.bplog.databinding.ActivityMainBinding;

import java.io.IOException;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'bplog' library on application startup.

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        try {
            PbLogs.init(5, 1024 * 1024, Environment.getExternalStorageDirectory() + "/log");
        } catch (IOException e) {
            e.printStackTrace();
        }

        binding.sampleText.setOnClickListener(v ->{

            long start = System.currentTimeMillis();
            for (int i = 0; i < 10000; i++) {
                PbLogs.getLogger("live").log(
                        "1111111222222222-" + i + "\n");
            }

            for (int i = 0; i < 10000; i++) {
                PbLogs.getLogger("login").log(
                        "1111111222222222-" + i + "\n");
            }

            long end = System.currentTimeMillis();

            Log.e("TAG", "time -> " + (end - start));
        });
    }


}