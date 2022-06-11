#pragma once
#include <SDL.h>
#include <stdio.h>
#include <vector>
#include "Point3D.h"
#include "Triangle.h"
#pragma warning(disable:4996)

struct Location {
	Point3D position = { 0,-CAMERA_HEIGHT,CAMERA_CARMIDPOINT_DIST };
	Point3D rotation = {0,0,0};
	bool shownflag = true;
	int index;
};

class BlenderObject
{
	std::vector<Triangle*> triangles;
	Image3D img;
	void Load(const char* objectFile, double scale);

protected:
	void BlenderObject_draw(Point3D camPos, Point3D worldRot, double camDeg, double camDepth, Engine* engine, bool clean, double maxy, int ind);
	void close();
	double clip;
	std::vector<Location> objectList;

public:
	BlenderObject() {}
	BlenderObject(const char* objectFile, const char* textureFile, double scale, int num);
	virtual ~BlenderObject();

	//getter
	Point3D getRotation(int i) { return objectList[i].rotation; }
	Point3D getPosition(int i) { return objectList[i].position; }
	double getZ(int i) { return objectList[i].position.z; }
	double getRotY(int i) { return objectList[i].rotation.y; }
	int getIndex(int i) { return objectList[i].index; }

	//setter
	void setRotation(Point3D r, int i) { objectList[i].rotation = r; }
	void setPos(Point3D p, int i) { objectList[i].position = p; }
	void setRotY(double yd, int i) { objectList[i].rotation.y = yd; }
	void setIndex(int ind, int i) { objectList[i].index = ind; }
	void setClip(double c) { clip = c; }

	//virtual function
	virtual void logic();
	virtual void draw3D(Point3D campos, double camDeg, double camDepth, Engine* engine, bool& clean, int ind, double maxy = HEIGHT) = 0;
};

