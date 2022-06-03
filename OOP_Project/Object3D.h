#pragma once
#include <SDL.h>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include "Point3D.h"
#include "Triangle.h"
#include "Image3D.h"


class Object3D
{
protected:
	char objFile[20];
	Point3D position;
	Point3D rotation;
	std::vector<Triangle*> triangles;
	Engine engine;
	Image3D img;

public:
	Object3D(const char* objectFile, const char* textureFile);
	virtual ~Object3D();
	virtual void Load();
	void Logic(double elapsedTime);
	void draw(SDL_Renderer*);
};
