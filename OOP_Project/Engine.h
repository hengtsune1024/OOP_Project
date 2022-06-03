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

public:
	Engine();
	~Engine();

	double getZ0() { return Z0; }
	Uint32* getPixels() { return pixels; }
	SDL_Surface* getSurface() { return img; }
	Point3D Translate(const Point3D& original, const Point3D& translation);
	Point3D Rotate(const Point3D& original, Point3D rotation);
	Point3D ApplyPerspective(const Point3D& original);
	Point3D CenterScreen(const Point3D& original);
};
