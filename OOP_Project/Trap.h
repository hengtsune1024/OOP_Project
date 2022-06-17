#pragma once
#include "Image.h"
#include "constants.h"
#include "BlenderObject.h"
#include "Line.h"
#include <SDL.h>

class Trap : public BlenderObject
{
	int car1stain;
	int car2stain;
	Image stain;

public:
	Trap();
	~Trap();
	Trap(SDL_Renderer* renderer);

	//setter and getter
	void setTrap(Line *line, int lineindex, int ind);
	int gettrap(bool car, bool invincible, int ind);
	friend int getNearestTrap(Trap& trap, int startpos);

	void drawStain(SDL_Renderer* renderer,  bool car);
	bool hitTrap(double carY, double height, double mod, int ind);

	//virtaul override
	void draw3D(Point3D campos, double camDeg, double camDepth, Engine* engine, bool& clean, int ind, double maxy = HEIGHT) override;
	void logic(void* = NULL, void* = NULL) override;

	void close();
};

