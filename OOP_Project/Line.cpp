#include "Line.h"

Line::Line(){
    spriteX = curve = x = y = z = 0;
}

void Line::project(int camX, int camY, int camZ, double camD)
{
    double _x = x, _y = y, _z = z;
    double co = cos(camD), si = sin(camD);
    _x = co * (x - camX) - si * (z - camZ);
    _z = si * (x - camX) + co * (z - camZ);

    if (_z < 0) {
        W = 0;
        return;
    }

    scale = CAMERA_DEPTH / _z;

    X = (1 + scale * _x) * WIDTH / 2;
    Y = (1 - scale * (_y - camY)) * HEIGHT / 2;
    W = scale * ROAD_WIDTH * WIDTH / 2;

    //if (W > WIDTH)
    //    W = WIDTH;
    /*
    scale = CAMERA_DEPTH / (z - camZ);
    X = (1 + scale * (x - camX)) * WIDTH / 2;
    Y = (1 - scale * (y - camY)) * HEIGHT / 2;
    W = scale * ROAD_WIDTH * WIDTH / 2;*/
    
}