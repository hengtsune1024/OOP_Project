#pragma once
#include <SDL.h>
#include <cmath>
#include "constants.h"
#include "BlenderObject.h"
#include "RacingCar.h"
#include <vector>
#include "Line.h"
#define ITEM_FRICTION 50

struct Move {
	double moveDegree = 0;
	double moveVel = 0;
	bool isMoving = false;
};

class PhysicalItem: public BlenderObject
{
	vector<Move> move;
	vector<Line>* lines;
	
public:
	PhysicalItem() {}
	PhysicalItem(const char* objfile, const char* texfile, vector<Line>* l, double scale);
	~PhysicalItem();

	void setItem(Line* line, int lineindex, int ind);
	//getter
	double getZ(int ind) { return objectList[ind].position.z; }

	//isCollided
	void collide(RacingCar*);

	//virtual override
	void draw3D(Point3D pos, double camDeg, double camDepth, Engine* engine, bool& clean, int ind, double maxy = HEIGHT) override;
	void logic() override;
	
	void close();
};

