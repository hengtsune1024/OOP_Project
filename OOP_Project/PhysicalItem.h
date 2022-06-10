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

	//getter
	double getZ() { return position.z; }

	//isCollided
	void collide(RacingCar*);

	//virtual override
	void draw3D(Point3D pos, double camDeg, double camDepth, Engine* engine, bool& clean, double maxy = HEIGHT) override;
	void logic() override;
	
	void close();
};

