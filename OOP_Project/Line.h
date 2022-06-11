#pragma once
#include "constants.h"
#include "Image.h"
#include <cmath>
#include <SDL.h>

class Line
{
    double x, y, z; // 3d center of line
    double X, Y, W; // screen coord
    double scale;   // needed in projection
    double spriteX;
    double clip;
    double slope;
    unsigned long long type;    // type of road (RoadType)
    double roadDegree;
    double roadVelM;
    double criticalVel;

    Image* sprite;
    Image* actsprite;

public:

    Line();
    ~Line();

    //getter
    double getx() const { return x; }
    double gety() const { return y; }
    double getz() const { return z; }
    double getX() const { return X; }
    double getY() const { return Y; }
    double getW() const { return W; }
    double getSlope() const { return slope; }
    unsigned long long getType() const { return type; }
    double getRoadDegree() { return roadDegree; }
    double getRoadVelM() { return roadVelM; }
    double getCritVel() { return criticalVel; }

    //setter
    void setz(double _z) { z = _z; }
    void sety(double _y) { y = _y; }
    void setx(double _x) { x = _x; }
    void addType(RoadType t) { type |= t; }
    void setType(unsigned long long t) { type = t; }
    void setAll(double _x, double _y, double _z, double _s, unsigned long long _t, double rd, double rvm, double max);
    void setSprite(Image* i, double sX) { sprite = i; spriteX = sX; }
    void setClip(double c) { clip = c; }
    void setActSprite(Image* i, double sX) { actsprite = i; spriteX = sX; }
    void setRoadVelM(double rvm) { roadVelM = rvm; }
    void setRoadDegree(double rd) { roadDegree = rd; }

    //projection
    void project(int camX, int camY, int camZ, double camDegree, double camDepth);

    //draw image
    void drawSprite(SDL_Renderer*);
    void drawActSprite(SDL_Renderer*, int);

};

