#pragma once
#include "Point3D.h"
#include "Engine.h"
#include "Image3D.h"
#include "constants.h"
#include "Triangle.h"
#include <algorithm>
#include <vector>

class Cube
{
	char objFile[20];
	Point3D position;
	Point3D rotation;
	std::vector<Triangle*> triangles;
	Engine engine;
	Image3D img;

public:
	Cube(const char* objFile, const char* texFile);
	~Cube();
	void setPos(Point3D p) { position = p; }
	double getZ() { return position.z; }
	void draw(SDL_Renderer* renderer, Point3D pos, double camDeg, double camDepth);
};

