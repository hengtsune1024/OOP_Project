#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Object3D.h"
#include "Entity.h"
#include "Cube.h"
using namespace std;

class Tool : public Entity
{
	/*
	in entity
	Image* entityImg;
	int numImage;
	int state;
	*/
	int Tool1;	//speedup
	int Tool2;	//invincible
	//int Tool3;	//

	int gettime;
	Image tool1img;
	Image tool2img;

	Cube cube;

public:
	Tool();
	~Tool();
	Tool(const char* path, int n, SDL_Renderer* renderer);

	void setTool(Line* line);

	//void draw(SDL_Renderer* renderer, Line*);
	void getTools();
	int usetool(ToolType);
	void drawmytool(SDL_Renderer* renderer);
};