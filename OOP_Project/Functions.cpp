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
endtext("WIN", "../fonts/akabara-cinderella.ttf", 100, 0x00, { 255, 255, 255 }, BLENDED, { NULL, NULL, NULL }, window.GetRenderer(), { 260, 175 }, { NULL, NULL }, NULL, SDL_FLIP_NONE, 255),
gradetext("GRADE", "../fonts/akabara-cinderella.ttf", 75, 0x00, { 255, 255, 255 }, BLENDED, { NULL, NULL, NULL }, window.GetRenderer(), { 470, 10 }, { NULL, NULL }, NULL, SDL_FLIP_NONE, 255),
hinttext("Click to continue", "../fonts/akabara-cinderella.ttf", 50, 0x00, { 211, 211, 211 }, BLENDED, { NULL, NULL, NULL }, window.GetRenderer(), { 390, 500 }, { NULL, NULL }, NULL, SDL_FLIP_NONE, 255),
rec1text(" ", "../fonts/akabara-cinderella.ttf", 100, 0x00, { 255, 0, 0 }, BLENDED, { NULL, NULL, NULL }, window.GetRenderer(), { 250, 500 }, { NULL, NULL }, NULL, SDL_FLIP_NONE, 255),
rec2text(" ", "../fonts/akabara-cinderella.ttf", 100, 0x00, { 0, 255, 0 }, BLENDED, { NULL, NULL, NULL }, window.GetRenderer(), { 250, 500 }, { NULL, NULL }, NULL, SDL_FLIP_NONE, 255),
rec3text(" ", "../fonts/akabara-cinderella.ttf", 100, 0x00, { 0, 255, 255 }, BLENDED, { NULL, NULL, NULL }, window.GetRenderer(), { 250, 500 }, { NULL, NULL }, NULL, SDL_FLIP_NONE, 255)

{
	FILE* f = fopen("../records/record.dat", "rb");
	fseek(f, 0, SEEK_SET);
	fread(rec, sizeof(rec), 1, f);
	fclose(f);
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
					Record();
				}
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

void Functions::Record()
{
	bool quit = false;
	SDL_Event e;
	int record = rec[0];
	int ms, sec, min;
	if (record != 0)
	{
		ms = record % 1000;
		record /= 1000;
		sec = record % 60;
		record -= sec;
		min = record / 60;
		sprintf_s(recordstr, "1.  %02d : %02d : %03d", min, sec, ms);
	}
	else 
	{
		sprintf_s(recordstr, "1.  -- : -- : ---");
	}
	rec1text.close();
	rec1text.setString(recordstr);
	rec1text.setPos({ 200, 50 });
	rec1text.generateTexture();
	rec1text.draw();

	record = rec[1];
	if (record != 0)
	{
		ms = record % 1000;
		record /= 1000;
		sec = record % 60;
		record -= sec;
		min = record / 60;
		sprintf_s(recordstr, "1.  %02d : %02d : %03d", min, sec, ms);
	}
	else
	{
		sprintf_s(recordstr, "1.  -- : -- : ---");
	}
	rec2text.close();
	rec2text.setString(recordstr);
	rec2text.setPos({ 200, 200 });
	rec2text.generateTexture();
	rec2text.draw();

	record = rec[2];
	if (record != 0)
	{
		ms = record % 1000;
		record /= 1000;
		sec = record % 60;
		record -= sec;
		min = record / 60;
		sprintf_s(recordstr, "1.  %02d : %02d : %03d", min, sec, ms);
	}
	else
	{
		sprintf_s(recordstr, "1.  -- : -- : ---");
	}
	rec3text.close();
	rec3text.setString(recordstr);
	rec3text.setPos({ 200, 350 });
	rec3text.generateTexture();
	rec3text.draw();

	hinttext.draw();
	window.display();

	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
			{
				exit(0);
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
	if (r <= rec[0] || rec[0] == 0)
	{
		rec[2] = rec[1];
		rec[1] = rec[0];
		rec[0] = r;
	}
	else if (r <= rec[1] || rec[1] == 0)
	{
		rec[2] = rec[1];
		rec[1] = r;
	}
	else if (r < rec[2] || rec[2] == 0)
	{
		rec[2] = r;
	}
	FILE* f = fopen("../records/record.dat", "wb");
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
	rec1text.close();
}