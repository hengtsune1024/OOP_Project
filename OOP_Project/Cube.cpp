#include "Cube.h"

Cube::Cube(Point3D pos, const char* objFile, const char* texFile) : Object3D(objFile, texFile)
{
	position = pos;
	rotation = { 0,0,0,0,0 };

	// front
	triangles.push_back(new Triangle({ -200, -200, -200, 0, 0 }, { -200, 200, -200, 0, 1 }, { 200, -200, -200, 1, 0 }));
	triangles.push_back(new Triangle({ -200, 200, -200, 0, 1 }, { 200, 200, -200, 1, 1 }, { 200, -200, -200, 1, 0 }));
	// back
	triangles.push_back(new Triangle({ 200, -200, 200, 1, 0 }, { -200, 200, 200, 0, 1 }, { -200, -200, 200, 0, 0 }));
	triangles.push_back(new Triangle({ 200, -200, 200, 1, 0 }, { 200, 200, 200, 1, 1 }, { -200, 200, 200, 0, 1 }));
	// left
	triangles.push_back(new Triangle({ -200, -200, 200, 0, 1 }, { -200, -200, -200, 0, 0 }, { 200, -200, 200, 1, 1 }));
	triangles.push_back(new Triangle({ -200, -200, -200, 0, 0 }, { 200, -200, -200, 1, 0 }, { 200, -200, 200, 1, 1 }));
	// right
	triangles.push_back(new Triangle({ -200, 200, -200, 0, 0 }, { 200, 200, 200, 1, 1 }, { 200, 200, -200, 1, 0 }));
	triangles.push_back(new Triangle({ -200, 200, -200, 0, 0 }, { -200, 200, 200, 0, 1 }, { 200, 200, 200, 1, 1 }));
	// top
	triangles.push_back(new Triangle({ -200, -200, 200, 0, 1 }, { -200, 200, -200, 1, 0 }, { -200, -200, -200, 0, 0 }));
	triangles.push_back(new Triangle({ -200, -200, 200, 0, 1 }, { -200, 200, 200, 1, 1 }, { -200, 200, -200, 1, 0 }));
	// bottom
	triangles.push_back(new Triangle({ 200, -200, -200, 0, 0 }, { 200, 200, -200, 1, 0 }, { 200, -200, 200, 0, 1 }));
	triangles.push_back(new Triangle({ 200, -200, 200, 0, 1 }, { 200, 200, -200, 1, 0 }, { 200, 200, 200, 1, 1 }));

}
Cube::~Cube() 
{}