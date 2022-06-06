#include "Functions.h"
Functions::~Functions()
{
}
Functions::Functions(RenderWindow& w, bool* d, bool* q) : count(3),window(w),
starttext("START", "../fonts/akabara-cinderella.ttf", 75, 0x00, { 255, 255, 0 }, BLENDED, { NULL, NULL, NULL }, window.GetRenderer(), { 470, 410 }, { NULL, NULL }, NULL, SDL_FLIP_NONE, 255),
icontext("3D Racing Car", "../fonts/akabara-cinderella.ttf", 100, 0x00, { 0, 255, 255 }, BLENDED, { NULL, NULL, NULL }, window.GetRenderer(), { 250, 200 }, { NULL, NULL }, NULL, SDL_FLIP_NONE, 255),
modetext("Single", "../fonts/akabara-cinderella.ttf", 30, 0x00, { 0, 0, 255 }, BLENDED, { NULL, NULL, NULL }, window.GetRenderer(), { 18, 41 }, { NULL, NULL }, NULL, SDL_FLIP_NONE, 255),
counttext("3", "../fonts/akabara-cinderella.ttf", 100, 0x00, { 255, 255, 255 }, BLENDED, { NULL, NULL, NULL }, window.GetRenderer(), { 260, 175 }, { NULL, NULL }, NULL, SDL_FLIP_NONE, 255),
recordtext("Record", "../fonts/akabara-cinderella.ttf", 26, 0x00, { 0, 0, 255 }, BLENDED, { NULL, NULL, NULL }, window.GetRenderer(), { 18, 168 }, { NULL, NULL }, NULL, SDL_FLIP_NONE, 255),
endtext("WIN", "../fonts/akabara-cinderella.ttf", 100, 0x00, { 255, 255, 255 }, BLENDED, { NULL, NULL, NULL }, window.GetRenderer(), { 260, 175 }, { NULL, NULL }, NULL, SDL_FLIP_NONE, 255)
{
	dual = d;
	quit = q;
}
void Functions::Menu(SDL_Renderer* renderer)
{
	SDL_RenderSetViewport(window.GetRenderer(), NULL);
	count = 3;
	SDL_Event e;
	int x, y;
	SDL_GetMouseState(&x, &y);
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
		{
			exit(0);
		}
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			if (e.button.button == SDL_BUTTON_LEFT)
			{
				//mode change(dual<->single)
				if ((x - 60) * (x - 60) <= 2500 && (y - 60) * (y - 60) <= 2500)
				{
					modetext.close();
					if ((*dual))
					{
						*dual = false;
						modetext.setString("Single");
						modetext.setPos({ 18, 41 });
					}
					else
					{
						*dual = true;
						modetext.setString("Dual");
						modetext.setPos({ 29, 42 });
					}
					modetext.generateTexture();
				}

				//start
				else if (x >= 300 && x <= 900 && y >= 400 && y <= 500)
					(* quit) = true;

				//record
				else if ((x - 60) * (x - 60) <= 2500 && (y - 180) * (y - 180) <= 2500)
				{

				}
			}
		}
	}
	starttext.draw();

	icontext.draw();
	roundedRectangleRGBA(renderer, 300, 400, 900, 500, 1, 255, 0, 255, 255);

	modetext.draw();
	if ((x - 60) * (x - 60) <= 2500 && (y - 60) * (y - 60) <= 2500)
		thickCircleRGBA(renderer, 60, 60, 52, 0, 255, 255, 255, 10);
	else
		aacircleRGBA(renderer, 60, 60, 50, 255, 0, 0, 255);

	recordtext.draw();
	if ((x - 60) * (x - 60) <= 2500 && (y - 180) * (y - 180) <= 2500)
		thickCircleRGBA(renderer, 60, 180, 52, 0, 255, 255, 255, 10);
	else
		aacircleRGBA(renderer, 60, 180, 50, 255, 0, 0, 255);

}

void Functions::Counting(Map& map)
{
	while (count)
	{
		counttext.close();
		window.clear();
		sprintf_s(countstring, "%d", count);
		counttext.setString(countstring);
		counttext.generateTexture();
		map.draw(window.GetRenderer());
		counttext.draw();
		if ((*dual))
		{
			counttext.close();
			counttext.setPos({ 860, 175 });
			counttext.generateTexture();
			counttext.draw();
			counttext.setPos({ 260, 175 });
		}
		window.display();
		SDL_Delay(1000);
		count--;
	}
}

void Functions::Victory(EndType type)
{
	endtext.close();
	if (type == PLAYER1)
	{
		endtext.setString("WIN");
		endtext.setPos({ 240, 175 });
	}
	else if (type == PLAYER2)
	{
		endtext.setString("WIN");
		endtext.setPos({ 840, 175 });
	}
	else if (type == VICTORY)
	{
		endtext.setString("VICTORY");
		endtext.setPos({ 110, 175 });
	}
	else if (type == FAILED)
	{
		endtext.setString("FAILED");
		endtext.setPos({ 130, 175 });
	}
	endtext.generateTexture();
	endtext.draw();
	window.display();
}
void Functions::Grade(EndType type, int record)
{

}

void Functions::Record()
{

}
void Functions::close()
{
	starttext.close();
	icontext.close();
	modetext.close();
	recordtext.close();
	counttext.close();
}