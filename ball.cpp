#include "ball.h"

void updateBallPos(int dx, int dy, Ball *b) {
    b->x += b->speedX * dx;
    b->y += b->speedY * dy;
}

void nextBallPos(int & x, int & y, int dx, int dy, Ball *b) {
    x = b->x + b->speedX * dx;
    y = b->y + b->speedY * dy;
}