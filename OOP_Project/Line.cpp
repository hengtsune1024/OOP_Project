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
    
    scale = CAMERA_DEPTH / _z;
    if (scale < 0)
        scale = -scale;
    X = (1 + scale * _x) * WIDTH / 2;
    Y = (1 - scale * (_y - camY)) * HEIGHT / 2;
    W = scale * ROAD_WIDTH * WIDTH / 2;

    /*
    scale = CAMERA_DEPTH / (z - camZ);
    X = (1 + scale * (x - camX)) * WIDTH / 2;
    Y = (1 - scale * (y - camY)) * HEIGHT / 2;
    W = scale * ROAD_WIDTH * WIDTH / 2;*/
    
}