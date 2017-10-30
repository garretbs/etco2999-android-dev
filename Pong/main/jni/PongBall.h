#ifndef SFML_EXAMPLE_PONGBALL_H
#define SFML_EXAMPLE_PONGBALL_H
#include <SFML/Graphics.hpp>

class PongBall {

public:
    sf::CircleShape sprite;
    void move();
    void setVelocity(int x, int y);
    void start(int x, int y, int veloX, int veloY);
    bool bounceY(int topY, int bottomY);
    bool scoreX(int leftX, int rightX);
    void goTo(int x, int y);
    sf::Vector2i getPosition();
    sf::Vector2i getVelocity();

    PongBall(int r);

private:
    sf::Vector2i velocity;
    sf::Vector2i position;
    int radius;
};
#endif
