#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "constants.h"
#include "Line.h"
#include "RacingCar.h"
#include "BlenderObject.h"
#include "Trap.h"
#include "Tool.h"
#include "Engine.h"
#include "PhysicalItem.h"
#include <vector>
#include <cmath>
#include "SDL_ttf.h"
using namespace std;

struct Quad 
{
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
	//road
	vector<Line> lines;						// road segments constituting the whole road
	int number_of_lines;					// the number of road segments
	void drawQuad(SDL_Renderer*, Quad);		// draw a road segment

	//cars
	bool dualMode;
	RacingCar* car1;
	RacingCar* car2;

	//timers
	SDL_TimerID moveTimer;						// car moving timer
	static Uint32 move(Uint32, void*);			// the function of car moving

	SDL_TimerID accelerateTimer;				// car accelerating timer	
	static Uint32 accelerate(Uint32, void*);	// the function of car accelerating

	SDL_TimerID mapObjectTimer;
	static Uint32 Objectlogic(Uint32, void*);	// the function of car accelerating

	void removeTimer();							// remove all timers

	//images
	Image streetlight;
	Image moon;

	//3D object
	Engine engine;
	PhysicalItem cube;

	Trap virus;
	Tool tools;
	Obstacle rock;


	//static member
	static Uint32 grass;
	static Uint32 rumble;
	static Uint32 road;
	static Uint32 laneLine;
	static SDL_Rect viewPort0;
	static SDL_Rect viewPort1;
	static SDL_Rect viewPort2;
	static unsigned long long type;

	//to end the game
	int endtime;
	EndType endtype;

	//totaltime of winning/victory car
	int record;

public:

	Map(SDL_Renderer*, bool = false);		// initialize all members except timers
	~Map();

	void generateMap();
	void carCollision(RacingCar* car);
	void setEndType(EndType e) { 
		endtype = e;
		endtime = SDL_GetTicks64() + 3000;
	}

	// getters
	RacingCar* getCar1() { return car1; }
	RacingCar* getCar2() { return car2; }
	EndType getendtype() { return endtype; }
	int getendtime() { return endtime; }
	int getrecord() { return record; }
	Tool* getTool() { return &tools; }
	Engine* getEng() { return &engine; }
	vector<Line>* getLines() { return &lines; }
	Obstacle* getObstacle() { return &rock; }

	// timer
	void startTimer();

	//tools
	void usetool(ToolType);

	//draw
	void draw(SDL_Renderer*);	// draw the map, car, and tools, etc.

	//close
	void quit();				// quit every member that need to be quit

	void changecar();

};

// unused code
/*
	
	void setVelLinear(double v) { velLinear = v; }
	void setVelAngular(double v) { velAngular = v; }
	void setAccLinear(double a) { accLinear = a; }
	double getPosX() { return posX; }
	double getPosY() { return posY; }
	double getVelLinear() { return velLinear; }
	double getVelAngular() { return velAngular; }
	double getAccLinear() { return accLinear; }
	
	
	double posX;			// x position in 3D world (moving forward and backword)
	double posY;			// y position in 3D world (moving right and left)
	double velLinear;		// linear velocity 
	double velAngular;		// angular velocity 
	double accLinear;		// linear acceleration 
	double camDegree;		// camera degree (in radius, x-axis is 0)
	double roadDegree;		// the degree from x-axis to the road vector (from current segment to the next segment) 
	double camDepth;
	double velM;
	double camHeight;

	//void turn(int);
	//void rush(RushType);
	

*/