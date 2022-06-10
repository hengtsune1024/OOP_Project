#include "Tool.h"
Tool::Tool():BlenderObject("../images/tool/tool.txt", "../images/tool/tool.bmp", 100, true) {

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
	BlenderObject("../images/tool/tool.txt", "../images/tool/tool.bmp", 750, true)
{
	gettime = SDL_GetTicks64() - STAIN_INTERVAL;
}

void Tool::setTool(Line* line) {
	position = { line->getx(),line->gety() + 1500,line->getz(),0,0,0 };
}

void Tool::draw3D(Point3D pos, double camDeg, double camDepth, Engine* engine, bool& clean, double maxy) {
	if (shownflag){
		BlenderObject_draw(pos, rotation, camDeg, camDepth, engine, clean, maxy);
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

	if (SDL_GetTicks64() - gettime < 3000)
		shownflag = false;
	else
		shownflag = true;
}

void Tool::getTools(bool car) {

	srand(std::time(NULL));
	if (shownflag)
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
	}
}

void Tool::logic()
{
	rotation.y += 0.1;
	if (rotation.y > PI * 2)
		rotation.y -= PI * 2;
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