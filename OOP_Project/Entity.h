#pragma once
#include <SDL.h>
#include "Image.h"

class Entity
{
	Image* entityImg;	//image array
	int numImage;	//array size
	int state;		//index of drawing image in array

public:
	Entity();
	virtual ~Entity();

protected:
	void setImage(const char[], int, SDL_Renderer*, int = 0);

	int getState() { return state; }
	Image& getCurrentState() { return entityImg[state]; }
	void setState(int s) { state = s; }

	void closeImg();

	void drawImg(SDL_Renderer*, SDL_Rect, SDL_Rect);
};

