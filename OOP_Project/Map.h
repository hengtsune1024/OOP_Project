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
	double posX;	//position in map
	double posY;
	double velLinear;	//velocity of driver
	double velAngular;
	double camDegree; 
	double roadDegree;
	int number_of_lines;

	vector<Line> lines;
	void drawQuad(SDL_Renderer*, Quad);
	//void generateTool();

	SDL_TimerID moveTimer;
	Uint32 moveInterval;
	static Uint32 move(Uint32, void*);
	void removeTimer();

	RacingCar* car;

public:
	Map();
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
	void setCar(RacingCar* c) { car = c; }
	 
	void init();
	void quit();
	void draw(SDL_Renderer*);
	
};

// unused code
/*

*/