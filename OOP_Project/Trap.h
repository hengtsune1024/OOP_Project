#pragma once
#include "Image.h"
#include "constants.h"
#include "BlenderObject.h"
#include "Line.h"
#include <SDL.h>

class Trap
{
	/*
	in entity
	Image* entityImg;
	int numImage;  
	int state;
	*/
	bool shownflag;
	int staintime;
	Image stain;

	bool side; //right=1 left=0
	BlenderObject trap3D;

public:
	Trap();
	~Trap();
	Trap(SDL_Renderer* renderer, bool side);

	void setTrap(Line *line);

	//void draw(SDL_Renderer* renderer, Line*);

	void gettrap(TrapType);
	void drawStain(SDL_Renderer* renderer);

	void close();
	void draw(Point3D pos, double camDeg, double camDepth, Engine* engine, bool& clean, double maxy = HEIGHT);
	void logic();
};

