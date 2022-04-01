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
	int posX;	//position in map
	int posY;
	int velX;	//velocity for driver
	double velY;
	double camDegree;
	int N;

	vector<Line> lines;
	void drawQuad(SDL_Renderer*, Quad);
	void generateTool();

	SDL_TimerID moveTimer;
	Uint32 moveInterval;
	static Uint32 move(Uint32, void*);
	void removeTimer();

	RacingCar* car;

public:
	Map();
	~Map();

	int getPosX() { return posX; }
	int getVelX() { return velX; }
	int getPosY() { return posY; }
	double getVelY() { return velY; }

	void startTimer(Uint32);
	void setVelX(int v) { velX = v; }
	void setVelY(double v) { velY = v; }
	void setCar(RacingCar* c) { car = c; }

	void init();
	void quit();
	void draw(SDL_Renderer*);
	
};