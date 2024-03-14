#ifndef Arena_h
#define Arena_h

#include <Arduino.h>
#include <list>
#include "ball.h"
class Arena {
private:
    int _width;
    int _height;
    int _maxSpeed;
    int _minSpeed;
    std::list<Ball> _balls;

public:
    Arena(int width, int height, int maxSpeed, int minSpeed);
    void addRandomBall(uint16_t color, int radius);
    void deleteRandomBall();

    void update();

    std::list<Ball> const& balls();

    ~Arena();
};

#endif