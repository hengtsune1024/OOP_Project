#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "System.h"
#include "RenderWindow.h"
#include "Map.h"
#include "RacingCar.h"
#include "Functions.h"

void eventHandler(SDL_Event&, RenderWindow&, Map&, RacingCar*, RacingCar* = NULL);

int main(int argc, char* argv[]) 
{
	bool dual = false;
	bool quit = false;

	System sdl;
	RenderWindow window;
	sdl.init();
	SDL_Event e;
	window.init(false);
	Image Ferrari("../images/ferrari.png", window.GetRenderer());
	Functions func(window, &dual, &quit);
	while (!quit)
	{
		//Menu
		while (!quit)
		{
			window.clear();
			Ferrari.draw(window.GetRenderer(), NULL, NULL, false);
			func.Menu(window.GetRenderer());
			window.display();
		}

		quit = false;
		Map map(window.GetRenderer(), dual);
		map.startTimer();
		unsigned int st, end, i = 0;

		while (!quit) {

			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
					quit = true;
					break;
				}
				eventHandler(e, window, map, map.getCar1(), map.getCar2());
			}
			st = SDL_GetTicks();

			window.clear();
			map.draw(window.GetRenderer());
			if (map.getendtype() != PLAYING)
			{
				func.Victory(map.getendtype());
				if (SDL_GetTicks64() >= map.getendtime())
				{
					func.Grade(map.getendtype(), map.getrecord());
					break;
				}
			}

			//Counting 3 2 1
			func.Counting(map);
			window.display();

			 end = SDL_GetTicks();
			//calculate fps
			if (i == 16) {
				i = 0;
				printf("%lf\n", 1000.0 / (end - st));
			}
			else {
				++i;
			}
		}
		map.quit();
	}
	func.close();
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
				//car1->turn(-1);
				break;
			case SDLK_d:
				car1->setVelAngular(car1->getVelAngular() + ROTATE);
				//car1->turn(1);
				break;
			case SDLK_SPACE:
				car1->rush(ENERGY);
				break;

			/*
			case SDLK_q:
				car1->setVelPerpen(car1->getVelPerpen() - 300);
				break;
			*/
			case SDLK_1:
				car1->usetool(SPEEDUP);
				break;
			case SDLK_2:
				car1->usetool(INVINCIBLE);
				break;
			case SDLK_3:

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
					//car2->turn(-1);
				}
				break;
			case SDLK_RIGHT:
				if (car2) {
					car2->setVelAngular(car2->getVelAngular() + ROTATE);
					//car2->turn(1);
				}
				break;
			case SDLK_RETURN:
				car2->rush(ENERGY);
				break;

				/*
			case SDLK_l:
				car2->setVelPerpen(car2->getVelPerpen() - 300);
				*/
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
				//car1->turn(0);
				break;
			case SDLK_d:
				car1->setVelAngular(car1->getVelAngular() - ROTATE);
				//car1->turn(0);
				break;
				/*
			case SDLK_q:
				car1->setVelPerpen(car1->getVelPerpen() + 300);
				break;
				*/

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
					//car2->turn(0);
				}
				break;
			case SDLK_RIGHT:
				if (car2) {
					car2->setVelAngular(car2->getVelAngular() - ROTATE);
					//car2->turn(0);
				}
				break;
				/*
			case SDLK_l:
				car2->setVelPerpen(car2->getVelPerpen() + 300);
				break;
				*/
			default:;
		}
	}
}
