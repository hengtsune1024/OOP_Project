/*
* The road model is almost completed
* car cannot move out of the road side over the distance of ROAD_WIDTH 
* the restriction on the degree of camera rotation is now depending on the road vector
*/

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "System.h"
#include "RenderWindow.h"
#include "Map.h" 
#include "RacingCar.h"

void eventHandler(SDL_Event&, RenderWindow&, Map&);

int main(int argc, char* argv[]) {
	System sdl;
	RenderWindow window;
	sdl.init();
	window.init();
	
	Map map(window.GetRenderer());
	
	SDL_Event e;
	bool quit = false;

	map.startTimer();

	while (!quit) {

		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
				quit = true;
				break;
			}
			eventHandler(e, window, map);
		}
		window.clear();
		map.draw(window.GetRenderer());
		window.display();
	}

	map.quit();
	window.quit();
	sdl.quit();
	return 0;
}

void eventHandler(SDL_Event& e, RenderWindow& w, Map& m) {
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.sym) {
			case SDLK_UP:
				cout << "[Main] Press button UP" << endl;
				m.setAccLinear(ACCELERATION);
				break;
			case SDLK_DOWN:
				cout << "[Main] Press button DOWN" << endl;
				m.setAccLinear(-ACCELERATION);
				break;
			case SDLK_LEFT:
				cout << "[Main] Press button LEFT" << endl;
				m.setVelAngular(m.getVelAngular() - ROTATE);
				m.turn(-1);
				break;
			case SDLK_RIGHT:
				cout << "[Main] Press button RIGHT" << endl;
				m.setVelAngular(m.getVelAngular() + ROTATE);
				m.turn(1);
				break;
			case SDLK_SPACE:
				cout << "[Main] Press button SPACE" << endl;
				m.rush();
				break;
		}
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
		switch (e.key.keysym.sym) {
			case SDLK_UP:
			case SDLK_DOWN:
				if (m.getVelLinear() > 0)
					m.setAccLinear(-FRICTION_ACC);
				else
					m.setAccLinear(FRICTION_ACC);
				break;
			case SDLK_LEFT:
				m.setVelAngular(m.getVelAngular() + ROTATE);
				m.turn(0);
				break;
			case SDLK_RIGHT:
				m.setVelAngular(m.getVelAngular() - ROTATE);
				m.turn(0);
				break;
		}
	}
}

