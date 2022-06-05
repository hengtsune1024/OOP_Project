#pragma once
#include <SDL.h> // Using SDL

class Text
{
public:
	Text();
	Text(const char* str, const char* fontPath, int fontSize, int style, SDL_Color fgColor, int textType, SDL_Color bgColor, SDL_Renderer* renderer, SDL_Point pos, SDL_Point center, double angle, SDL_RendererFlip flip, int alpha);
	void generateTexture();
	void close();
	void draw();
	void draw(SDL_Renderer* renderer, SDL_Point pos, SDL_Point center, double angle, SDL_RendererFlip flip, int alpha);

	int getWidth();
	int getHeight();
	void setString(const char* s);
	void setFontPath(const char* f);
	void setFontSize(int s);
	void setStyle(int s);
	void setFgColor(SDL_Color fc);
	void setTextType(int t);
	void setBgColor(SDL_Color bc);
	void setRenderer(SDL_Renderer* r);

	void setPos(SDL_Point p);
	void setCenterAngle(SDL_Point c, double a);
	void setFlip(SDL_RendererFlip f);
	void setAlpha(int a);

private:
	// texture data member
	SDL_Texture* texture;
	int width;
	int height;

	// font data member
	char* str;
	char* fontPath;
	int fontSize;
	int style;
	SDL_Color fgColor;
	int textType;
	SDL_Color bgColor;
	SDL_Renderer* renderer;

	// Display data member
	SDL_Point pos;
	SDL_Point center;
	double angle;
	SDL_RendererFlip flip;
	int alpha;
};