#include "Image3D.h"

Image3D::Image3D() : pixels(NULL), texture(NULL), surface(NULL)
{}

Uint32 Image3D::getColor(int x, int y) 
{
	if (x >= width)
		x = width - 1;
	if (x < 0)
		x = 0;
	if (y >= height)
		y = height - 1;
	if (y < 0)
		y = 0;
	return pixels[width * y + x];
}

void Image3D::loadSurface(const char* textureFile) {
	surface = SDL_LoadBMP(textureFile);
	if (surface == NULL) {
		std::cerr << "error: surface cannot be created: " << textureFile << "\n";
		exit(1);
	}
	pixels = (Uint32*)surface->pixels;
	width = surface->w;
	height = surface->h;
}

void Image3D::close() 
{
	SDL_FreeSurface(surface);
	pixels = NULL;
}