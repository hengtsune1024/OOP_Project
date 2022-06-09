#include "Obstacle.h"
Obstacle::Obstacle() :obst3D(NULL, NULL, ROCK_SIZE)
{
}
Obstacle::~Obstacle()
{
}
Obstacle::Obstacle(const char* objpath, const char* imgpath) : obst3D(objpath, imgpath, ROCK_SIZE)
{
	touchtime = 0;
}
void Obstacle::setObstacle(Line* line)
{
	
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

void Obstacle::drawObject3D(Point3D pos, double camDeg, double camDepth, Engine* engine, bool& clean, double maxy) {
	obst3D.draw(pos, { 0,0,0 }, camDeg, camDepth, engine, clean, maxy);
	clean = false;
}