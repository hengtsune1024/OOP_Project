#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Image.h"
#include "Line.h"
#include "BlenderObject.h"
using namespace std;

class Tool: public BlenderObject
{
	int Tool1;	//speedup
	int Tool2;	//invincible
	//int Tool3;	//

	int gettime;
	Image tool1img;
	Image tool2img;

	bool shownflag;


public:
	Tool();
	~Tool();
	Tool(SDL_Renderer* renderer);

	void setTool(Line* line);
	//void draw(SDL_Renderer* renderer, Line*);
	//void setPos(Point3D p) { pos = p; }
	//Cube* getCube() { return &cube; }
	void getTools();
	int usetool(ToolType);
	void drawmytool(SDL_Renderer* renderer);
	void close();
	void draw3D(Point3D pos, double camDeg, double camDepth, Engine* engine, bool& clean, double maxy = HEIGHT) override;
	void logic() override;
};