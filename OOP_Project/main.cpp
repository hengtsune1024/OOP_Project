#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "System.h"
#include "RenderWindow.h"
#include "Map.h" 
#include "RacingCar.h"

void eventHandler(SDL_Event&, RenderWindow&, Map&, RacingCar*, RacingCar* = NULL);
bool dual = true;

int main(int argc, char* argv[]) 
{
	
	System sdl;
	RenderWindow window;
	sdl.init();
	window.init(dual);
	
	Map map(window.GetRenderer(), dual);
	
	SDL_Event e;
	bool quit = false;

	map.startTimer();

	while (!quit) {

		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
				quit = true;
				break;
			}
			eventHandler(e, window, map, map.getCar1(), map.getCar2());
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

void eventHandler(SDL_Event& e, RenderWindow& w, Map& map, RacingCar* car1, RacingCar* car2) 
{
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym) 
		{
			//car 1
			case SDLK_w:
				car1->brake(1);
				break;
			case SDLK_s:
				car1->brake(2);
				break;
			case SDLK_a:
				car1->setVelAngular(car1->getVelAngular() - ROTATE);
				car1->turn(-1);
				break;
			case SDLK_d:
				car1->setVelAngular(car1->getVelAngular() + ROTATE);
				car1->turn(1);
				break;
			case SDLK_SPACE:
				car1->rush(ENERGY);
				break;

			case SDLK_e:
				car1->setVelPerpen(car1->getVelPerpen() + 300);
				break;
			case SDLK_q:
				car1->setVelPerpen(car1->getVelPerpen() - 300);
				break;

			case SDLK_1:
				car1->usetool(SPEEDUP);
				break;
			case SDLK_2:
				car1->usetool(INVINCIBLE);
				break;
			case SDLK_3:

				break;
			case SDLK_0:
				dual = true;
				break;

			//car 2
			case SDLK_UP:
				if (car2)
					car2->brake(1);
				break;
			case SDLK_DOWN:
				if (car2)
					car2->brake(2);
				break;
			case SDLK_LEFT:
				if (car2) {
					car2->setVelAngular(car2->getVelAngular() - ROTATE);
					car2->turn(-1);
				}
				break;
			case SDLK_RIGHT:
				if (car2) {
					car2->setVelAngular(car2->getVelAngular() + ROTATE);
					car2->turn(1);
				}
				break;
			case SDLK_RETURN:
				car2->rush(ENERGY);
				break;

			case SDLK_KP_1:
				car2->usetool(SPEEDUP);
				break;
			case SDLK_KP_2:
				car2->usetool(INVINCIBLE);
				break;
			case SDLK_KP_3:

				break;


			default:;
		}
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym) 
		{
			//car 1
			case SDLK_s:
			case SDLK_w:
				car1->brake(0);
				break;
			case SDLK_a:
				car1->setVelAngular(car1->getVelAngular() + ROTATE);
				car1->turn(0);
				break;
			case SDLK_d:
				car1->setVelAngular(car1->getVelAngular() - ROTATE);
				car1->turn(0);
				break;

			case SDLK_e:
				car1->setVelPerpen(car1->getVelPerpen() - 300);
				break;
			case SDLK_q:
				car1->setVelPerpen(car1->getVelPerpen() + 300);
				break;


			//car 2
			case SDLK_UP:
			case SDLK_DOWN:
				if (car2) {
					car2->brake(0);
				}
				break;
			case SDLK_LEFT:
				if (car2) {
					car2->setVelAngular(car2->getVelAngular() + ROTATE);
					car2->turn(0);
				}
				break;
			case SDLK_RIGHT:
				if (car2) {
					car2->setVelAngular(car2->getVelAngular() - ROTATE);
					car2->turn(0);
				}
				break;

			default:;
		}
	}
}

