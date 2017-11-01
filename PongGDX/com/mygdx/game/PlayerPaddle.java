package com.mygdx.game;

import com.badlogic.gdx.math.*;

public class PlayerPaddle extends PongPaddle {

    public PlayerPaddle(){
        super();
    }

    boolean bounceBall(Vector2 pos, int veloX, int size){
        return (pos.x+veloX <= position.x) && (pos.y <= position.y+height) && (pos.y+size >= position.y);
    }
}