#ifndef SFML_EXAMPLE_PONGPADDLE_H
#define SFML_EXAMPLE_PONGPADDLE_H


#include <SFML/Graphics.hpp>

class PongPaddle {

public:
    sf::RectangleShape sprite;
    int height;
    int width;
    int velocity;
    int minY;
    int maxY;

    void move(bool below);
    void goTo(int x, int y);
    void setSize(int w, int h);
    void setVelocity(int y);
    void setYRange(int min, int max);
    sf::Vector2i getPosition();
    bool bounceBall(sf::Vector2i pos, int veloX, int size);

    PongPaddle();

protected:
    sf::Vector2i position;

};


#endif
