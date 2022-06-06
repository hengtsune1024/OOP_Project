#include "Entity.h"

Entity::Entity() {

}

Entity::Entity(const char path[], int num_Imag, SDL_Renderer* renderer, int init_state) {
	numImage = num_Imag;
	entityImg = new Image[numImage];
	for (int i = 0; i < numImage; ++i) {
		char _path[100];
		sprintf_s(_path, "%s%02d.png", path, i + 1);
		entityImg[i].set(_path, renderer);
	}
	state = init_state;
}

Entity::~Entity() {
}
void Entity::setImage(const char path[], int num_Imag, SDL_Renderer* renderer, int init_state) 
{
	numImage = num_Imag;
	entityImg = new Image[numImage];
	for (int i = 0; i < numImage; ++i) {
		char _path[100];
		sprintf_s(_path, "%s%02d.png", path, i+1);
		entityImg[i].set(_path, renderer);
	}
	state = init_state;
}

void Entity::closeImg() {
	if (entityImg != NULL) {
		for (int i = 0; i < numImage; ++i)
			entityImg[i].close();
		delete[]entityImg;
	}
}

void Entity::drawImg(SDL_Renderer* renderer, Line *line) {
	if (shownflag)
		line->drawActSprite(renderer, state);
	/*
	SDL_Rect* s = &src, * d = &dst;
	if (s->x == ALL_REGION)
		s = NULL;
	if (d->x == ALL_REGION)
		d = NULL;
	entityImg[state].draw(renderer, s, d);
	*/
}

void Entity::setEntity(Line* line) {
	shownflag = true;
	line->setActSprite(entityImg, 2.5);
}

Uint32 Entity::changeData(Uint32 interval, void* param)
{
	Entity* p = (Entity*)param;
	if (p->time != 0)
	{
		if(p->numImage)
			p->state = (p->state + 1) % p->numImage;  // image frame	
		/*
		if (p->flag == true)
		{
			p->y = (p->y - 5 + HEIGHT) % HEIGHT; // image position
			if (p->y == 100)
				p->setShownFlag(false);
		}
		*/
		return interval;
	}
	else
	{
		return 0;
	}
}

void Entity::quit() {
	SDL_RemoveTimer(entitytimer);
	closeImg();
}
void Entity::startTimer(Uint32 t) {
	time = t;
	entitytimer = SDL_AddTimer(time, changeData, this); // Set Timer callback
}
void Entity::stopTimer() {
	time = 0;
}