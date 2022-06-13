#include "Obstacle.h"
Obstacle::Obstacle() : BlenderObject("../images/rock/rock.txt", "../images/rock/rock.bmp", ROCK_SIZE, NUM_OBSTACLE)
{
	touchtime = 0;
}
Obstacle::~Obstacle()
{
}

Obstacle::Obstacle(const char* objpath, const char* imgpath) : BlenderObject(objpath, imgpath, ROCK_SIZE, NUM_OBSTACLE)
{
	touchtime = 0;
}
void Obstacle::close() {
	BlenderObject::close();
}
void Obstacle::setObstacle(Line* line, int lineindex, int ind)
{
	objectList[ind].position = { line->getx(),line->gety() + ROCK_SIZE,line->getz(),0,0,0 };
	objectList[ind].index = lineindex;
}
int Obstacle::istouching()
{
	if (SDL_GetTicks64() - touchtime < 250)
		return 1;
	else
	{
		touchtime = SDL_GetTicks64();
		return 0;
	}
}

void Obstacle::draw3D(Point3D pos, double camDeg, double camDepth, Engine* engine, bool& clean, int ind, double maxy) {
	BlenderObject_draw(pos, { 0,0,0 }, camDeg, camDepth, engine, clean, maxy, ind);
	clean = false;
}