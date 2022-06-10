#pragma once
#include "BlenderObject.h"
#include "Line.h"


class Obstacle: public BlenderObject
{
	int touchtime;

public:
	Obstacle();
	Obstacle(const char* objpath, const char* imgpath);
	~Obstacle();

	//setter
	void setObstacle(Line* line);

	//state
	int istouching(); 
	
	void close();

	//virtaul override
	void draw3D(Point3D pos, double camDeg, double camDepth, Engine* engine, bool& clean, double maxy = HEIGHT) override;
};

