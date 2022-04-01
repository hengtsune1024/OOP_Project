#include "System.h"

void System::init() {

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "[System] SDL_Init Failed: " << SDL_GetError() << endl;
		exit(0);
	}
	cout << "[System] SDL_Init Succeeded" << endl;

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		cout << "[System] IMG_Init Failed: " << IMG_GetError() << endl;
		exit(0);

	}
	cout << "[System] IMG_Init Succeeded" << endl;

}
void System::quit() {
	IMG_Quit();
	SDL_Quit();
}