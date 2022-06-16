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
    double curve;
    unsigned long long type;    // type of road (RoadType)
    double roadDegree;
    double roadVelM;
    double criticalVel;

    Image* sprite;

public:

    Line();
    Line(double xx, double yy, double zz);
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
    double getCurve() { return curve; }
    double getClip() { return clip; }

    //setter
    void setz(double _z) { z = _z; }
    void sety(double _y) { y = _y; }
    void setx(double _x) { x = _x; }
    void addType(RoadType t) { type |= t; }
    void deleteType(RoadType t) { type -= t; }
    void setType(unsigned long long t) { type = t; }
    void setAll(double _x, double _y, double _z, double _s, unsigned long long _t, double rd, double rvm, double max);
    void setSprite(Image* i, double sX) { sprite = i; spriteX = sX; }
    void setClip(double c) { clip = c; }
    void setRoadVelM(double rvm) { roadVelM = rvm; }
    void setRoadDegree(double rd) { roadDegree = rd; }
    void setCritVel(double cv) { criticalVel = cv; }
    void setCurve(double c) { curve = c; }
    void setSlope(double s) { slope = s; }

    //projection
    void project(int camX, int camY, int camZ, double camDegree, double camDepth);

    //draw image
    void drawSprite(SDL_Renderer*);

};

