#pragma once
#include "Entity.h"
class Obstacle :
    public Entity
{
	/*
	in entity
	Image* entityImg;
	int numImage;
	int state;
	*/

	int touchtime;

public:
	Obstacle();
	~Obstacle();
	Obstacle(const char* path, int n, SDL_Renderer* renderer);

	void setObstacle(Line* line);
	int istouching();
};

