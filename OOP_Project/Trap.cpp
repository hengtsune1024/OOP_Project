#include "Trap.h"
Trap::Trap() {

}

Trap::~Trap() {

}
Trap::Trap(const char* path, int n, SDL_Renderer* renderer) :stain("../images/stain.png", renderer)
{
	staintime = SDL_GetTicks64() - STAIN_INTERVAL;
	setImage(path, n, renderer, 0);
}

void Trap::setTrap(Line *line) {
	setEntity(line);
}
/*
void Trap::draw(SDL_Renderer* renderer, Line *line) {
	
	drawImg(renderer, line);
}
*/
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