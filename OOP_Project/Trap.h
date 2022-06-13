#pragma once
#include "Image.h"
#include "constants.h"
#include "BlenderObject.h"
#include "Line.h"
#include <SDL.h>

struct TrapSet {
	int staintime;
};

class Trap : public BlenderObject
{
	TrapSet car1trap;
	TrapSet car2trap;

	Image stain;

public:
	Trap();
	~Trap();
	Trap(SDL_Renderer* renderer);

	//setter and getter
	void setTrap(Line *line, int lineindex, int ind);
	int gettrap(bool car, int ind);
	//bool getSide(int ind) { return side[ind]; }
	int getNearestTrap(int startpos);

	void drawStain(SDL_Renderer* renderer,  bool car);
	bool hitTrap(double carY, double height, double mod, int ind);

	//virtaul override
	void draw3D(Point3D campos, double camDeg, double camDepth, Engine* engine, bool& clean, int ind, double maxy = HEIGHT) override;
	void logic(void* = NULL, void* = NULL) override;

	void close();
};

