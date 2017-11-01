package com.mygdx.game;


import com.badlogic.gdx.math.*;

public class PongBall {

    private Vector2 velocity;
    private Vector2 position;
    int radius;

    public PongBall(int r){
        velocity = new Vector2(0, 0);
        position = new Vector2(0, 0);
        radius = r;
    }

    void move(){
        position.add(velocity);
    }

    void setVelocity(int x, int y){
        velocity.x = x;
        velocity.y = y;
    }

    void start(int x, int y, int veloX, int veloY){
        position.x = x;
        position.y = y;
        setVelocity(veloX, veloY);
    }

    void goTo(int x, int y){
        position.x = x;
        position.y = y;
        setVelocity(0, 0);
    }

    boolean bounceY(int topY, int bottomY){
        if(position.y <= topY || position.y >= bottomY){
            velocity.y *= -1;
            return true;
        }
        return false;
    }

    boolean scoreX(int leftX, int rightX){
        return position.x <= leftX || position.x >= rightX;
    }

    Vector2 getPosition(){
        return position;
    }

    Vector2 getVelocity(){
        return velocity;
    }


}
