#include "Obstacle.h"
Obstacle::Obstacle()
{
}
Obstacle::~Obstacle()
{
}
Obstacle::Obstacle(const char* path, int n, SDL_Renderer* renderer)
{
	touchtime = 0;
	setImage(path, n, renderer, 0);
}
void Obstacle::setObstacle(Line* line)
{
	setEntity(line);
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
