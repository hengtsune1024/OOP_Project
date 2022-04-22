#pragma once
#include "Entity.h"



class Trap :
    public Entity
{
	/*
	in entity
	Image* entityImg;
	int numImage;  
	int state;
	*/
	char path[100];
	int x;
	int y;
	int staintime;
	Image stain;

public:
	Trap();
	~Trap();
	Trap(const char* path, int n, SDL_Renderer* renderer);

	void setTrap(Line *line);

	int getWidth();
	int getHeight();
	//void draw(SDL_Renderer* renderer, Line*);
	void gettrap(TrapType);

	void drawStain(SDL_Renderer* renderer);
	
};

