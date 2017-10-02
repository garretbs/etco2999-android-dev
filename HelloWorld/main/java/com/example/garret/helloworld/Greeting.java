package com.example.garret.helloworld;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.util.ArrayList;
import java.util.Date;
import java.util.Random;

public class Greeting extends AppCompatActivity {

    final static int buttonA = 0xff000000;
    static int buttonR = 0;
    static int buttonG = 255;
    static int buttonB = 0;

    ListView myListView;
    ArrayList<String> myStringArray1 = new ArrayList<String>();
    ArrayAdapter<String> myArrayAdapter;

    final int port = 1101;
    final String address = "206.21.94.228";

    /*
    Build an app which features dynamic content which updates according to lifecycle callback events.
    Make sure that dynamic content persists when the user navigates away from and back to your app with the onSaveInstanceState and onRestoreInstanceState events.
    I used the following tools for my implementation: LinearLayout, ListView, ListAdapter
    You will earn +10 bonus points on your final lab grade if you use a different scheme...
    Submit your project's "src" directory in a .zip file named using this convention: LastName-FirstName_Lab-##_Month-Day-Year
    The "src" directory of a project called "MyProject" will be located in "MyProject/app/"
    */

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_greeting);

        Button butt = (Button) findViewById(R.id.virusButton);
        changeButtColor(butt);

        /*if(savedInstanceState == null){
            myStringArray1 = savedInstanceState.getStringArrayList("myStringArray2");
        }*/

        myStringArray1 = new ArrayList<>();
        myArrayAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, myStringArray1);
        myListView = (ListView) findViewById(R.id.magicList);
        myListView.setAdapter(myArrayAdapter);

        myArrayAdapter.add("onCreate()");

    }


    public void doClientStuff(){
        new Thread(){
            public void run(){
                try {
                    SocketAddress clientAddress = new InetSocketAddress(address, port);

                    Socket host = new Socket();
                    host.connect(clientAddress, port);

                    String message = "BOING! The time is " + new Date().toString();
                    host.getOutputStream().write(message.getBytes("UTF-8"));
                    host.getOutputStream().write('\n');
                    host.getOutputStream().flush();
                    host.getOutputStream().close();

                    host.close();
                    }catch(IOException e){
                        return;
                    }
            }

        }.start();

    }

    public void doButtonStuff(View v){
        Button butt = (Button) v;
        Random randy = new Random();

        buttonR = randy.nextInt(255);
        buttonG = randy.nextInt(255);
        buttonB = randy.nextInt(255);
        changeButtColor(butt);
        myArrayAdapter.add("doButtonStuff()");

        doClientStuff();

    }

    private void changeButtColor(Button butt){
        butt.setBackgroundColor(buttonA | ((buttonR<<16) | (buttonG<<8) | buttonB));
        butt.setTextColor(buttonA | ((byteRollover(buttonR+128)<<16) | (byteRollover(buttonG+128)<<8) | byteRollover(buttonB+128)));
    }

    private int byteRollover(int input){
        if(input > 255){
            return input-255;
        }else{
            return input;
        }
    }

    @Override
    protected void onResume(){
        super.onResume();
        myArrayAdapter.add("onResume()");
    }

    @Override
    protected void onStart(){
        super.onStart();
        myArrayAdapter.add("onStart()");
    }

    @Override
    protected void onRestart(){
        super.onRestart();
        myArrayAdapter.add("onRestart()");
    }

    @Override
    protected void onPause(){
        super.onPause();
        myArrayAdapter.add("onPause()");
    }

    @Override
    protected void onStop(){
        super.onStop();
        myArrayAdapter.add("onStop()");
    }

    @Override
    protected void onSaveInstanceState(Bundle outState){
        myArrayAdapter.add("onSaveInstanceState()");
        outState.putStringArrayList("myStringArray1", myStringArray1);
        super.onSaveInstanceState(outState);
    }

    @Override
    public void onRestoreInstanceState(Bundle savedInstanceState) {
        super.onRestoreInstanceState(savedInstanceState);
        myStringArray1 = savedInstanceState.getStringArrayList("myStringArray1");
        myArrayAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, myStringArray1);
        myListView.setAdapter(myArrayAdapter);
        myArrayAdapter.add("onRestoreInstanceState()");
    }
}
