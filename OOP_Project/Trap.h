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
	bool side; //right=1 left=0

public:
	Trap();
	~Trap();
	Trap(SDL_Renderer* renderer, bool side);

	//setter and getter
	void setTrap(Line *line, int lineindex, int ind);
	void gettrap(TrapType, bool car, int startpos);
	bool getSide() { return side; }

	void drawStain(SDL_Renderer* renderer,  bool car);

	//virtaul override
	void draw3D(Point3D campos, double camDeg, double camDepth, Engine* engine, bool& clean, int ind, double maxy = HEIGHT) override;
	void logic() override;

	void close();
};

