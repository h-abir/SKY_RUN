#include "Obstacle.h"
#include <cstdlib>
#include <time.h>

void Obstacle::move() {
    x -= speed;

    if (x + width < 0) {
        x = 1280; // You may need to adjust this value based on your window size
        y= rand() % 960; // You may need to adjust this value based on your window size
    }

}
