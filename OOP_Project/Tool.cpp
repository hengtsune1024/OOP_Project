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
//{ 1,1,1,1,1,1,1}  { 0,0,0,0,0,0,0 }
Tool::Tool(SDL_Renderer* renderer) : car1tool{ 0,0,0,0,0,0,1 }, car2tool{ 0,0,0,0,0,0,1 },
	tool1img("../images/star.png", renderer), tool2img("../images/star.png", renderer), tool3img("../images/star.png", renderer), 
	tool4img("../images/star.png", renderer), tool5img("../images/star.png", renderer), tool6img("../images/star.png", renderer),
	BlenderObject("../images/tool/tool.txt", "../images/tool/tool.bmp", 750, NUM_TOOL)
{
	gettime = SDL_GetTicks64() - STAIN_INTERVAL;
}

int Tool::getNearestTool(int startpos)
{
	for (int i = 0; i < NUM_TOOL; ++i) {
		if (startpos - objectList[i].index <= 0) {
			if (i == 0)
				return 0;
			else if (objectList[i].index + objectList[i - 1].index < 2 * startpos) {
				return i;
			}
			else
				return i - 1;
		}
	}
	return 0;
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
	SDL_Rect loc1 = { 345,11,43,35 };
	SDL_Rect loc2 = { 385,11,43,35 };
	SDL_Rect loc3 = { 425,11,43,35 };
	SDL_Rect loc4 = { 465,11,43,35 };
	SDL_Rect loc5 = { 505,11,43,35 };
	SDL_Rect loc6 = { 545,11,43,35 };

	if (car) {
		//car1
		if (car1tool.Tool1)
			tool1img.draw(renderer, NULL, &loc1);
		if (car1tool.Tool2)
			tool2img.draw(renderer, NULL, &loc2);
		if (car1tool.Tool3)
			tool2img.draw(renderer, NULL, &loc3);
		if (car1tool.Tool4)
			tool2img.draw(renderer, NULL, &loc4);
		if (car1tool.Tool5)
			tool2img.draw(renderer, NULL, &loc5);
		if (car1tool.Tool6)
			tool2img.draw(renderer, NULL, &loc6);
	}
	else {
		//car2
		if (car2tool.Tool1)
			tool1img.draw(renderer, NULL, &loc1);
		if (car2tool.Tool2)
			tool2img.draw(renderer, NULL, &loc2);
		if (car2tool.Tool3)
			tool2img.draw(renderer, NULL, &loc3);
		if (car2tool.Tool4)
			tool2img.draw(renderer, NULL, &loc4);
		if (car2tool.Tool5)
			tool2img.draw(renderer, NULL, &loc5);
		if (car2tool.Tool6)
			tool2img.draw(renderer, NULL, &loc6);
	}
	/*
	if (SDL_GetTicks64() - gettime < 3000)
		shownflag = false;
	else
		shownflag = true;*/
}

void Tool::getTools(bool car, int ind) 
{
	srand(std::time(NULL));
	int num = rand() % 100;
	if (num < 96)
		num %= 6;
	else
		num = 6;
	if (objectList[ind].shownflag)
	{
		gettime = SDL_GetTicks64();
		while (num != -1)
		{
			if (car) {
				if (!checktoolset(car1tool) || num == 6)
				{
					car1tool.Tool7 = 1;
					num = -1;
				}
				else 
				{
					switch (num)
					{
					case SPEEDUP:
						if (!car1tool.Tool1)
						{
							car1tool.Tool1 = 1;
							num = -1;
						}
						else
							num++;
						break;
					case INVINCIBLE:
						if (!car1tool.Tool2)
						{
							car1tool.Tool2 = 1;
							num = -1;
						}
						else
							num++;
						break;
					case HEALING:
						if (!car1tool.Tool3)
						{
							car1tool.Tool3 = 1;
							num = -1;
						}
						else
							num++;
						break;
					case NAVIGATION:
						if (!car1tool.Tool4)
						{
							car1tool.Tool4 = 1;
							num = -1;
						}
						else
							num++;
						break;
					case LIGHTNING:
						if (!car1tool.Tool5)
						{
							car1tool.Tool5 = 1;
							num = -1;
						}
						else
							num++;
						break;
					case GHOST:
						if (!car1tool.Tool6)
						{
							car1tool.Tool6 = 1;
							num = -1;
						}
						else
							num = 0;
						break;

					}
				}
			}
			else {
				if (!checktoolset(car2tool) || num == 6)
				{
					car2tool.Tool7 = 1;
					num = -1;
				}
				else
				{
					switch (num)
					{
					case SPEEDUP:
						if (!car2tool.Tool1)
						{
							car2tool.Tool1 = 1;
							num = -1;
						}
						else
							num++;
						break;
					case INVINCIBLE:
						if (!car2tool.Tool2)
						{
							car2tool.Tool2 = 1;
							num = -1;
						}
						else
							num++;
						break;
					case HEALING:
						if (!car2tool.Tool3)
						{
							car2tool.Tool3 = 1;
							num = -1;
						}
						else
							num++;
						break;
					case NAVIGATION:
						if (!car2tool.Tool4)
						{
							car2tool.Tool4 = 1;
							num = -1;
						}
						else
							num++;
						break;
					case LIGHTNING:
						if (!car2tool.Tool5)
						{
							car2tool.Tool5 = 1;
							num = -1;
						}
						else
							num++;
						break;
					case GHOST:
						if (!car2tool.Tool6)
						{
							car2tool.Tool6 = 1;
							num = -1;
						}
						else
							num = 0;
						break;
					}
				}
			}
		}
		objectList[ind].shownflag = false;
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
			case HEALING:
				if (car1tool.Tool3)
				{
					car1tool.Tool3 = 0;
					return HEALING;
				}
				break;
			case NAVIGATION:
				if (car1tool.Tool4)
				{
					car1tool.Tool4 = 0;
					return NAVIGATION;
				}
				break;
			case LIGHTNING:
				if (car1tool.Tool5)
				{
					car1tool.Tool5 = 0;
					return LIGHTNING;
				}
				break;
			case GHOST:
				if (car1tool.Tool6)
				{
					car1tool.Tool6 = 0;
					return GHOST;
				}
				break;
			case SWITCH:
				if (car1tool.Tool7)
				{
					car1tool.Tool7 = 0;
					return SWITCH;
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
		case HEALING:
			if (car2tool.Tool3)
			{
				car2tool.Tool3 = 0;
				return HEALING;
			}
			break;
		case NAVIGATION:
			if (car2tool.Tool4)
			{
				car2tool.Tool4 = 0;
				return NAVIGATION;
			}
			break;
		case LIGHTNING:
			if (car2tool.Tool5)
			{
				car2tool.Tool5 = 0;
				return LIGHTNING;
			}
			break;
		case GHOST:
			if (car2tool.Tool6)
			{
				car2tool.Tool6 = 0;
				return GHOST;
			}
			break;
		case SWITCH:
			if (car2tool.Tool7)
			{
				car2tool.Tool7 = 0;
				return SWITCH;
			}
			break;
		}
	}
	return -1;
}

int Tool::checktoolset(ToolSet set)
{
	if (!set.Tool1 || !set.Tool2 || !set.Tool3 || !set.Tool4 || !set.Tool5 || !set.Tool6)
		return 1;
	else
		return 0;
}

void Tool::getalltools(bool car)
{
	if (car)
	{
		car1tool.Tool1 = 1;
		car1tool.Tool2 = 1;
		car1tool.Tool3 = 1;
		car1tool.Tool4 = 1;
		car1tool.Tool5 = 1;
		car1tool.Tool6 = 1;
	}
	else
	{
		car2tool.Tool1 = 1;
		car2tool.Tool2 = 1;
		car2tool.Tool3 = 1;
		car2tool.Tool4 = 1;
		car2tool.Tool5 = 1;
		car2tool.Tool6 = 1;
	}
}