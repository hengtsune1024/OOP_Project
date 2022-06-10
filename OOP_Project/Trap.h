#pragma once
#include "Image.h"
#include "constants.h"
#include "BlenderObject.h"
#include "Line.h"
#include <SDL.h>

class Trap : public BlenderObject
{
	/*
	in entity
	Image* entityImg;
	int numImage;  
	int state;
	*/
	int staintime;
	Image stain;

	bool side; //right=1 left=0

public:
	Trap();
	~Trap();
	Trap(SDL_Renderer* renderer, bool side);

	void setTrap(Line *line);

	//void draw(SDL_Renderer* renderer, Line*);

	void gettrap(TrapType);
	void drawStain(SDL_Renderer* renderer);

	void close();
	void draw3D(Point3D campos, double camDeg, double camDepth, Engine* engine, bool& clean, double maxy = HEIGHT) override;
	void logic() override;
	bool getSide() { return side; }
};

