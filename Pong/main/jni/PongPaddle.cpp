
#include "PongPaddle.h"

PongPaddle::PongPaddle(){
    position.x, position.y = 0;
    sprite.setPosition(position.x, position.y);
    sprite.setFillColor(sf::Color(255, 255, 255));
    //setSize(1, 1);
}

void PongPaddle::move(bool below){
    if(below){ //press is below paddle
        position.y += position.y+height >= maxY ? 0 : velocity;
    }else{ //press is above paddle
        position.y -= position.y <= minY ? 0 : velocity;
    }
    sprite.setPosition(position.x, position.y);
}
void PongPaddle::goTo(int x, int y){
    position.x = x;
    position.y = y;
    sprite.setPosition(position.x, position.y);
}

void PongPaddle::setSize(int w, int h){
    width = w;
    height = h;
    //sprite.setOrigin(width>>1, height>>1);
    sprite.setSize(sf::Vector2f(width, height-width));
}

bool PongPaddle::bounceBall(sf::Vector2i pos, int veloX, int size){
    //this code is for the cpu paddle only
    return (pos.x+size+veloX >= position.x) && (pos.y <= position.y+height) && (pos.y+size >= position.y);
    //return pos.x+size >= position.x && pos.y <= position.y+height && (pos.y+size) >= position.y;
}

void PongPaddle::setVelocity(int y){
    velocity = y;
}

sf::Vector2i PongPaddle::getPosition(){
    return position;
}

void PongPaddle::setYRange(int min, int max){
    minY = min;
    maxY = max;
}