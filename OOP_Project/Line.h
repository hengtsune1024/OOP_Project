#pragma once
#include "constants.h"
#include <cmath>

class Line
{
    double x, y, z; // 3d center of line
    double X, Y, W; // screen coord
    double curve;   // road turning
    double scale;   // needed in projection

public:

    Line();
    double getx() { return x; }
    double gety() { return y; }
    double getz() { return z; }
    double getX() { return X; }
    double getY() { return Y; }
    double getW() { return W; }
    double getCurve() { return curve; }
    void setz(double _z) { z = _z; }
    void sety(double _y) { y = _y; }
    void setx(double _x) { x = _x; }
    void setCurve(double c) { curve = c; }
    void project(int camX, int camY, int camZ, double camDegree, double camDepth);
    
};

