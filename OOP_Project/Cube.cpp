#include "Cube.h"

Cube::Cube(const char* objectFile, const char* texFile)
{
	rotation = { 0,0,0,0,0 };
	
	// front
	triangles.push_back(new Triangle({ -CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 0, 0 }, { CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 1, 0 }, { -CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 0, 1 }));
	triangles.push_back(new Triangle({ -CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 0, 1 }, { CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 1, 0 }, { CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 1, 1 }));
	// back
	triangles.push_back(new Triangle({ CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 1, 0 }, { -CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 0, 0 }, { -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 0, 1 }));
	triangles.push_back(new Triangle({ CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 1, 0 }, { -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 0, 1 }, { CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 1, 1 }));
	// left
	triangles.push_back(new Triangle({ -CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 0, 1 }, { CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 1, 1 }, { -CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 0, 0 }));
	triangles.push_back(new Triangle({ -CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 0, 0 }, { CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 1, 1 }, { CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 1, 0 }));
	// right
	triangles.push_back(new Triangle({ -CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 0, 0 }, { CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 1, 0 }, { CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 1, 1 }));
	triangles.push_back(new Triangle({ -CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 0, 0 }, { CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 1, 1 }, { -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 0, 1 }));
	// top
	triangles.push_back(new Triangle({ -CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 0, 1 }, { -CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 0, 0 }, { -CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 1, 0 }));
	triangles.push_back(new Triangle({ -CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 0, 1 }, { -CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 1, 0 }, { -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 1, 1 }));
	// bottom
	triangles.push_back(new Triangle({ CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 0, 0 }, { CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 0, 1 }, { CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 1, 0 }));
	triangles.push_back(new Triangle({ CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 0, 1 }, { CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 1, 1 }, { CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 1, 0 }));
	if (objectFile != NULL)
		strcpy_s(objFile, objectFile);
	//this->scale = scale;

	position = { 0,2000,6000 };
	rotation = { 0,0,0 };

	img.surface = SDL_LoadBMP(texFile);
	if (img.surface == NULL) {
		printf("error");
		exit(1);
	}
	img.pixels = (Uint32*)img.surface->pixels;
	img.width = img.surface->w;
	img.height = img.surface->h;
}
Cube::~Cube() 
{
	for (int i = 0; i < 12; ++i)
		delete[]triangles[i];
	SDL_FreeSurface(img.surface);
}

void Cube::draw(SDL_Renderer* renderer,Point3D pos, double camDeg, double camDepth, Engine* engine) 
{
	Uint32* bmp = engine->getPixels();
	memset(bmp, 0, WIDTH * HEIGHT * 4);
	memset(engine->getZBuffer(), 0, HEIGHT * WIDTH * sizeof(double));

	std::vector<Triangle*> allTriangles;
	for (int i = 0; i < triangles.size(); ++i) {
		triangles[i]->calculateWorldPoints(rotation, position, engine);
		triangles[i]->calculateCameraPoints(pos,camDeg, engine);
		std::vector<Triangle*> clippedTriangles = triangles[i]->GetZClippedTriangles();
		for (int j = 0; j < clippedTriangles.size(); ++j) {
			allTriangles.push_back(clippedTriangles[j]);
		}
		//allTriangles.push_back(triangles[i]);
	}

	//std::sort(allTriangles.begin(), allTriangles.end(), Triangle::compare);


	for (int i = 0; i < allTriangles.size(); ++i) {
		allTriangles[i]->calculateDrawPoints(rotation, position, camDepth, engine);
		if (allTriangles[i]->getNormalZ() < 0) {
			std::vector<Triangle*> clippedTriangles = allTriangles[i]->GetClippedTriangles();
			for (int j = 0; j < clippedTriangles.size(); ++j) {
				clippedTriangles[j]->draw(bmp, img,engine->getZBuffer());
				delete clippedTriangles[j];
			}
			delete allTriangles[i];
		}
	}
	img.texture = SDL_CreateTextureFromSurface(renderer, engine->getSurface());

	if (img.texture == NULL) {
		std::cout << SDL_GetError() << std::endl;
		exit(1);
	}
	//SDL_RenderCopyEx(renderer, img.texture, NULL, NULL,0,NULL,SDL_FLIP_VERTICAL);
	SDL_RenderCopy(renderer, img.texture, NULL, NULL);
	SDL_DestroyTexture(img.texture);
}

/*
	// front
	triangles.push_back(new Triangle({ -CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 0, 0 }, { -CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 0, 1 }, { CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 1, 0 }));
	triangles.push_back(new Triangle({ -CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 0, 1 }, { CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 1, 1 }, { CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 1, 0 }));
	// back
	triangles.push_back(new Triangle({ CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 1, 0 }, { -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 0, 1 }, { -CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 0, 0 }));
	triangles.push_back(new Triangle({ CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 1, 0 }, { CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 1, 1 }, { -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 0, 1 }));
	// left
	triangles.push_back(new Triangle({ -CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 0, 1 }, { -CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 0, 0 }, { CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 1, 1 }));
	triangles.push_back(new Triangle({ -CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 0, 0 }, { CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 1, 0 }, { CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 1, 1 }));
	// right
	triangles.push_back(new Triangle({ -CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 0, 0 }, { CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 1, 1 }, { CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 1, 0 }));
	triangles.push_back(new Triangle({ -CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 0, 0 }, { -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 0, 1 }, { CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 1, 1 }));
	// top
	triangles.push_back(new Triangle({ -CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 0, 1 }, { -CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 1, 0 }, { -CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 0, 0 }));
	triangles.push_back(new Triangle({ -CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 0, 1 }, { -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 1, 1 }, { -CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 1, 0 }));
	// bottom
	triangles.push_back(new Triangle({ CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 0, 0 }, { CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 1, 0 }, { CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 0, 1 }));
	triangles.push_back(new Triangle({ CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 0, 1 }, { CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 1, 0 }, { CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 1, 1 }));
	*/