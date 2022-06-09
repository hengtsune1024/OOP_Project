#include "Trap.h"
Trap::Trap(): BlenderObject("../images/trap/trap.txt", "../images/trap/trap.bmp", 100) {

}

Trap::~Trap() {
}
void Trap::close() {
	stain.close();
}
Trap::Trap(SDL_Renderer* renderer, bool _side): side(_side),
	stain("../images/stain.png", renderer), BlenderObject("../images/trap/trap.txt", "../images/trap/trap.bmp", 500)
{
	staintime = SDL_GetTicks64() - STAIN_INTERVAL;
}

void Trap::logic()
{
	rotation.x += 0.05;
	rotation.y += 0.06;
	rotation.z += 0.055;
	if (rotation.x > PI * 2)
		rotation.x -= PI * 2;
	if (rotation.y > PI * 2)
		rotation.y -= PI * 2;
	if (rotation.z > PI * 2)
		rotation.z -= PI * 2;
}

void Trap::setTrap(Line *line) 
{
	if (side)
		position = { line->getx() + ROAD_WIDTH / 2.0, line->gety() + 1200 ,line->getz(),0,0,0 };
	else
		position =  { line->getx() - ROAD_WIDTH / 2.0, line->gety() + 1200 ,line->getz(),0,0,0 };
	//trap3D.setRotY(atan(((line + 1)->getx() - line->getx()) / SEGMENT_LENGTH));
}

void Trap::draw3D(Point3D pos, double camDeg, double camDepth, Engine* engine, bool& clean, double maxy)
{
	if (shownflag)
	{
		BlenderObject_draw(pos, rotation, camDeg, camDepth, engine, clean, maxy);
		clean = false;
	}
}

void Trap::drawStain(SDL_Renderer* renderer) {
	if (SDL_GetTicks64() - staintime < STAIN_INTERVAL)
	{
		stain.draw(renderer, NULL, NULL);
		shownflag = false;
	}
	else
		shownflag = true;
}

void Trap::gettrap(TrapType type) {
	switch (type)
	{
		case STAIN:
			if (shownflag == true)
				staintime = SDL_GetTicks64();
			break;
		case DIZZY:

			break;
		case SPEEDDOWN:

			break;
	}
}