
#ifndef SFML_EXAMPLE_PLAYERPADDLE_H
#define SFML_EXAMPLE_PLAYERPADDLE_H


#include "PongPaddle.h"

class PlayerPaddle : public PongPaddle{
public:
    PlayerPaddle();
    bool bounceBall(sf::Vector2i pos, int veloX, int size);
};


#endif
