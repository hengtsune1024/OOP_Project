#include "Tool.h"
Tool::Tool():toolBlock("../images/tool/tool.txt", "../images/tool/tool.bmp", 100) {

}
Tool::~Tool() {
}

void Tool::close() {
	tool1img.close();
	tool2img.close();
	removeTimer();
}
Tool::Tool(SDL_Renderer* renderer) : Tool1(0), Tool2(0), shownflag(true),
	tool1img("../images/star.png", renderer), tool2img("../images/star.png", renderer),
	toolBlock("../images/tool/tool.txt", "../images/tool/tool.bmp", 750)
{
	gettime = SDL_GetTicks64() - STAIN_INTERVAL;
}

void Tool::setTool(Line* line) {
	toolBlock.setPos({ line->getx(),line->gety() + 1500,line->getz(),0,0,0 });
	//setEntity(line);
	//setPos({ line->getx(),line->gety() + 100 + CUBE_SIZE,line->getz(),0,0,0 });
}
/*
void Trap::draw(SDL_Renderer* renderer, Line *line) {

	drawImg(renderer, line);
}
*/
void Tool::draw(Point3D pos, double camDeg, double camDepth, Engine* engine, bool clean, double maxy) {
	if (shownflag)
		toolBlock.draw(pos, toolBlock.getRotation(), camDeg, camDepth, engine, clean, maxy);
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

Uint32 Tool::changeData(Uint32 interval, void* para) 
{
	Tool* tool = (Tool*)para;
	double ry = tool->toolBlock.getRotY() + 0.1;
	if (ry > 3.1415926535 * 2)
		ry -= 3.1415926535 * 2;
	tool->toolBlock.setRotation({ 0,ry,0 });
	return interval;
}

void Tool::startTimer() {
	timer = SDL_AddTimer(50, changeData, this);
}

void Tool::removeTimer() {
	SDL_RemoveTimer(timer);
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