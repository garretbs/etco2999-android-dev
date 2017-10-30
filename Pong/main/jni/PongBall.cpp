#include "PongBall.h"

PongBall::PongBall(int r){
    velocity.x, velocity.y = 0;
    radius = r;
    sprite.setRadius(radius);
    sprite.setFillColor(sf::Color(255, 255, 255));
    //sprite.setOrigin(radius, radius);
}

void PongBall::move(){
    position += velocity;
    sprite.setPosition(position.x, position.y);
}

void PongBall::setVelocity(int x, int y){
    velocity.x = x;
    velocity.y = y;
}

void PongBall::start(int x, int y, int veloX, int veloY){
    position.x = x;
    position.y = y;
    setVelocity(veloX, veloY);
    sprite.setPosition(position.x, position.y);
}

void PongBall::goTo(int x, int y){
    position.x = x;
    position.y = y;
    velocity.x = 0;
    velocity.y = 0;
}

bool PongBall::bounceY(int topY, int bottomY){
    if(position.y <= topY || position.y >= bottomY){
        velocity.y *= -1;
        return true;
    }
    return false;
}

bool PongBall::scoreX(int leftX, int rightX){
    return position.x <= leftX || position.x >= rightX;
}

sf::Vector2i PongBall::getPosition(){
    return position;
}

sf::Vector2i PongBall::getVelocity(){
    return velocity;
}