#pragma once
#include "BlenderObject.h"
#include "Line.h"


class Obstacle: public BlenderObject
{
	int touchtime;
	vector<bool> isBroken;

public:
	Obstacle();
	Obstacle(const char* objpath, const char* imgpath);
	~Obstacle();

	//setter
	void setObstacle(Line* line, int lineindex, int ind);

	//state
	int istouching();
	bool hitObstacle(double carx, double height, double mod, int ind);
	int getNearestObstacle(int startpos);
	
	void close();
	void broken(int ind) { isBroken[ind] = true; }
	bool getBroken(int ind) { return isBroken[ind]; }
	

	//virtaul override
	void draw3D(Point3D pos, double camDeg, double camDepth, Engine* engine, bool& clean, int ind, double maxy = HEIGHT) override;
};

