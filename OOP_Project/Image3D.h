#pragma once
#include <SDL.h>
#include "constants.h"

class Image3D
{
public:
	int width;
	int height;
	SDL_Texture* texture;
	SDL_Surface* surface;
	Uint32* pixels;
	Uint32 getColor(int x, int y);
};

