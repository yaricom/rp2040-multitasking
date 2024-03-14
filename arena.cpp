#include "arena.h"

Arena::Arena(int width, int height, int maxSpeed, int minSpeed) {
    _width = width;
    _height = height;
    _maxSpeed = maxSpeed;
    _minSpeed = minSpeed;
}

Arena::~Arena() {
    _balls.clear();
}

void Arena::addRandomBall(uint16_t color, int radius) {
    int size = radius * 2;
    int x = random(_width - size) + radius;
    int y = random(_height - size) + radius;
    int speedX = (int)random(_minSpeed, _maxSpeed);
    if (random(10) > 5) {
      speedX *= -1;
    }
    int speedY = (int)random(_minSpeed, _maxSpeed);
    if (random(10) > 5) {
      speedY *= -1;
    }

    Ball ball = {.x=x, .y=y, .radius=radius, .color=color, .speedX=speedX, .speedY=speedY};

    _balls.push_back(ball);
};

void Arena::deleteRandomBall() {
    int ballsCount = _balls.size();
    if (ballsCount == 1) {
      // left one ball
      return;
    }
    int index = random(ballsCount);

    auto l_front = _balls.begin();
    std::advance(l_front, index);

    _balls.erase(l_front);
}

void Arena::update() {
    int dx = 1, dy = 1;
    int x = 0, y = 0;
    for (Ball & b : _balls) {
        nextBallPos(x, y, dx, dy, &b);
        if (x - b.radius < 0 || x + b.radius > _width) {
            b.speedX *= -1;
        }
        if (y - b.radius < 0 || y + b.radius > _height) {
            b.speedY *= -1;
        }
        updateBallPos(dx, dy, &b);
    }
}

std::list<Ball> const& Arena::balls() {
    return _balls;
}