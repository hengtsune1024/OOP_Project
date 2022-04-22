#pragma once
#include "constants.h"
#include "Image.h"
#include <cmath>
#include <SDL.h>

class Line
{
    double x, y, z; // 3d center of line
    double X, Y, W; // screen coord
    double curve;   // road turning
    double scale;   // needed in projection
    double spriteX;
    double clip;
    RoadType type;    
    Image* sprite;
    Image* actsprite;

public:

    Line();
    double getx() { return x; }
    double gety() { return y; }
    double getz() { return z; }
    double getX() { return X; }
    double getY() { return Y; }
    double getW() { return W; }
    double getCurve() { return curve; }
    RoadType getType() { return type; }

    void setz(double _z) { z = _z; }
    void sety(double _y) { y = _y; }
    void setx(double _x) { x = _x; }
    void setCurve(double c) { curve = c; }
    void setType(RoadType t) { type = t; }
    void setAll(double _x, double _y, double _z, double _c) { z = _z; x = _x; y = _y; curve = _c; }
    void setSprite(Image* i, double sX) { sprite = i; spriteX = sX; }
    void setClip(double c) { clip = c; }

    void project(int camX, int camY, int camZ, double camDegree, double camDepth,double);
    void drawSprite(SDL_Renderer*);
    
    void setActSprite(Image* i, double sX) { actsprite = i; spriteX = sX; }
    void drawActSprite(SDL_Renderer*, int);

};

