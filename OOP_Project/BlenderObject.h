#pragma once
#include <SDL.h>
#include <stdio.h>
#include <vector>
#include "Point3D.h"
#include "Triangle.h"
#pragma warning(disable:4996)

class BlenderObject
{
	std::vector<Triangle*> triangles;
	Image3D img;
	void Load(const char* objectFile, double scale);

protected:
	Point3D position;
	Point3D rotation;
	bool shownflag;
	void BlenderObject_draw(Point3D camPos, Point3D worldRot, double camDeg, double camDepth, Engine* engine, bool clean, double maxy);
	void close();

public:
	BlenderObject(const char* objectFile, const char* textureFile, double scale, bool shown);
	virtual ~BlenderObject();

	//getter
	Point3D getRotation() { return rotation; }
	Point3D getPosition() { return position; }
	double getZ() { return position.z; }
	double getRotY() { return rotation.y; }

	//setter
	void setRotation(Point3D r) { rotation = r; }
	void setPos(Point3D p) { position = p; }
	void setRotY(double yd) { rotation.y = yd; }

	//virtual function
	virtual void logic();
	virtual void draw3D(Point3D campos, double camDeg, double camDepth, Engine* engine, bool& clean, double maxy = HEIGHT) = 0;
};

