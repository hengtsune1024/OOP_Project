#include "Trap.h"

Trap::Trap() {

}
Trap::~Trap() {

}
Trap::Trap(const char* path, int n, SDL_Renderer* renderer) :stain("../images/stain.png", renderer)

{
	staintime = 0;
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
	if (staintime != 0)
	{
		//stain.draw(renderer, { NULL }, { NULL });
		stain.draw(renderer, NULL, NULL);
		staintime--;
	}
}

void Trap::gettrap(TrapType type) {
	switch (type)
	{
		case STAIN:
			staintime = 100000;
			break;
		case DIZZY:

			break;
		case LOST:

			break;

	}
}
int Trap::getWidth() {
	return 0;
}
int Trap::getHeight() {
	return 0;
}

