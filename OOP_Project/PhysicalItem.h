#pragma once
#include <SDL.h>
#include <cmath>
#include "constants.h"
#include "BlenderObject.h"
#include "RacingCar.h"
#include <vector>
#include "Line.h"
#define ITEM_FRICTION 50

class PhysicalItem: public BlenderObject
{
	double moveDegree;
	double moveVel;
	bool isMoving;
	vector<Line>* lines;
	
public:
	PhysicalItem(const char* objfile, const char* texfile, vector<Line>* l, double scale);
	~PhysicalItem();
	void draw3D(Point3D pos, double camDeg, double camDepth, Engine* engine, bool& clean, double maxy = HEIGHT) override;
	void setMove(double mv, double md) { moveDegree = md, moveVel = mv; }
	void setMoving(bool m);
	void setPos(Point3D p) {position = p; }
	double getZ() { return position.z; }
	void collide(RacingCar*);
	void logic();
};

