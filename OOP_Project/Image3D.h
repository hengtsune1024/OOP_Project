#pragma once
#include <SDL.h>
#include <iostream>
#include "constants.h"

class Image3D
{
	int width;
	int height;
	SDL_Texture* texture;
	SDL_Surface* surface;
	Uint32* pixels;

public:
	Image3D();
	~Image3D() {}
	int getWidth() { return width; }
	int getHeight() { return height; }

	void setDimension(int w, int h) { width = w, height = h; }
	void loadSurface(const char* textureFile);	//loadBmp for surface and set pixels, width, height

	Uint32 getColor(int x, int y);

	void close();
};

