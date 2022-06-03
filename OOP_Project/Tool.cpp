#include "Tool.h"
Tool::Tool() {

}
Tool::~Tool() {

}
Tool::Tool(const char* path, int n, SDL_Renderer* renderer) : Tool1(0), Tool2(0),
	tool1img("../images/star.png", renderer), tool2img("../images/star.png", renderer), cube()
{
	gettime = SDL_GetTicks64() - STAIN_INTERVAL;
	setImage(path, n, renderer, 0);
}

void Tool::setTool(Line* line) {
	setEntity(line);
}
/*
void Trap::draw(SDL_Renderer* renderer, Line *line) {

	drawImg(renderer, line);
}
*/
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