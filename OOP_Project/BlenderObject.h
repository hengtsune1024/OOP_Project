#pragma once
#include <SDL.h>
#include <string.h>
#include <stdio.h>
#include <vector>
#include "Point3D.h"
#include "Triangle.h"

class BlenderObject
{
	char objFile[30];
	double scale;
	Point3D position;
	Point3D rotation;
	std::vector<Triangle*> triangles;
	Image3D img;

	void Load(const char* objectFile);

public:
	BlenderObject(const char* objectFile, const char* textureFile, double scale);
	~BlenderObject();
	//void Logic(double elapsedTime);
	void draw(Point3D pos, Point3D rot, double camDeg, double camDepth, Engine* engine, bool clean, double maxy = HEIGHT);	
			//pos: camera position, rot: how the object needs to be rotated

	Point3D getRotation() { return rotation; }
	void setRotation(Point3D r) { rotation = r; }
	double getZ() { return position.z; }
	void setPos(Point3D p) { position = p; }
};

