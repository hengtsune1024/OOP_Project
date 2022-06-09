#include "Trap.h"
Trap::Trap(): trap3D("../images/trap/trap.txt", "../images/trap/trap.bmp", 100) {

}

Trap::~Trap() {
}
void Trap::close() {
	stain.close();
}
Trap::Trap(SDL_Renderer* renderer, bool _side): side(_side), shownflag(true),
	stain("../images/stain.png", renderer), trap3D("../images/trap/trap.txt", "../images/trap/trap.bmp", 500)
{
	staintime = SDL_GetTicks64() - STAIN_INTERVAL;
}

void Trap::logic()
{
	Point3D rot = trap3D.getRotation();
	rot.x += 0.05;
	rot.y += 0.06;
	rot.z += 0.055;
	if (rot.x > PI * 2)
		rot.x -= PI * 2;
	if (rot.y > PI * 2)
		rot.y -= PI * 2;
	if (rot.z > PI * 2)
		rot.z -= PI * 2;
	trap3D.setRotation(rot);
}

void Trap::setTrap(Line *line) 
{
	if (side)
		trap3D.setPos({ line->getx(), line->gety() + 1200 ,line->getz(),0,0,0 });
	else
		trap3D.setPos({ line->getx() - ROAD_WIDTH / 2.0, line->gety() + 1200 ,line->getz(),0,0,0 });
	//trap3D.setRotY(atan(((line + 1)->getx() - line->getx()) / SEGMENT_LENGTH));
}

void Trap::draw(Point3D pos, double camDeg, double camDepth, Engine* engine, bool& clean, double maxy)
{
	if (shownflag){

		trap3D.draw(pos, trap3D.getRotation(), camDeg, camDepth, engine, clean, maxy);
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