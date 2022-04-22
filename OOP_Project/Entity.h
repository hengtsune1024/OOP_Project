#pragma once
#include <SDL.h>
#include "Image.h"
#include "Line.h"
#include <vector>

class Entity
{
	Image* entityImg;	//image array
	int numImage;	//array size
	int state;		//index of drawing image in array
	//vector<Line>& line;

	SDL_TimerID entitytimer;
	Uint32 time;
	static Uint32 changeData(Uint32 interval, void* param); // Timer callback 

public:
	Entity();
	virtual ~Entity();

	void quit();
	void startTimer(Uint32 t);
	void stopTimer();
	void setEntity(Line *line);
	

protected:
	void setImage(const char[], int, SDL_Renderer*, int = 0);

	int getState() { return state; }
	Image& getCurrentState() { return entityImg[state]; }
	void setState(int s) { state = s; }

	void closeImg();
	void drawImg(SDL_Renderer*, Line *);

	
};

