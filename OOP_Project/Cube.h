#pragma once
#include "Object3D.h"

class Cube: public Object3D
{
public:
	Cube(Point3D pos,const char* objFile, const char* texFile);
	~Cube();
};

