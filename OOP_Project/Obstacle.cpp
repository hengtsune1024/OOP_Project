#include "Obstacle.h"
Obstacle::Obstacle() : BlenderObject("../images/rock/rock.txt", "../images/rock/rock.bmp", ROCK_SIZE, NUM_OBSTACLE,1)
{
	touchtime = 0;
}
Obstacle::~Obstacle()
{
}

Obstacle::Obstacle(const char* objpath, const char* imgpath) : BlenderObject(objpath, imgpath, ROCK_SIZE, NUM_OBSTACLE, 1), isBroken(NUM_OBSTACLE, false)
{
	touchtime = 0;
}

bool Obstacle::hitObstacle(double carx, double height, int ind)
{
	return objectList[ind].shownflag && (carx > objectList[ind].position.x - OBSTACLE_WIDTH && carx < objectList[ind].position.x + OBSTACLE_WIDTH)
		&& (height < objectList[ind].position.y + ROCK_SIZE);
}

void Obstacle::close() {
	BlenderObject::close();
}

int Obstacle::getNearestObstacle(int startpos)
{
	for (int i = 0; i < NUM_OBSTACLE; ++i) 
	{
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

void Obstacle::setObstacle(Line* line, int lineindex, int ind)
{
	double shift = (ROAD_WIDTH * 0.9 * 2) * rand() / (RAND_MAX + 1.0) - ROAD_WIDTH * 0.9;
	objectList[ind].position = { line->getx() + shift,line->gety() + ROCK_SIZE,line->getz(),0,0,0 };
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

void Obstacle::draw3D(Point3D pos, double camDeg, double camDepth, Engine* engine, bool& clean, int ind, double maxy) 
{
	if (!isBroken[ind]) 
	{
		BlenderObject_draw(pos, { 0,0,0 }, camDeg, camDepth, engine, clean, maxy, ind);
		clean = false;
	}
}
