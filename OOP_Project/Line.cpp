#include "Line.h"

Line::Line() :curve(0), x(0), y(0), z(0), type(NORMAL) 
{}

void Line::project(int camX, int camY, int camZ, double camDegree, double camDepth)
{
    double _x = x, _y = y - camY, _z = z - camZ;
    double co = cos(camDegree), si = sin(camDegree);
    _x = co * (x - camX) - si * (z - camZ);
    _z = si * (x - camX) + co * (z - camZ);  

    if (_z < 0) {
        W = 0;
        return;
    }

    scale = camDepth / _z;

    X = (1 + scale * _x) * WIDTH / 2;
    Y = (1 - scale * _y) * HEIGHT / 2;
    W = scale * ROAD_WIDTH * WIDTH / 2;

    //if (W > WIDTH)
    //    W = WIDTH;
    /*
    scale = CAMERA_DEPTH / (z - camZ);
    X = (1 + scale * (x - camX)) * WIDTH / 2;
    Y = (1 - scale * (y - camY)) * HEIGHT / 2;
    W = scale * ROAD_WIDTH * WIDTH / 2;*/
    
}