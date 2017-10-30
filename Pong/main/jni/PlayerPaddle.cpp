
#include "PlayerPaddle.h"

PlayerPaddle::PlayerPaddle(){
}

bool PlayerPaddle::bounceBall(sf::Vector2i pos, int veloX, int size){
    //this code is for the cpu paddle only
    return (pos.x+veloX <= position.x) && (pos.y <= position.y+height) && (pos.y+size >= position.y);
    //return pos.x <= position.x+width && pos.y <= position.y+height && pos.y+size >= position.y;
}