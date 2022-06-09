#include "Tool.h"
Tool::Tool():BlenderObject("../images/tool/tool.txt", "../images/tool/tool.bmp", 100) {

}
Tool::~Tool() {
}

void Tool::close() {
	tool1img.close();
	tool2img.close();
}
Tool::Tool(SDL_Renderer* renderer) : Tool1(0), Tool2(0), shownflag(true),
	tool1img("../images/star.png", renderer), tool2img("../images/star.png", renderer),
	BlenderObject("../images/tool/tool.txt", "../images/tool/tool.bmp", 750)
{
	gettime = SDL_GetTicks64() - STAIN_INTERVAL;
}

void Tool::setTool(Line* line) {
	position = { line->getx(),line->gety() + 1500,line->getz(),0,0,0 };
	//setEntity(line);
	//setPos({ line->getx(),line->gety() + 100 + CUBE_SIZE,line->getz(),0,0,0 });
}
/*
void Trap::draw(SDL_Renderer* renderer, Line *line) {

	drawImg(renderer, line);
}
*/
void Tool::draw3D(Point3D pos, double camDeg, double camDepth, Engine* engine, bool& clean, double maxy) {
	if (shownflag){
		BlenderObject_draw(pos, rotation, camDeg, camDepth, engine, clean, maxy);
		clean = false;
	}
}


void Tool::drawmytool(SDL_Renderer* renderer) {
	SDL_Rect loc1 = { 13,73,60,50 };
	SDL_Rect loc2 = { 53,73,60,50 };
	
	if (Tool1)
		tool1img.draw(renderer, NULL, &loc1);
	if (Tool2)
		tool2img.draw(renderer, NULL, &loc2);

	if (SDL_GetTicks64() - gettime < 3000)
		shownflag = false;
	else
		shownflag = true;
}

void Tool::getTools() {

	srand(std::time(NULL));
	if (shownflag)
	{
		gettime = SDL_GetTicks64();
		switch (rand() % 2)
		{
		case SPEEDUP:
			if (!Tool1)
				Tool1 = 1;
			else
				Tool2 = 1;
			break;
		case INVINCIBLE:
			if (!Tool2)
				Tool2 = 1;
			else
				Tool1 = 1;
			break;
		}
	}
}

void Tool::logic()
{
	rotation.y += 0.1;
	if (rotation.y > PI * 2)
		rotation.y -= PI * 2;
}


int Tool::usetool(ToolType type) {
	switch (type)
	{
	case SPEEDUP:
		if (Tool1)
		{
			Tool1 = 0;
			return SPEEDUP;
		}
		break;
	case INVINCIBLE:
		if (Tool2)
		{
			Tool2 = 0;
			return INVINCIBLE;
		}
		break;
	}
	return -1;
}