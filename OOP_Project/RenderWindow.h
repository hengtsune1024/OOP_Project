#pragma once
#include "constants.h"
#include <iostream>
#include <SDL.h>
using namespace std;

class RenderWindow
{
	SDL_Window* window;
	SDL_Renderer* renderer;
public:
	RenderWindow() :window(nullptr), renderer(nullptr) {}

	void init(bool);
	void quit();

	void clear();
	void display();

	SDL_Renderer* GetRenderer() { return renderer; }

};

