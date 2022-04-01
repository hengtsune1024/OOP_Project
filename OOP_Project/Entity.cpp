#include "Entity.h"

Entity::Entity() {
	entityImg = NULL;
}
Entity::~Entity() {

}
void Entity::setImage(const char path[], int num_Imag, SDL_Renderer* renderer, int init_state) {
	numImage = num_Imag;
	entityImg = new Image[numImage];
	for (int i = 0; i < numImage; ++i) {
		char _path[100];
		sprintf_s(_path, "%s%d.png", path, i);
		entityImg[i].set(_path, renderer);
	}
	state = init_state;
}

void Entity::closeImg() {
	for (int i = 0; i < numImage; ++i)
		entityImg[i].close();
}

void Entity::drawImg(SDL_Renderer* renderer,SDL_Rect src,SDL_Rect dst) {
	SDL_Rect* s = &src, * d = &dst;
	if (s->x == ALL_REGION)
		s = NULL;
	if (d->x == ALL_REGION)
		d = NULL;
	entityImg[state].draw(renderer, s, d);
}
