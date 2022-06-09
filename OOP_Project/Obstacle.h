#pragma once
#include "BlenderObject.h"
#include "Line.h"


class Obstacle
{
	
	int touchtime;
	BlenderObject obst3D;

public:
	Obstacle();
	~Obstacle();
	Obstacle(const char* objpath, const char* imgpath);

	void setObstacle(Line* line);
	int istouching();
	void setPos(Point3D p) { obst3D.setPos(p); }
	void drawObject3D(Point3D pos, double camDeg, double camDepth, Engine* engine, bool& clean, double maxy = HEIGHT);
};

