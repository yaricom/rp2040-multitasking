#ifndef Ball_h
#define Ball_h

#include "Arduino.h"

struct Ball{
    int x;
    int y;
    int radius;
    uint16_t color;
    int speedX;
    int speedY;
};

void updateBallPos(int dx, int dy, Ball * b);
void nextBallPos(int & x, int & y, int dx, int dy, Ball * b);

#endif