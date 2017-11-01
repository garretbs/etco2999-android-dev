package com.mygdx.game;

import com.badlogic.gdx.math.*;

public class PongPaddle {

    int height;
    int width;
    int velocity;
    int minY;
    int maxY;

    protected Vector2 position;

    public PongPaddle(){
        position = new Vector2(0, 0);
    }

    void move(boolean below){
        if(below){
            position.y += position.y+height >= maxY ? 0 : velocity;
        }else{
            position.y -= position.y <= minY ? 0 : velocity;
        }
    }

    void goTo(int x, int y){
        position.x = x;
        position.y = y;
    }

    void setSize(int w, int h){
        width = w;
        height = h;
    }

    boolean bounceBall(Vector2 pos, int veloX, int size){
        return (pos.x+size+veloX >= position.x) && (pos.y <= position.y+height) && (pos.y+size >= position.y);
    }

    void setVelocity(int y){
        velocity = y;
    }

    Vector2 getPosition(){
        return position;
    }

    void setYRange(int min, int max){
        minY = min;
        maxY = max;
    }

}
