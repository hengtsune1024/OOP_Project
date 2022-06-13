#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Image.h"
#include "Line.h"
#include "BlenderObject.h"
using namespace std;

struct ToolSet {
	int Tool1;	//speedup
	int Tool2;	//invincible
	int Tool3;	//healing
	int Tool4;	//navigation
	int Tool5;	//lightning
	int Tool6;	//ghost
	int Tool7;	//SWITCH
};

class Tool: public BlenderObject
{
	ToolSet car1tool;
	ToolSet car2tool;
	int gettime;

	Image tool1img;
	Image tool2img;
	Image tool3img;
	Image tool4img;
	Image tool5img;
	Image tool6img;
	Image tool7img;
public:
	Tool();
	~Tool();
	Tool(SDL_Renderer* renderer);

	//setter and getter
	void setTool(Line* line, int lineindex, int ind);
	void getTools(bool car, int ind);

	int getNearestTool(int startpos);
	//tool function
	int usetool(ToolType, bool car);
	void drawmytool(SDL_Renderer* renderer, bool car);	//car=1 for car1, car=0 for car2
	void getalltools(bool car);


	//virtual override
	void draw3D(Point3D pos, double camDeg, double camDepth, Engine* engine, bool& clean, int ind, double maxy = HEIGHT) override;
	void logic() override;

	int checktoolset(ToolSet set);

	void close();
};