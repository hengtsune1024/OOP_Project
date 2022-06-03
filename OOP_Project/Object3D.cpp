#include "Object3D.h"

Object3D::Object3D(const char* objectFile, const char* textureFile, double scale)
{
	strcpy_s(objFile, objectFile);
	this->scale = scale;

	position = { 0,2000,6000 };
	rotation = { 0,0,0 };
	img.surface = SDL_LoadBMP(textureFile);
	if (img.surface == NULL) {
		printf("error");
		exit(1);
	}
	img.pixels = (Uint32*)img.surface->pixels;
	img.width = img.surface->w;
	img.height = img.surface->h;
}
Object3D::~Object3D() {
	for (int i = 0; i < triangles.size(); ++i)
		delete[]triangles[i];
	SDL_FreeSurface(img.surface);
}

void Object3D::Load()
{
	
}


void Object3D::Logic(double elapsedTime)
{
	rotation.y += 1 * elapsedTime;
}

void Object3D::draw(SDL_Renderer* renderer)
{
	Uint32* bmp = engine.getPixels();
	memset(bmp, 0, WIDTH * HEIGHT * 4);

	std::vector<Triangle*> allTriangles;
	for (int i = 0; i < triangles.size(); ++i) {
		triangles[i]->calculateWorldPoints(rotation, position, &engine);
		triangles[i]->calculateCameraPoints(engine.getCamera(), &engine);

		std::vector<Triangle*> clippedTriangles = triangles[i]->GetZClippedTriangles();
		for (int j = 0; j < clippedTriangles.size(); ++j) {
			allTriangles.push_back(clippedTriangles[j]);
		}
		//allTriangles.push_back(faces[i]);
	}

	std::sort(allTriangles.begin(), allTriangles.end(), Triangle::compare);


	for (int i = 0; i < allTriangles.size(); ++i) {
		allTriangles[i]->calculateDrawPoints(rotation, position, &engine);
		if (allTriangles[i]->getNormalZ() < 0) {
			std::vector<Triangle*> clippedTriangles = allTriangles[i]->GetClippedTriangles();
			for (int j = 0; j < clippedTriangles.size(); ++j) {
				clippedTriangles[j]->draw(bmp, img);
				delete clippedTriangles[j];
			}
		}
		delete allTriangles[i];
	}
	img.texture = SDL_CreateTextureFromSurface(renderer, engine.getSurface());

	if (img.texture == NULL) {
		std::cout << SDL_GetError() << std::endl;
		exit(1);
	}

	SDL_RenderCopy(renderer, img.texture, NULL, NULL);
	SDL_DestroyTexture(img.texture);
}