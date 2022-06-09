#pragma once
#include "BlenderObject.h"
#include "Line.h"


class Obstacle: public BlenderObject
{
	
	int touchtime;

public:
	Obstacle();
	~Obstacle();
	Obstacle(const char* objpath, const char* imgpath);

	void setObstacle(Line* line);
	int istouching();
	void setPos(Point3D p) { position = p; }
	void draw3D(Point3D pos, double camDeg, double camDepth, Engine* engine, bool& clean, double maxy = HEIGHT) override;
};

