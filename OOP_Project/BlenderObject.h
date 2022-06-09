#pragma once
#include <SDL.h>
#include <stdio.h>
#include <vector>
#include "Point3D.h"
#include "Triangle.h"
#pragma warning(disable:4996)

class BlenderObject
{
	double scale;
	std::vector<Triangle*> triangles;
	Image3D img;
	void Load(const char* objectFile);

protected:
	Point3D position;
	Point3D rotation;
	bool shownflag;
	void BlenderObject_draw(Point3D pos, Point3D worldRot, double camDeg, double camDepth, Engine* engine, bool clean, double maxy);

public:
	BlenderObject(const char* objectFile, const char* textureFile, double scale);
	virtual ~BlenderObject();
	//void Logic(double elapsedTime);
			//pos: camera position, rot: how the object needs to be rotated

	Point3D getRotation() { return rotation; }
	Point3D getPosition() { return position; }
	void setRotation(Point3D r) { rotation = r; }
	double getZ() { return position.z; }
	void setPos(Point3D p) { position = p; }
	void setRotY(double yd) { rotation.y = yd; }
	double getRotY() { return rotation.y; }
	virtual void logic();
	virtual void draw3D(Point3D campos, double camDeg, double camDepth, Engine* engine, bool& clean, double maxy = HEIGHT) = 0;
};

