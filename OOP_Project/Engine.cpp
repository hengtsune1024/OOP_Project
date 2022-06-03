#include "Engine.h"

Engine::Engine()
{
	FILE* f;
	fopen_s(&f, "../images/tmp.bmp", "ab");
	fseek(f, 54, SEEK_SET);
	Uint32 tmp = 0;
	fwrite(&tmp, sizeof(tmp), WIDTH * HEIGHT, f);
	fclose(f);

	Z0 = (WIDTH / 2.0) / tan((FIELD_OF_VIEW / 2.0) * 3.1415926535 / 180.0);
	img = SDL_LoadBMP("../images/tmp.bmp");
	pixels = (Uint32*)img->pixels;
}
Engine::~Engine()
{
	SDL_FreeSurface(img);
}
Point3D Engine::Translate(const Point3D& original, const Point3D& translation)
{
	Point3D toReturn;
	toReturn.x = original.x + translation.x;
	toReturn.y = original.y + translation.y;
	toReturn.z = original.z + translation.z;
	toReturn.u = original.u;
	toReturn.v = original.v;
	toReturn.w = original.w;
	return toReturn;
}
Point3D Engine::Rotate(const Point3D& original, Point3D rotation)
{
	Point3D toReturn;
	toReturn.x = original.x * (cos(rotation.z) * cos(rotation.y)) +
		original.y * (cos(rotation.z) * sin(rotation.y) * sin(rotation.x) - sin(rotation.z) * cos(rotation.x)) +
		original.z * (cos(rotation.z) * sin(rotation.y) * cos(rotation.x) + sin(rotation.z) * sin(rotation.x));
	toReturn.y = original.x * (sin(rotation.z) * cos(rotation.y)) +
		original.y * (sin(rotation.z) * sin(rotation.y) * sin(rotation.x) + cos(rotation.z) * cos(rotation.x)) +
		original.z * (sin(rotation.z) * sin(rotation.y) * cos(rotation.x) - cos(rotation.z) * sin(rotation.x));
	toReturn.z = original.x * (-sin(rotation.y)) +
		original.y * (cos(rotation.y) * sin(rotation.x)) +
		original.z * (cos(rotation.y) * cos(rotation.x));
	toReturn.u = original.u;
	toReturn.v = original.v;
	toReturn.w = original.w;
	return toReturn;
}
Point3D Engine::ApplyPerspective(const Point3D& original)
{
	Point3D toReturn;
	/*
	toReturn.x = original.x * Z0 / (Z0 + original.z);
	toReturn.y = original.y * Z0 / (Z0 + original.z);
	toReturn.z = original.z;
	toReturn.u = original.u * Z0 / (Z0 + original.z);
	toReturn.v = original.v * Z0 / (Z0 + original.z);
	toReturn.w = original.w * Z0 / (Z0 + original.z);*/

	toReturn.x = original.x * DEFAULT_CAMERA_DEPTH / original.z * WIDTH / 2;
	toReturn.y = original.y * DEFAULT_CAMERA_DEPTH / original.z * HEIGHT / 2;
	toReturn.z = original.z;
	toReturn.u = original.u * Z0 / (Z0 + original.z);
	toReturn.v = original.v * Z0 / (Z0 + original.z);
	toReturn.w = original.w * Z0 / (Z0 + original.z);

	return toReturn;
}
Point3D Engine::CenterScreen(const Point3D& original)
{
	Point3D toReturn;
	toReturn.x = original.x + WIDTH / 2;
	toReturn.y = -original.y + HEIGHT / 2;
	toReturn.z = original.z;
	toReturn.u = original.u;
	toReturn.v = original.v;
	toReturn.w = original.w;
	return toReturn;
}