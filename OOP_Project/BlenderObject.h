#pragma once
#include <SDL.h>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include "Point3D.h"
#include "Triangle.h"

class BlenderObject
{
	char objFile[20];
	double scale;
	Point3D position;
	Point3D rotation;
	std::vector<Triangle*> triangles;
	Image3D img;

public:
	BlenderObject(const char* objectFile, const char* textureFile, double scale);
	void Load();
	void Logic(double elapsedTime);
	void draw(SDL_Renderer* renderer, Point3D pos, double camDeg, double camDepth, Engine* engine);
};

