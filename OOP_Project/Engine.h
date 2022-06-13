#pragma once
#include "Point3D.h"
#include "constants.h"
#include <cmath>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class Engine
{
	double Z0;
	Uint32* pixels;
	SDL_Surface* img;
	static double zBuffer[WIDTH * HEIGHT];

public:
	Engine();
	~Engine();

	//getter
	double getZ0() { return Z0; }
	Uint32* getPixels() { return pixels; }
	SDL_Surface* getSurface() { return img; }
	double* getZBuffer() { return zBuffer; }

	//projection
	Point3D Translate(const Point3D& original, const Point3D& translation);
	Point3D Rotate(const Point3D& original, Point3D rotation);
	Point3D ApplyPerspective(const Point3D& original, double camDepth);
	Point3D CenterScreen(const Point3D& original);

	//render pixels to the window
	void drawAll(SDL_Renderer* renderer);
};
