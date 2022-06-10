#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>
#include "System.h"
#include "RenderWindow.h"
#include "constants.h"
#include "Text.h"
#include "Map.h"
class Functions
{
	RenderWindow& window;
	bool* dual;
	bool* quit;
	bool* menu;
	Text starttext;
	Text icontext;
	Text modetext;
	Text counttext;
	Text endtext;
	Text gradetext;
	Text recordtext;
	Text hinttext;

	Text quittext;

	Uint64 rec[3];
	Text  rec1text, rec2text, rec3text;
	int count;
	char countstring[10];
	char recordstr[50];
public:
	~Functions();
	Functions(RenderWindow& window, bool* d, bool* q, bool* m);
	void Menu(SDL_Renderer* renderer);
	void Counting(Map& map);
	void Victory(EndType type);
	void Grade(EndType type, int record);
	void Record();
	void setrecord(int r);
	void close();
};

