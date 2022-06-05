#include "System.h"

void System::init() {

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "[System] SDL_Init Failed: " << SDL_GetError() << endl;
		SDL_Delay(3000);
		exit(1);
	}
	cout << "[System] SDL_Init Succeeded" << endl;

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		cout << "[System] IMG_Init Failed: " << IMG_GetError() << endl;
		SDL_Delay(3000);
		exit(1);

	}

	// Initialize SDL_ttf	
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		exit(1);
	}

	cout << "[System] IMG_Init Succeeded" << endl;
}
void System::quit() {
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}