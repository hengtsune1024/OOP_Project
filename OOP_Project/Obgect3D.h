#pragma once
#include <SDL.h>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include "Point3D.h"
#include "Triangle.h"

class Obgect3D
{
	char objFile[20];
	double scale;
	Point3D position;
	Point3D rotation;
	std::vector<Triangle*> triangles;
	Engine engine;
	//Image img;

public:
	Obgect3D(const char* objectFile, const char* textureFile, double scale);
	void Load();
	void Logic(double elapsedTime);
	void draw(SDL_Renderer*);
};
