#pragma once
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "Engine.h"
#include "Image3D.h"
#include "Point3D.h"
#include <vector>



struct Color {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

class Triangle
{
	Point3D points[3];
	Point3D worldPoints[3];
	Point3D cameraPoints[3];
	Point3D drawPoints[3];
	Color color;
	double averageZ;
	double normalZ;

public:
	Triangle(Point3D, Point3D, Point3D);
	double getNormalZ() { return normalZ; }
	void draw(Uint32*, Image3D&, double zbuffer[], double maxy = HEIGHT);
	void calculateWorldPoints(const Point3D& rotation, const Point3D& position, Engine* engine,bool);
	void calculateDrawPoints(const Point3D& rotation, const Point3D& position,double camDepth, Engine* engine);
	void calculateCameraPoints(Point3D pos, double camDeg, Engine* engine,bool);
	//static bool compare(Triangle*, Triangle*);
	std::vector<Triangle*> GetClippedTriangles();
	std::vector<Triangle*> GetZClippedTriangles();
};


