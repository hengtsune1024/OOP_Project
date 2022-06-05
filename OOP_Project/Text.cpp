#include "Text.h"
#include <stdio.h>
#include <SDL.h> // Using SDL
#include <string.h>
#include "constants.h"
#include "SDL_ttf.h"


Text::Text()
{
}

// A structure that represents a color.
// https://wiki.libsdl.org/SDL_Color

Text::Text(const char* str, const char* fontPath, int fontSize, int style, SDL_Color fgColor, int textType, SDL_Color bgColor, SDL_Renderer* renderer, SDL_Point pos, SDL_Point center, double angle, SDL_RendererFlip flip, int alpha)
{
	setString(str);
	setFontPath(fontPath);
	setFontSize(fontSize);
	setStyle(style);
	setFgColor(fgColor);
	setTextType(textType);
	setBgColor(bgColor);
	setRenderer(renderer);

	generateTexture();

	setPos(pos);
	setCenterAngle(center, angle);
	setFlip(flip);
	setAlpha(alpha);
}

void Text::generateTexture()
{
	// TTF Font sturct	
	// https://www.libsdl.org/projects/docs/SDL_ttf/SDL_ttf_49.html#SEC49
	TTF_Font* ttfFont = NULL;

	// Open the font	
	// https://www.libsdl.org/projects/docs/SDL_ttf/SDL_ttf_14.html#SEC14
	ttfFont = TTF_OpenFont(fontPath, fontSize); //("../fonts/lazy.ttf", 28);	
	if (ttfFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		exit(0);
	}


	// Set style of font including TTF_STYLE_BOLD | TTF_STYLE_ITALIC | TTF_STYLE_UNDERLINE | TTF_STYLE_NORMAL
	// https://www.libsdl.org/projects/docs/SDL_ttf/SDL_ttf_22.html#SEC22	
	TTF_SetFontStyle(ttfFont, style);

	// Render text surface
	// The way SDL_ttf works is that you create a new image from a font and color. 
	// We're going to be loading our image from text rendered by SDL_ttf instead of a file.

	SDL_Surface* surface = NULL;

	// Creates a solid/shaded/blended color surface from the font, text, and color given.
	// https://www.libsdl.org/projects/docs/SDL_ttf/SDL_ttf_35.html
	if (textType == SOLID)
	{
		// Quick and Dirty
		surface = TTF_RenderText_Solid(ttfFont, str, fgColor);
	}
	else if (textType == SHADED)
	{
		// Slow and Nice, but with a Solid Box
		surface = TTF_RenderText_Shaded(ttfFont, str, fgColor, bgColor);
	}

	else if (textType == BLENDED)
	{
		// Slow Slow Slow, but Ultra Nice over another image
		surface = TTF_RenderUTF8_Blended(ttfFont, str, fgColor);


	}

	// Free font
	// https://www.libsdl.org/projects/docs/SDL_ttf/SDL_ttf_18.html#SEC18
	TTF_CloseFont(ttfFont);

	if (surface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		exit(0);
	}
	else
	{
		// Create texture from surface pixels
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (texture == NULL)
		{
			printf("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
			exit(0);
		}

		//Get text dimensions and information
		width = surface->w;
		height = surface->h;

		// Get rid of old loaded surface
		SDL_FreeSurface(surface);
	}
}

void Text::close()
{
	// Free loaded image	
	SDL_DestroyTexture(texture);
}

void Text::draw()
{
	SDL_Rect d;
	d.x = pos.x;
	d.y = pos.y;
	d.w = width;
	d.h = height;


	// Set and enable standard alpha blending mode for a texture	
	if (SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND) == -1)
	{
		printf("SDL_SetTextureBlendMode failed: %s\n", SDL_GetError());
		return;
	}

	// Modulate texture alpha	
	if (SDL_SetTextureAlphaMod(texture, alpha) == -1)
	{
		printf("SDL_SetTextureAlphaMod failed: %s\n", SDL_GetError());
		return;
	}

	// Copy a portion of the texture to the current rendering target, 
	// optionally rotating it by angle around the given center and also flipping it top-bottom and/or left-right.
	// if center is NULL, rotation will be done around dstrect.w / 2, dstrect.h / 2	
	SDL_RenderCopyEx(renderer, texture, NULL, &d, angle, &center, flip);
}

void Text::draw(SDL_Renderer* renderer, SDL_Point pos, SDL_Point center, double angle, SDL_RendererFlip flip, int alpha)
{
	setPos(pos);
	setCenterAngle(center, angle);
	setFlip(flip);
	setAlpha(alpha);

	draw();
}

int Text::getWidth()
{
	return width;
}

int Text::getHeight()
{
	return height;
}

void Text::setString(const char* s)
{
	str = new char[strlen(s)];
	strcpy_s(str, strlen(s) + 1, s);
}

void Text::setFontPath(const char* f)
{
	fontPath = new char[strlen(f)];
	strcpy_s(fontPath, strlen(f) + 1, f);
}

void Text::setFontSize(int s)
{
	fontSize = s;
}

void Text::setStyle(int s)
{
	style = s;
}

void Text::setFgColor(SDL_Color fc)
{
	fgColor = fc;
}

void Text::setTextType(int t)
{
	textType = t;
}

void Text::setBgColor(SDL_Color bc)
{
	bgColor = bc;
}

void Text::setRenderer(SDL_Renderer* r)
{
	renderer = r;
}

void Text::setPos(SDL_Point p)
{
	pos = p;
}

void Text::setCenterAngle(SDL_Point c, double a)
{
	center = c;
	angle = a;
}

void Text::setFlip(SDL_RendererFlip f)
{
	flip = f;
}

void Text::setAlpha(int a)
{
	alpha = a;
}