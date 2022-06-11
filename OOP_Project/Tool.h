#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Entity.h"
using namespace std;

class Tool : public Entity
{
	int Tool1;	//speedup
	int Tool2;	//invincible
	int Tool3;	//healing
	int Tool4;	//navigation
	int Tool5;	//lightning
	int Tool6;	//ghost
	int Tool7;	//SWITCH

	int gettime;
	Image tool1img;
	Image tool2img;


public:
	Tool();
	~Tool();
	Tool(const char* path, int n, SDL_Renderer* renderer);

	void setTool(Line* line);

	//void draw(SDL_Renderer* renderer, Line*);
	//void setPos(Point3D p) { pos = p; }
	//Cube* getCube() { return &cube; }
	void getTools();
	int usetool(ToolType);
	void drawmytool(SDL_Renderer* renderer);
	void close();
};