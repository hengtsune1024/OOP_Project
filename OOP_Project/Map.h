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
	//projection
	double posX;			// x position in 3D world (moving forward and backword)
	double posY;			// y position in 3D world (moving right and left)
	double velLinear;		// linear velocity 
	double velAngular;		// angular velocity 
	double accLinear;		// linear acceleration 
	double camDegree;		// camera degree (in radius, x-axis is 0)
	double roadDegree;		// the degree from x-axis to the road vector (from current segment to the next segment) 
	double camDepth;

	//road
	vector<Line> lines;						// road segments constituting the whole road
	int number_of_lines;					// the number of road segments
	void drawQuad(SDL_Renderer*, Quad);		// draw a road segment

	//timers
	SDL_TimerID moveTimer;						// car moving timer
	Uint32 moveInterval;						// the interval of moving timer
	static Uint32 move(Uint32, void*);			// the function of car moving

	SDL_TimerID accelerateTimer;				// car accelerating timer	
	Uint32 accelerateInterval;					// the interval of accelerating timer
	static Uint32 accelerate(Uint32, void*);	// the function of car accelerating

	SDL_TimerID chargeTimer;
	Uint32 chargeInterval;
	static Uint32 charge(Uint32, void*);

	void removeTimer();							// remove all timers

	//
	double energy;
	
	//states
	bool isRushing;
	bool fullEnergy;

	//others
	RacingCar car;								// the car in the map
	 
public:

	Map();					// default constructor (initialize nothing)
	Map(SDL_Renderer*);		// initialize all members except timers
	~Map();

	// getters
	double getPosX() { return posX; }
	double getPosY() { return posY; }
	double getVelLinear() { return velLinear; }
	double getVelAngular() { return velAngular; }
	double getAccLinear() { return accLinear; }

	// timer
	void startTimer();

	// setters
	void setVelLinear(double v) { velLinear = v; }
	void setVelAngular(double v) { velAngular = v; }
	void setAccLinear(double a) { accLinear = a; }
	void turn(int);
	  
	void quit();				// quit every member that need to be quit
	void draw(SDL_Renderer*);	// draw the map, car, and tools, etc.
	void rush();
};

// unused code
/*

*/