#pragma once
#include <SDL.h>
#include "SDL_image.h"
#include "constants.h"
#include <iostream>
using namespace std;

class Image
{
	char path[100];		//�Ϥ����|
	SDL_Texture* texture;
	int width;		//�Ϥ��e��
	int height;

	int alpha;

public:
	Image() {}
	Image(const char* path, SDL_Renderer* renderer);	//�Ϥ���l��
	void set(const char* path, SDL_Renderer* renderer);		//�]�w���|�Prenderer
	void close();
	void draw(SDL_Renderer* renderer, SDL_Rect* src, SDL_Rect* dst, bool flip = false);
	int getWidth() { return width; }
	int getHeight() { return height; }
	void setAlpha(int a) { alpha = a; }
	int getAlpha() { return alpha; }
};

