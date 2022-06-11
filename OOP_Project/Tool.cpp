#include "Tool.h"
Tool::Tool(): BlenderObject("../images/tool/tool.txt", "../images/tool/tool.bmp", 750, NUM_TOOL)
{
	gettime = SDL_GetTicks64() - STAIN_INTERVAL;
}
Tool::~Tool() {
}

void Tool::close() {
	BlenderObject::close();
	tool1img.close();
	tool2img.close();
}

Tool::Tool(SDL_Renderer* renderer) : car1tool{ 0,0 }, car2tool{ 0,0 },
	tool1img("../images/star.png", renderer), tool2img("../images/star.png", renderer),
	BlenderObject("../images/tool/tool.txt", "../images/tool/tool.bmp", 750, NUM_TOOL)
{
	gettime = SDL_GetTicks64() - STAIN_INTERVAL;
}

void Tool::setTool(Line* line, int lineindex, int ind) {
	objectList[ind].position = { line->getx(),line->gety() + 1500,line->getz(),0,0,0 };
	objectList[ind].index = lineindex;
}

void Tool::draw3D(Point3D pos, double camDeg, double camDepth, Engine* engine, bool& clean, int ind, double maxy) {
	if (objectList[ind].shownflag){
		BlenderObject_draw(pos, objectList[ind].rotation, camDeg, camDepth, engine, clean, maxy, ind);
		clean = false;
	}
}


void Tool::drawmytool(SDL_Renderer* renderer, bool car) {
	SDL_Rect loc1 = { 13,73,60,50 };
	SDL_Rect loc2 = { 53,73,60,50 };

	if (car) {
		//car1
		if (car1tool.Tool1)
			tool1img.draw(renderer, NULL, &loc1);
		if (car1tool.Tool2)
			tool2img.draw(renderer, NULL, &loc2);
	}
	else {
		//car2
		if (car2tool.Tool1)
			tool1img.draw(renderer, NULL, &loc1);
		if (car2tool.Tool2)
			tool2img.draw(renderer, NULL, &loc2);
	}
	/*
	if (SDL_GetTicks64() - gettime < 3000)
		shownflag = false;
	else
		shownflag = true;*/
}

void Tool::getTools(bool car, int startpos) 
{
	srand(std::time(NULL));
	int index;
	for (int i = 0; i < NUM_TOOL; ++i) {
		if (startpos - objectList[i].index <= 0) {
			if (i == 0)
				index = 0;
			else if (objectList[i].index + objectList[i - 1].index < 2 * startpos) {
				index = i;
			}
			else
				index = i - 1;
			break;
		}
	}

	if (objectList[index].shownflag)
	{
		gettime = SDL_GetTicks64();
		if (car) {
			switch (rand() % 2)
			{
				case SPEEDUP:
					if (!car1tool.Tool1)
						car1tool.Tool1 = 1;
					else
						car1tool.Tool2 = 1;
					break;
				case INVINCIBLE:
					if (!car1tool.Tool2)
						car1tool.Tool2 = 1;
					else
						car1tool.Tool1 = 1;
					break;
			}
		}
		else {
			switch (rand() % 2)
			{
				case SPEEDUP:
					if (!car2tool.Tool1)
						car2tool.Tool1 = 1;
					else
						car2tool.Tool2 = 1;
					break;
				case INVINCIBLE:
					if (!car2tool.Tool2)
						car2tool.Tool2 = 1;
					else
						car2tool.Tool1 = 1;
					break;
			}
		}
		objectList[index].shownflag = false;
	}
}

void Tool::logic()
{
	for (int i = 0; i < NUM_TOOL; ++i) {
		objectList[i].rotation.y += 0.1;
		if (objectList[i].rotation.y > PI * 2)
			objectList[i].rotation.y -= PI * 2;
	}
}


int Tool::usetool(ToolType type, bool car) {
	if (car) {
		switch (type)
		{
			case SPEEDUP:
				if (car1tool.Tool1)
				{
					car1tool.Tool1 = 0;
					return SPEEDUP;
				}
				break;
			case INVINCIBLE:
				if (car1tool.Tool2)
				{
					car1tool.Tool2 = 0;
					return INVINCIBLE;
				}
				break;
		}
	}
	else{
		switch (type)
		{
			case SPEEDUP:
				if (car2tool.Tool1)
				{
					car2tool.Tool1 = 0;
					return SPEEDUP;
				}
				break;
			case INVINCIBLE:
				if (car2tool.Tool2)
				{
					car2tool.Tool2 = 0;
					return INVINCIBLE;
				}
				break;
		}
	}
	return -1;
}