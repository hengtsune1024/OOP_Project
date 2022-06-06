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
	Text starttext;
	Text icontext;
	Text modetext;
	Text counttext;
	Text endtext;
	Text gradetext;
	Text recordtext;
	Text hinttext;
	Uint64 rec1, rec2, rec3;
	Text  rec1text, rec2text, rec3text;
	int count;
	char countstring[10];
	char recordstr[50];
public:
	~Functions();
	Functions(RenderWindow& window, bool* d, bool* q);
	void Menu(SDL_Renderer* renderer);
	void Counting(Map& map);
	void Victory(EndType type);
	void Grade(EndType type, int record);
	void Record();
	void close();
};

