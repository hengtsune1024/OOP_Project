#pragma once
#include <SDL.h>
#include "SDL_image.h"
#include "constants.h"
#include <iostream>
using namespace std;

class Image
{
	SDL_Texture* texture;
	int width;		//圖片寬高
	int height;
	int alpha;

public:
	Image();
	Image(const char* path, SDL_Renderer* renderer);	//圖片初始化
	~Image() {}

	//getter
	int getWidth() { return width; }
	int getHeight() { return height; }
	int getAlpha() { return alpha; }

	//setter
	void set(const char* path, SDL_Renderer* renderer);		//設定路徑與renderer
	void setAlpha(int a) { alpha = a; }

	//render image to the window
	void draw(SDL_Renderer* renderer, SDL_Rect* src, SDL_Rect* dst, bool flip = false);

	//close
	void close();
};

