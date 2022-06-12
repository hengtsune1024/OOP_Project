#include "Functions.h"
Functions::~Functions()
{
}
Functions::Functions(RenderWindow& w, bool* d, bool* q, bool* m) : count(3),window(w),
starttext("START", "../fonts/akabara-cinderella.ttf", 75, 0x00, { 255, 255, 0 }, BLENDED, { NULL, NULL, NULL }, window.GetRenderer(), { 470, 410 }, { NULL, NULL }, NULL, SDL_FLIP_NONE, 255),
icontext("3D Racing Car", "../fonts/akabara-cinderella.ttf", 100, 0x00, { 0, 255, 255 }, BLENDED, { NULL, NULL, NULL }, window.GetRenderer(), { 250, 200 }, { NULL, NULL }, NULL, SDL_FLIP_NONE, 255),
modetext("Single", "../fonts/akabara-cinderella.ttf", 30, 0x00, { 0, 0, 255 }, BLENDED, { NULL, NULL, NULL }, window.GetRenderer(), { 18, 41 }, { NULL, NULL }, NULL, SDL_FLIP_NONE, 255),
counttext("3", "../fonts/akabara-cinderella.ttf", 100, 0x00, { 255, 255, 255 }, BLENDED, { NULL, NULL, NULL }, window.GetRenderer(), { 260, 175 }, { NULL, NULL }, NULL, SDL_FLIP_NONE, 255),
recordtext("Record", "../fonts/akabara-cinderella.ttf", 26, 0x00, { 0, 0, 255 }, BLENDED, { NULL, NULL, NULL }, window.GetRenderer(), { 18, 168 }, { NULL, NULL }, NULL, SDL_FLIP_NONE, 255),
endtext("WIN", "../fonts/akabara-cinderella.ttf", 100, 0x00, { 255, 255, 255 }, BLENDED, { NULL, NULL, NULL }, window.GetRenderer(), { 260, 175 }, { NULL, NULL }, NULL, SDL_FLIP_NONE, 255),
gradetext("GRADE", "../fonts/akabara-cinderella.ttf", 75, 0x00, { 255, 255, 255 }, BLENDED, { NULL, NULL, NULL }, window.GetRenderer(), { 470, 10 }, { NULL, NULL }, NULL, SDL_FLIP_NONE, 255),
hinttext("Click to continue", "../fonts/akabara-cinderella.ttf", 50, 0x00, { 211, 211, 211 }, BLENDED, { NULL, NULL, NULL }, window.GetRenderer(), { 390, 520 }, { NULL, NULL }, NULL, SDL_FLIP_NONE, 255),
rectext(" ", "../fonts/akabara-cinderella.ttf", 55, 0x00, { 255, 0, 0 }, BLENDED, { NULL, NULL, NULL }, window.GetRenderer(), { 250, 500 }, { NULL, NULL }, NULL, SDL_FLIP_NONE, 255),
quittext("Quit","../fonts/akabara-cinderella.ttf", 26, 0x00, { 0, 0, 255 }, BLENDED, { NULL, NULL, NULL }, window.GetRenderer(), { 1112, 527 }, { NULL, NULL }, NULL, SDL_FLIP_NONE, 255),
maptext(" ", "../fonts/akabara-cinderella.ttf",40,0x00,{0,0,0}, BLENDED, { NULL, NULL, NULL }, window.GetRenderer(), { 0, 0 }, { NULL, NULL }, NULL, SDL_FLIP_NONE, 255)
{
	FILE* f = fopen("../bin/record.dat", "rb");
	fseek(f, 0, SEEK_SET);
	fread(rec, sizeof(rec), 1, f);
	fclose(f);
	dual = d;
	quit = q;
	menu = m;
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
			*quit = true;
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
					(*menu) = false;

				//record
				else if ((x - 60) * (x - 60) <= 2500 && (y - 180) * (y - 180) <= 2500)
				{
					Record(renderer);
				}

				//quit
				else if ((x - 1140) * (x - 1140) <= 2500 && (y - 540) * (y - 540) <= 2500)
					*quit = true;
			}
		}
	}

	icontext.draw();
	//start button
	if ((x >= 300 && x <= 900) && (y >= 400 && y <= 500)) {
		boxRGBA(renderer, 300, 400, 900, 500, 255, 255, 255, 128);
		thickLineRGBA(renderer, 300, 400, 900, 400, 10, 0, 255, 255, 255);
		thickLineRGBA(renderer, 300, 500, 900, 500, 10, 0, 255, 255, 255);
		thickLineRGBA(renderer, 300, 400, 300, 500, 10, 0, 255, 255, 255);
		thickLineRGBA(renderer, 900, 400, 900, 500, 10, 0, 255, 255, 255);
		filledPieRGBA(renderer, 300, 400, 5, 180, 270, 0, 255, 255, 255);
		filledPieRGBA(renderer, 900, 400, 5, 270, 360, 0, 255, 255, 255);
		filledPieRGBA(renderer, 300, 500, 5, 90, 180, 0, 255, 255, 255);
		filledPieRGBA(renderer, 900, 500, 5, 0, 90, 0, 255, 255, 255);
	}
	else
		roundedRectangleRGBA(renderer, 300, 400, 900, 500, 1, 255, 0, 255, 255);
	starttext.draw();

	//mode button
	if ((x - 60) * (x - 60) <= 2500 && (y - 60) * (y - 60) <= 2500) {
		filledCircleRGBA(renderer, 60, 60, 52, 255, 255, 255, 128);
		thickCircleRGBA(renderer, 60, 60, 52, 0, 255, 255, 255, 10);
	}
	else
		aacircleRGBA(renderer, 60, 60, 50, 255, 0, 0, 255);
	modetext.draw();

	//record button
	if ((x - 60) * (x - 60) <= 2500 && (y - 180) * (y - 180) <= 2500) {
		filledCircleRGBA(renderer, 60, 180, 52, 255, 255, 255, 128);
		thickCircleRGBA(renderer, 60, 180, 52, 0, 255, 255, 255, 10);
	}
	else
		aacircleRGBA(renderer, 60, 180, 50, 255, 0, 0, 255);
	recordtext.draw();

	//quit button
	if ((x - 1140) * (x - 1140) <= 2500 && (y - 540) * (y - 540) <= 2500) {
		filledCircleRGBA(renderer, 1140, 540, 52, 255, 255, 255, 128);
		thickCircleRGBA(renderer, 1140, 540, 52, 0, 255, 255, 255, 10);
	}
	else
		aacircleRGBA(renderer, 1140, 540, 52, 255, 0, 0, 255);
	quittext.draw();
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
		endtext.generateTexture();
		endtext.draw();
		endtext.close();
		endtext.setString("HP = 0");
		endtext.setPos({ 130, 285 });
	}
	else if (type == ALLDEAD) 
	{
		endtext.setString("FAILED");
		endtext.generateTexture();
		endtext.setPos({ 130, 175 });
		endtext.draw();
		endtext.setPos({ 730, 175 });
		endtext.draw();

		endtext.close();
		endtext.setString("HP = 0");
		endtext.generateTexture();
		endtext.setPos({ 130, 285 });
		endtext.draw();
		endtext.setPos({ 730, 285 });
	}
	endtext.generateTexture();
	endtext.draw();
	window.display();
}
void Functions::Grade(EndType type, int record)
{
	SDL_RenderSetViewport(window.GetRenderer(), NULL);
	window.clear();
	gradetext.close();
	if (record)
	{
		setrecord(record);
		switch (type)
		{
			case PLAYER1:
				gradetext.setString("Winner : Player 1 ");
				gradetext.setPos({ 300, 10 });

				break;
			case PLAYER2:
				gradetext.setString("Winner : Player 2 ");
				gradetext.setPos({ 300, 10 });

				break;
			case VICTORY:
				gradetext.setString("Victory");
				gradetext.setPos({ 430, 10 });
				break;
		}
		gradetext.generateTexture();
		gradetext.draw();
		window.display();
		SDL_Delay(1000);

		int ms = record % 1000;
		record /= 1000;
		int sec = record % 60;
		record -= sec;
		int min = record / 60;
		sprintf_s(recordstr, "%02d : %02d : %03d", min, sec, ms);
		gradetext.close();
		gradetext.setString("Record:");
		gradetext.setPos({ 450, 150 });
		gradetext.generateTexture();
		gradetext.draw();

		gradetext.close();
		gradetext.setString(recordstr);
		gradetext.setPos({ 320, 300 });
		gradetext.generateTexture();
		gradetext.draw();

		window.display();
		SDL_Delay(2000);
	}
	else
	{
		switch (type)
		{
			case PLAYER1:
				gradetext.setString("Winner : Player 1 ");
				gradetext.setPos({ 300, 10 });

				break;
			case PLAYER2:
				gradetext.setString("Winner : Player 2 ");
				gradetext.setPos({ 300, 10 });

				break;
			case FAILED:
				gradetext.setString("FAILED");
				gradetext.setPos({ 430, 10 });
				break;
			case ALLDEAD:
				gradetext.setString("ALL DEAD");
				gradetext.setPos({ 400, 10 });
				break;
		}
		gradetext.generateTexture();
		gradetext.draw();
		window.display();
		SDL_Delay(1000);

		gradetext.close();
		gradetext.setString("Record:");
		gradetext.setPos({ 450, 150 });
		gradetext.generateTexture();
		gradetext.draw();

		gradetext.close();
		gradetext.setString("-- : -- : ---");
		gradetext.setPos({ 400, 300 });
		gradetext.generateTexture();
		gradetext.draw();

		window.display();
		SDL_Delay(2000);
	}
	hinttext.draw();
	window.display();
	SDL_Event e;
	bool conti = false;
	while (!conti)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
			{
				exit(0);
			}
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				conti = true;
			}
		}
	}
}

void Functions::Record(SDL_Renderer* renderer)
{
	bool quit = false;
	SDL_Event e;
	int record;
	int ms, sec, min;
	char recordstring[50] = "\0";

	boxColor(renderer, 100, 70, 1110, 500, 0x80ffffff);

	for (int j = 0; j < 10; ++j)
	{
		record = rec[j];
		if (record != 0)
		{
			ms = record % 1000;
			record /= 1000;
			sec = record % 60;
			record -= sec;
			min = record / 60;
			sprintf_s(recordstring, "%2d.  %02d : %02d : %03d", j + 1, min, sec, ms);
		}
		else {
			sprintf_s(recordstring, "%2d.  -- : -- : ---", j + 1);
		}
		rectext.close();
		rectext.setString(recordstring);
		if (j < 5) {
			rectext.setPos({ WIDTH - 50 - 440, 100 + j * 75 });
		}
		else {
			rectext.setPos({ WIDTH + 25, 100 + (j - 5) * 75});
		}
		
		rectext.generateTexture();
		printf("%d %d \n", rectext.getWidth(), rectext.getHeight());
		rectext.draw();
	}

	hinttext.draw();
	window.display();

	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
			{
				quit = true;
				*Functions::quit = true;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				quit = true;
			}
		}
	}
}

void Functions::setrecord(int r)
{
	int k = 9;
	while (k >= 0 && rec[k] == 0)
		--k;
	++k;
	if (k >= 0 && k <= 9) {
		rec[k] = r;
	}
	else if (k == 10) {
		if (r < rec[9]) {
			int j = 8;
			while (j >= 0 && r < rec[j])
				--j;
			++j;
			for (int i = 9; i > j; --i)
				rec[i] = rec[i - 1];
			rec[j] = r;
		}
	}
	FILE* f = fopen("../bin/record.dat", "wb");
	fseek(f, 0, SEEK_SET);
	fwrite(rec, sizeof(rec), 1, f);
	fclose(f);
}


void Functions::close()
{
	starttext.close();
	icontext.close();
	modetext.close();
	recordtext.close();
	counttext.close();
	gradetext.close();
	hinttext.close();
	rectext.close();
	quittext.close();
}