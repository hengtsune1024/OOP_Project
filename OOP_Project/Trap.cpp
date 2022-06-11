#include "Trap.h"
Trap::Trap(): BlenderObject("../images/trap/trap.txt", "../images/trap/trap.bmp", 100, NUM_TRAP) {

}

Trap::~Trap() {
}

void Trap::close() {
	BlenderObject::close();
	stain.close();
}
Trap::Trap(SDL_Renderer* renderer):
	stain("../images/stain.png", renderer), BlenderObject("../images/trap/trap.txt", "../images/trap/trap.bmp", 500, NUM_TRAP)
{
	car1trap.staintime = car2trap.staintime = SDL_GetTicks64() - STAIN_INTERVAL;
}

bool Trap::hitTrap(double carx, double height, double mod, int ind)
{
	return (carx > objectList[ind].position.x - TRAP_WIDTH * mod && carx < objectList[ind].position.x + TRAP_WIDTH * mod)
		&& (height < objectList[ind].position.y + 500);
}

int Trap::getNearestTrap(int startpos) 
{
	for (int i = 0; i < NUM_TRAP; ++i) {
		if (startpos - objectList[i].index <= 0) {
			if (i == 0)
				return 0;
			else if (objectList[i].index + objectList[i - 1].index < 2 * startpos) {
				return i;
			}
			else
				return i - 1;
		}
	}
	return 0;
}

void Trap::logic()
{
	for (int i = 0; i < NUM_TRAP; ++i) 
	{
		//rotation
		objectList[i].rotation.x += 0.05;
		objectList[i].rotation.y += 0.06;
		objectList[i].rotation.z += 0.055;
		if (objectList[i].rotation.x > PI * 2)
			objectList[i].rotation.x -= PI * 2;
		if (objectList[i].rotation.y > PI * 2)
			objectList[i].rotation.y -= PI * 2;
		if (objectList[i].rotation.z > PI * 2)
			objectList[i].rotation.z -= PI * 2;
	}
}

void Trap::setTrap(Line *line, int lineindex, int ind) 
{
	double shift = ROAD_WIDTH * rand() / (RAND_MAX + 1.0) - ROAD_WIDTH * 0.5;
	objectList[ind].position = { line->getx() + shift, line->gety() + 1500 ,line->getz(),0,0,0 };
	objectList[ind].index = lineindex;
}

void Trap::draw3D(Point3D pos, double camDeg, double camDepth, Engine* engine, bool& clean, int ind, double maxy)
{
	if (objectList[ind].shownflag)
	{
		BlenderObject_draw(pos, objectList[ind].rotation, camDeg, camDepth, engine, clean, maxy, ind);
		clean = false;
	}
}

void Trap::drawStain(SDL_Renderer* renderer, bool car) 
{
	Uint64 time = SDL_GetTicks64();
	if (car) 
	{
		if (time - car1trap.staintime < STAIN_INTERVAL)
		{
			stain.draw(renderer, NULL, NULL);
		}
		//else if (time - car2trap.staintime > STAIN_INTERVAL)	//when stain ends, the trap comes back
		//	shownflag = true;

	}else
	{
		if (time - car2trap.staintime < STAIN_INTERVAL)
		{
			stain.draw(renderer, NULL, NULL);
		}
		//else if (time - car1trap.staintime > STAIN_INTERVAL)
		//	shownflag = true;
	}
}

void Trap::gettrap(TrapType type, bool car, int ind) 
{
	if (car) 
	{
		switch (type)
		{
			case STAIN:
				if (objectList[ind].shownflag == true){
					car1trap.staintime = SDL_GetTicks64();
					objectList[ind].shownflag = false;
				}
				break;
			case DIZZY:

				break;
			case SPEEDDOWN:

				break;
		}
	}
	else 
	{
		switch (type)
		{
			case STAIN:
				if (objectList[ind].shownflag == true) {
					car2trap.staintime = SDL_GetTicks64();
					objectList[ind].shownflag = false;
				}
				break;
			case DIZZY:

				break;
			case SPEEDDOWN:

				break;
		}
	}
}