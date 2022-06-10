#pragma once
#include "Image.h"
#include "constants.h"
#include "BlenderObject.h"
#include "Line.h"
#include <SDL.h>

class Trap : public BlenderObject
{
	int staintime;
	Image stain;

	bool side; //right=1 left=0

public:
	Trap();
	~Trap();
	Trap(SDL_Renderer* renderer, bool side);

	//setter and getter
	void setTrap(Line *line);
	void gettrap(TrapType);
	bool getSide() { return side; }

	void drawStain(SDL_Renderer* renderer);

	//virtaul override
	void draw3D(Point3D campos, double camDeg, double camDepth, Engine* engine, bool& clean, double maxy = HEIGHT) override;
	void logic() override;

	void close();
};

