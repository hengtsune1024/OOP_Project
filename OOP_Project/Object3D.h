#pragma once
#include <SDL.h>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include "Point3D.h"
#include "Triangle.h"

class Object3D
{
	char objFile[20];
	double scale;
	Point3D position;
	Point3D rotation;
	std::vector<Triangle*> triangles;
	Engine engine;
	//Image img;

public:
	Object3D(const char* objectFile, const char* textureFile, double scale);
	~Object3D();
	void Load();
	void Logic(double elapsedTime);
	void draw(SDL_Renderer*);
};
