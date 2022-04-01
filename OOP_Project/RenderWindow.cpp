#include "RenderWindow.h"

void RenderWindow::init() {
	window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		cout << "[RenderWindow] Window initialization failed : " << SDL_GetError() << endl;
		return;
	}
	cout << "[RenderWindow] Window initialized" << endl;

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		cout << "[RenderWindow] Renderer initialization failed : " << SDL_GetError() << endl;
		return;
	}
	cout << "[RenderWindow] Renderer initialized" << endl;
}

void RenderWindow::display() {
	SDL_RenderPresent(renderer);
}

void RenderWindow::quit() {
	SDL_DestroyWindow(window);
	cout << "[RenderWindow] Window quit" << endl;
	SDL_DestroyRenderer(renderer);
	cout << "[RenderWindow] Renderer quit" << endl;
}

void RenderWindow::clear() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
	SDL_RenderClear(renderer);
}