package com.example.garret.helloworld;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import java.util.Random;

public class Greeting extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_greeting);
    }
    public void changeButtonColor(View v){
        Button butt = (Button) v;
        Random randy = new Random();

        final int alpha = 0xff000000;
        int r = randy.nextInt(255);
        int g = randy.nextInt(255);
        int b = randy.nextInt(255);

        butt.setBackgroundColor(alpha | ((r<<16) | (b<<8) | g));
        butt.setTextColor(alpha | ((byteRollover(r+128)<<16) | (byteRollover(b+128)<<8) | byteRollover(g+128)));
    }

    private int byteRollover(int input){
        if(input > 255){
            return input-255;
        }else{
            return input;
        }
    }
}
