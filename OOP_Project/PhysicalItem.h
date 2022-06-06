#pragma once
#include <SDL.h>
#include <cmath>
#include "constants.h"
#include "BlenderObject.h"
#include "RacingCar.h"
#include <vector>
#include "Line.h"
#define ITEM_FRICTION 50

class PhysicalItem
{
	BlenderObject obj3D;
	double moveDegree;
	double moveVel;
	bool isMoving;
	SDL_TimerID timer;
	vector<Line>* lines;
	static Uint32 move(Uint32, void*);
	void startTimer();
	
public:
	PhysicalItem(const char* objfile, const char* texfile, vector<Line>* l, double scale);
	~PhysicalItem();
	void drawObj3D(Point3D pos, double camDeg, double camDepth, Engine* engine, bool clean, double maxy = HEIGHT);
	void setMove(double mv, double md) { moveDegree = md, moveVel = mv; }
	void setMoving(bool m);
	void setPos(Point3D p) { obj3D.setPos(p); }
	double getZ() { return obj3D.getZ(); }
	void collide(RacingCar*);
};

