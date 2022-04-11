#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "constants.h"
#include "Line.h"
#include "RacingCar.h"
#include <vector>
#include <cmath>
using namespace std;

class RacingCar;

struct Quad {
	Uint32 color;
	double x1;
	double y1;
	double w1;
	double x2;
	double y2;
	double w2;
};

class Map
{
	double posX;			// x position in 3D world (moving forward and backword)
	double posY;			// y position in 3D world (moving right and left)
	double velLinear;		// linear velocity 
	double velAngular;		// angular velocity 
	double camDegree;		// camera degree (in radius, x-axis is 0)
	double roadDegree;		// the degree from x-axis to the road vector (from current segment to the next segment) 
	int number_of_lines;	// the number of road segments

	vector<Line> lines;						// road segments constituting the whole road
	void drawQuad(SDL_Renderer*, Quad);		// draw a road segment
	//void generateTool();

	SDL_TimerID moveTimer;				// car moving timer
	Uint32 moveInterval;				// the interval of moving timer
	static Uint32 move(Uint32, void*);	// the function of car moving
	void removeTimer();					// remove all timers
	RacingCar car;						// the car
public:

	Map();					// default constructor (initialize nothing)
	Map(SDL_Renderer*);		// initialize all members except timers
	~Map();

	// getters
	double getPosX() { return posX; }
	double getPosY() { return posY; }
	double getVelLinear() { return velLinear; }
	double getVelAngular() { return velAngular; }

	// timer
	void startTimer(Uint32);

	// setters
	void setVelLinear(double v) { velLinear = v; }
	void setVelAngular(double v) { velAngular = v; }
	void turn(int);
	  
	void quit();				// quit every member that need to be quit
	void draw(SDL_Renderer*);	// draw the map, car, and tools, etc.
	
};

// unused code
/*
	void init();
	void setCar(RacingCar* c) { car = c; }
*/