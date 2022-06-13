#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "System.h"
#include "RenderWindow.h"
#include "Map.h"
#include "RacingCar.h"
#include "Functions.h"

void eventHandler(SDL_Event&, RenderWindow&, Map&, int&, int&, RacingCar*, RacingCar* = NULL);
void drawchosentool(SDL_Renderer*, int&, int&, bool);

int main(int argc, char* argv[]) 
{
	bool dual = 0;
	bool menu = true;
	bool quit = false;

	System sdl;
	RenderWindow window;
	sdl.init();
	SDL_Event e;
	window.init(false);
	Image Ferrari("../images/ferrari.png", window.GetRenderer());
	Functions func(window, &dual, &quit, &menu);
	while (!quit)
	{
		//Menu
		menu = 1;
		while (menu && !quit)
		{
			window.clear();
			Ferrari.draw(window.GetRenderer(), NULL, NULL, false);
			func.Menu(window.GetRenderer());
			window.display();
		}

		if (quit)
			break;

		Map map(window.GetRenderer(), dual);
		map.startTimer();
		unsigned int st, end, i = 0;
		int chosen1 = 1;
		int chosen2 = 1;
		while (!quit) {

			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
					quit = true;
					break;
				}
				eventHandler(e, window, map, chosen1, chosen2, map.getCar1(), map.getCar2());
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
			drawchosentool(window.GetRenderer(), chosen1, chosen2, dual);
			
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

void eventHandler(SDL_Event& e, RenderWindow& w, Map& map, int& chosen1, int& chosen2, RacingCar* car1, RacingCar* car2)
{
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		//car 1
		case SDLK_w:
			if (!car1->Dizzy())
				car1->brake(1);
			break;
		case SDLK_s:
			if (!car1->Dizzy())
				car1->brake(2);
			break;
		case SDLK_a:
			if (!car1->Dizzy())
			{
				if (car1->islost())
					car1->setVelAngular(car1->getVelAngular() + ROTATE);
				else {
					car1->setVelAngular(car1->getVelAngular() - ROTATE);
				}
			}
			break;
		case SDLK_d:
			if (!car1->Dizzy())
			{
				if (car1->islost())
					car1->setVelAngular(car1->getVelAngular() - ROTATE);
				else
					car1->setVelAngular(car1->getVelAngular() + ROTATE);
			}
			break;
		case SDLK_q:
			if (car2)
				chosen1 < 6 ? chosen1++ : chosen1 = 1;
			else
				chosen1 < 4 ? chosen1++ : chosen1 = 1;
			break;
		case SDLK_e:
			switch (chosen1)
			{
			case 1:
				if (!car1->Dizzy())
					car1->usetool(SPEEDUP, map.getTool(), true);
				break;
			case 2:
				if (!car1->Dizzy())
					car1->usetool(INVINCIBLE, map.getTool(), true);
				break;
			case 3:
				if (!car1->Dizzy())
					car1->usetool(HEALING, map.getTool(), true);
				break;
			case 4:
				if (!car1->Dizzy())
					car1->usetool(GHOST, map.getTool(), true);
				break;
			case 5:
				if (car2 && !car1->Dizzy())
					if (car1->usetool(LIGHTNING, map.getTool(), true))
						car2->beattacked();
				break;
			case 6:
				if (car2 && !car1->Dizzy())
				{
					if (car1->usetool(SWITCH, map.getTool(), true))
						if (car1->getPosX() < car2->getPosX())
							map.changecar();
				}
				break;
			}
			break;
		case SDLK_SPACE:
			if (!car1->Dizzy())
				car1->rush(ENERGY);
				break;
		/*
			break;
		
		case SDLK_1:
			if (!car1->Dizzy())
				car1->usetool(SPEEDUP, map.getTool(), true);
			break;
		case SDLK_2:
			if (!car1->Dizzy())
				car1->usetool(INVINCIBLE, map.getTool(), true);
			break;
		case SDLK_3:
			if (!car1->Dizzy())
				car1->usetool(HEALING, map.getTool(), true);
			break;
		case SDLK_4:
			if (!car1->Dizzy())
				car1->usetool(GHOST, map.getTool(), true);
			break;
		case SDLK_5:
			if (car2 && !car1->Dizzy())
				if (car1->usetool(LIGHTNING, map.getTool(), true))
					car2->beattacked();
					if (car2->getHP() <= 0)
					{
						map.setEndType(PLAYER1);
					}
				}
			break;
		case SDLK_6:
			if (car2 && !car1->Dizzy())
			{
				if (car1->usetool(SWITCH, map.getTool(), true))
					if (car1->getPosX() < car2->getPosX())
						map.changecar();
			}
			break;
		*/


			//car 2
		case SDLK_i:
			if (car2 && !car2->Dizzy())
				car2->brake(1);
			break;
		case SDLK_k:
			if (car2 && !car2->Dizzy())
				car2->brake(2);
			break;
		case SDLK_j:
			if (car2 && !car2->Dizzy()) 
			{
				if (car2->islost())
					car2->setVelAngular(car2->getVelAngular() + ROTATE);
				else {
					car2->setVelAngular(car2->getVelAngular() - ROTATE);
				}
			}
			break;
		case SDLK_l:
			if (car2 && !car2->Dizzy()) 
			{
				if (car2->islost())
					car2->setVelAngular(car2->getVelAngular() - ROTATE);
				else
					car2->setVelAngular(car2->getVelAngular() + ROTATE);
			}
			break;
		case SDLK_RETURN:
			if (car2 && !car2->Dizzy())
				car2->rush(ENERGY);
			break;

		case SDLK_u:
			chosen2 < 6 ? chosen2++ : chosen2 = 1;
			break;
		case SDLK_o:
			switch (chosen2)
			{
			case 1:
				if (car2 && !car2->Dizzy())
					car2->usetool(SPEEDUP, map.getTool(), false);
				break;
			case 2:
				if (car2 && !car2->Dizzy())
					car2->usetool(INVINCIBLE, map.getTool(), false);
				break;
			case 3:
				if (car2 && !car2->Dizzy())
					car2->usetool(HEALING, map.getTool(), false);
				break;
			case 4:
				if (car2 && !car2->Dizzy())
					car2->usetool(GHOST, map.getTool(), false);
				break;
			case 5:
				if (car2 && !car2->Dizzy())
					if (car2->usetool(LIGHTNING, map.getTool(), false))
						car1->beattacked();
				break;
			case 6:
				if (car2 && !car2->Dizzy())
				{
					if (car2->usetool(SWITCH, map.getTool(), false))
						if (car2->getPosX() < car1->getPosX())
							map.changecar();
				}
				break;
			}
			break;

			/*
		case SDLK_l:
			car2->setVelPerpen(car2->getVelPerpen() - 300);
			*/
		/*
		case SDLK_KP_1:
			if (car2 && !car2->Dizzy())
				car2->usetool(SPEEDUP, map.getTool(), false);
			break;
		case SDLK_KP_2:
			if (car2 && !car2->Dizzy())
				car2->usetool(INVINCIBLE, map.getTool(), false);
			break;
		case SDLK_KP_3:
			if (car2 && !car2->Dizzy())
				car2->usetool(HEALING, map.getTool(), false);
			break;
		case SDLK_KP_4:
			if (car2 && !car2->Dizzy())
				car2->usetool(GHOST, map.getTool(), false);
			break;
		case SDLK_KP_5:
			if (car2 && !car2->Dizzy())
				if (car2->usetool(LIGHTNING, map.getTool(), false)) {
					car1->beattacked();
					if (car1->getHP() <= 0)
					{
						map.setEndType(PLAYER2);
					}
				}
			break;
		case SDLK_KP_6:
			if (car2 && !car2->Dizzy())
			{
				if (car2->usetool(SWITCH, map.getTool(), false))
					if (car2->getPosX() < car1->getPosX())
						map.changecar();
			}
			break;
		*/
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
				if (!car1->Dizzy())
					car1->brake(0);
				break;
			case SDLK_a:
			case SDLK_d:
				if (!car1->Dizzy()) 
				{
					car1->setVelAngular(0);
				}
				break;

			//car 2
			case SDLK_i:
			case SDLK_k:
				if (car2 && !car2->Dizzy()) {
					car2->brake(0);
				}
				break;
			case SDLK_j:
			case SDLK_l:
				if (car2 && !car2->Dizzy())
				{
					car2->setVelAngular(0);
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

void drawchosentool(SDL_Renderer* renderer, int& chosen1, int& chosen2, bool dual)
{
	
	switch (chosen1)
	{
	case 1:
		boxRGBA(renderer, 350, 10, 385, 45, 0, 255, 255, 100);
		break;
	case 2:
		boxRGBA(renderer, 390, 10, 425, 45, 0, 255, 255, 100);
		break;
	case 3:
		boxRGBA(renderer, 430, 10, 465, 45, 0, 255, 255, 100);
		break;
	case 4:
		boxRGBA(renderer, 470, 10, 505, 45, 0, 255, 255, 100);
		break;
	case 5:
		boxRGBA(renderer, 510, 10, 545, 45, 0, 255, 255, 100);
		break;
	case 6:
		boxRGBA(renderer, 550, 10, 585, 45, 0, 255, 255, 100);
		break;
	}
	
	
	if (dual)
	{
		SDL_Rect viewPort2 = { WIDTH,0,WIDTH,HEIGHT };
		SDL_RenderSetViewport(renderer, &viewPort2);
		switch (chosen2)
		{
		case 1:
			boxRGBA(renderer, 350, 10, 385, 45, 0, 255, 255, 100);
			break;
		case 2:
			boxRGBA(renderer, 390, 10, 425, 45, 0, 255, 255, 100);
			break;
		case 3:
			boxRGBA(renderer, 430, 10, 465, 45, 0, 255, 255, 100);
			break;
		case 4:
			boxRGBA(renderer, 470, 10, 505, 45, 0, 255, 255, 100);
			break;
		case 5:
			boxRGBA(renderer, 510, 10, 545, 45, 0, 255, 255, 100);
			break;
		case 6:
			boxRGBA(renderer, 550, 10, 585, 45, 0, 255, 255, 100);
			break;
		}
		SDL_RenderSetViewport(renderer, NULL);
	}
	
}