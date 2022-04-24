#pragma once
#include <SDL.h>
#include "SDL_image.h"
#include "constants.h"
#include <iostream>
using namespace std;

class Image
{
	char path[100];		//圖片路徑
	SDL_Texture* texture;
	int width;		//圖片寬高
	int height;

public:
	Image() {}
	Image(const char* path, SDL_Renderer* renderer);	//圖片初始化
	void set(const char* path, SDL_Renderer* renderer);		//設定路徑與renderer
	void close();
	void draw(SDL_Renderer* renderer, SDL_Rect* src, SDL_Rect* dst, bool flip = false);
	int getWidth() { return width; }
	int getHeight() { return height; }
};

