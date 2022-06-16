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
	int car1tool[6];
	int car2tool[6];
	int gettime;
	bool dual;
	int chosen1;
	int chosen2;

	Image tool1img;
	Image tool2img;
	Image tool3img;
	Image tool4img;
	Image tool5img;
	Image tool6img;

public:
	Tool();
	~Tool();
	Tool(SDL_Renderer* renderer, bool d);

	//setter and getter
	void setTool(Line* line, int lineindex, int ind);
	void getTools(bool car, int ind);
	int getchosentool(bool car) { return car ? chosen1 : chosen2; }
	void setchosentool(bool car);

	int getNearestTool(int startpos);
	//tool function
	int usetool(ToolType, bool car);
	void drawmytool(SDL_Renderer* renderer, bool car);	//car=1 for car1, car=0 for car2
	void getalltools(bool car);

	bool hitTool(double carx, double height, double mod, int ind);

	//virtual override
	void draw3D(Point3D pos, double camDeg, double camDepth, Engine* engine, bool& clean, int ind, double maxy = HEIGHT) override;
	void logic(void* = NULL, void* = NULL) override;

	int checktoolset(bool car);

	void close();
};