/*
* the car can get out of the road
* and the camera can turn 75 degrees each side
* but when the car is at the sideways,  the background and road image will be broken.
*/

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "System.h"
#include "RenderWindow.h"
#include "RacingCar.h"
#include "Map.h" 

void eventHandler(SDL_Event&, RenderWindow&, RacingCar&, Map&);

int main(int argc, char* argv[]) {
	System sdl;
	RenderWindow window;
	sdl.init();
	window.init();

	Map map;
	map.init();

	RacingCar car;
	car.init(map,window.GetRenderer());

	SDL_Event e;
	bool quit = false;
	
	map.startTimer(50);
	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
				quit = true;
				break;
			}
			eventHandler(e, window, car, map);
		}
		window.clear();
		map.draw(window.GetRenderer());
		car.draw(window.GetRenderer());
		window.display();
	}

	map.quit();
	car.quit();
	
	window.quit();
	sdl.quit();
	return 0;
}

void eventHandler(SDL_Event& e, RenderWindow& w, RacingCar& c, Map& m) {
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.sym) {
			case SDLK_UP:
				cout << "[Main] Press button UP" << endl;
				m.setVelX(m.getVelX() + SPEED);
				break;
			case SDLK_DOWN:
				cout << "[Main] Press button DOWN" << endl;
				m.setVelX(m.getVelX() - SPEED);
				break;
			case SDLK_LEFT:
				cout << "[Main] Press button LEFT" << endl;
				c.turn(-1);
				m.setVelY(m.getVelY() - ROTATE);
				break;
			case SDLK_RIGHT:
				cout << "[Main] Press button RIGHT" << endl;
				c.turn(1);
				m.setVelY(m.getVelY() + ROTATE);
				break;
			case SDLK_SPACE:
				SDL_Delay(5000);
				cout << "[Main] Press button SPACE" << endl;
				break;
		}
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
		switch (e.key.keysym.sym) {
			case SDLK_UP:
				m.setVelX(m.getVelX() - SPEED);
				break;
			case SDLK_DOWN:
				m.setVelX(m.getVelX() + SPEED);
				//else if (c.getVelX() == 0)
					//c.setVel(c.getVelX() - 30);
				break;
			case SDLK_LEFT:
				m.setVelY(m.getVelY() + ROTATE);
				c.turn(0);
				break;
			case SDLK_RIGHT:
				m.setVelY(m.getVelY() - ROTATE);
				c.turn(0);
				break;
		}
	}
}