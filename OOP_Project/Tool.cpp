#include "Tool.h"
Tool::Tool(): BlenderObject("../images/tool/tool.txt", "../images/tool/tool.bmp", 750, NUM_TOOL,1)
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
//{ 1,1,1,1,1,1}  { 0,0,0,0,0,0 }
Tool::Tool(SDL_Renderer* renderer, bool d) : car1tool{ 1,1,1,1,1,1 }, car2tool{ 1,1,1,1,1,1 }, dual(d),
	tool1img("../images/tool/mushroom.png", renderer), tool2img("../images/tool/star.png", renderer), tool3img("../images/tool/heal.png", renderer), 
	tool4img("../images/tool/ghost.png", renderer), tool5img("../images/tool/lightning.png", renderer), tool6img("../images/tool/8.png", renderer),
	BlenderObject("../images/tool/tool.txt", "../images/tool/tool.bmp", 750, NUM_TOOL, 1)
{
	gettime = SDL_GetTicks64() - STAIN_INTERVAL;
}

bool Tool::hitTool(double carx, double height, double mod, int ind) 
{
	return objectList[ind].shownflag && carx > objectList[ind].position.x - TOOL_WIDTH * mod && carx < objectList[ind].position.x + TOOL_WIDTH * mod && height < objectList[ind].position.y + 1300;
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
	SDL_Rect loc1 = { 347,11,43,35 };
	SDL_Rect loc2 = { 387,11,43,35 };
	SDL_Rect loc3 = { 427,11,43,35 };
	SDL_Rect loc4 = { 467,11,43,35 };
	SDL_Rect loc5 = { 507,11,43,35 };
	SDL_Rect loc6 = { 547,11,43,35 };
	if (!dual)
	{
		//tool column
		roundedRectangleRGBA(renderer, 350, 10, 385, 45, 1, 255, 0, 255, 255);
		roundedRectangleRGBA(renderer, 390, 10, 425, 45, 1, 255, 0, 255, 255);
		roundedRectangleRGBA(renderer, 430, 10, 465, 45, 1, 255, 0, 255, 255);
		roundedRectangleRGBA(renderer, 470, 10, 505, 45, 1, 255, 0, 255, 255);

		if (car1tool.Tool1)
			tool1img.draw(renderer, NULL, &loc1);
		if (car1tool.Tool2)
			tool2img.draw(renderer, NULL, &loc2);
		if (car1tool.Tool3)
			tool3img.draw(renderer, NULL, &loc3);
		if (car1tool.Tool4)
			tool4img.draw(renderer, NULL, &loc4);
	}
	else
	{
		roundedRectangleRGBA(renderer, 350, 10, 385, 45, 1, 255, 0, 255, 255);
		roundedRectangleRGBA(renderer, 390, 10, 425, 45, 1, 255, 0, 255, 255);
		roundedRectangleRGBA(renderer, 430, 10, 465, 45, 1, 255, 0, 255, 255);
		roundedRectangleRGBA(renderer, 470, 10, 505, 45, 1, 255, 0, 255, 255);
		roundedRectangleRGBA(renderer, 510, 10, 545, 45, 1, 255, 0, 255, 255);
		roundedRectangleRGBA(renderer, 550, 10, 585, 45, 1, 0, 255, 255, 255);
		if (car) {
			//car1

			if (car1tool.Tool1)
				tool1img.draw(renderer, NULL, &loc1);
			if (car1tool.Tool2)
				tool2img.draw(renderer, NULL, &loc2);
			if (car1tool.Tool3)
				tool3img.draw(renderer, NULL, &loc3);
			if (car1tool.Tool4)
				tool4img.draw(renderer, NULL, &loc4);
			if (car1tool.Tool5)
				tool5img.draw(renderer, NULL, &loc5);
			if (car1tool.Tool6)
				tool6img.draw(renderer, NULL, &loc6);
		}
		else {
			//car2
			if (car2tool.Tool1)
				tool1img.draw(renderer, NULL, &loc1);
			if (car2tool.Tool2)
				tool2img.draw(renderer, NULL, &loc2);
			if (car2tool.Tool3)
				tool3img.draw(renderer, NULL, &loc3);
			if (car2tool.Tool4)
				tool4img.draw(renderer, NULL, &loc4);
			if (car2tool.Tool5)
				tool5img.draw(renderer, NULL, &loc5);
			if (car2tool.Tool6)
				tool6img.draw(renderer, NULL, &loc6);
		}
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
	int num = rand() % 51;
	if (num < 50)
		num %= 5;
	else
		num = 5;
	if (!dual)
		num = rand() % 4;
	if (objectList[ind].shownflag)
	{
		gettime = SDL_GetTicks64();
		while (num != -1)
		{
			if (car) {
				if (!checktoolset(car1tool) || num == 5)
				{
					car1tool.Tool6 = 1;
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
					case GHOST:
						if (!car1tool.Tool4)
						{
							car1tool.Tool4 = 1;
							num = -1;
						}
						else
							num = 0;
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


					}
				}
			}
			else {
				if (!checktoolset(car2tool) || num == 5)
				{
					car2tool.Tool6 = 1;
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
					case GHOST:
						if (!car2tool.Tool4)
						{
							car2tool.Tool4 = 1;
							num = -1;
						}
						else
							num = 0;
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

					}
				}
			}
		}
		objectList[ind].shownflag = false;
	}
}

void Tool::logic(void*, void*)
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
			case GHOST:
				if (car1tool.Tool4)
				{
					car1tool.Tool4 = 0;
					return GHOST;
				}
				break;
			case LIGHTNING:
				if (car1tool.Tool5)
				{
					car1tool.Tool5 = 0;
					return LIGHTNING;
				}
				break;

			case SWITCH:
				if (car1tool.Tool6)
				{
					car1tool.Tool6 = 0;
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
		case GHOST:
			if (car2tool.Tool4)
			{
				car2tool.Tool4 = 0;
				return GHOST;
			}
			break;
		case LIGHTNING:
			if (car2tool.Tool5)
			{
				car2tool.Tool5 = 0;
				return LIGHTNING;
			}
			break;

		case SWITCH:
			if (car2tool.Tool6)
			{
				car2tool.Tool6 = 0;
				return SWITCH;
			}
			break;
		}
	}
	return -1;
}

int Tool::checktoolset(ToolSet set)
{
	if (!set.Tool1 || !set.Tool2 || !set.Tool3 || !set.Tool4 || !set.Tool5)
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
	}
	else
	{
		car2tool.Tool1 = 1;
		car2tool.Tool2 = 1;
		car2tool.Tool3 = 1;
		car2tool.Tool4 = 1;
		car2tool.Tool5 = 1;
	}
}