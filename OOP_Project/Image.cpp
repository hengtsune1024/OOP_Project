#include "Image.h"

Image::Image(const char* path, SDL_Renderer* renderer)
{
	SDL_Surface* imgSurface = IMG_Load(path);
	if (imgSurface == NULL)
	{
		cout << "[Image] SDL_LoadBMP failed: " << IMG_GetError() << endl;
		SDL_Delay(3000);
		exit(1);
	}

	texture = SDL_CreateTextureFromSurface(renderer, imgSurface);
	if (texture == NULL)
	{
		cout << "[Image] SDL_CreateTextureFromSurface failed: " << SDL_GetError() << endl;
		SDL_Delay(3000);
		exit(1);
	}
	cout << "[Image] SDL_CreateTextureFromSurface OK 0000" << endl;
	width = imgSurface->w;   // �����c�i���o�Ϥ��e��
	height = imgSurface->h;

	SDL_FreeSurface(imgSurface);

}

void Image::set(const char* path, SDL_Renderer* renderer)
{
	SDL_Surface* imgSurface = IMG_Load(path);
	if (imgSurface == NULL)
	{
		cout << "[Image] SDL_LoadBMP failed: " << IMG_GetError() << endl;
		SDL_Delay(3000);
		exit(1);
	}
	cout << "[Image] IMG_Load OK" << endl;

	SDL_SetColorKey(imgSurface, true, SDL_MapRGB(imgSurface->format, 0xff, 0xff, 0xff));

	texture = SDL_CreateTextureFromSurface(renderer, imgSurface);
	if (texture == NULL)
	{
		cout << "[Image] SDL_CreateTextureFromSurface failed: " << SDL_GetError() << endl;
		SDL_Delay(3000);
		exit(1);
	}
	cout << "[Image] SDL_CreateTextureFromSurface OK" << endl;
	width = imgSurface->w;
	height = imgSurface->h;

	SDL_FreeSurface(imgSurface);
}

void Image::close()
{
	SDL_DestroyTexture(texture);
}

void Image::draw(SDL_Renderer* renderer, SDL_Rect* src, SDL_Rect* dst)
{	/*
	SDL_Rect* d = &dst, * s = &src;

	// ALLREGION���۩w�q�`�� ��ܥ�����
	if (dst.x == ALL_REGION)
	{
		d = NULL;
	}
	if (src.x == ALL_REGION)
	{
		s = NULL;
	}
	*/
	SDL_RenderCopy(renderer, texture, src, dst);
}