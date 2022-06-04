#include "BlenderObject.h"

BlenderObject::BlenderObject(const char* objectFile, const char* textureFile, double scale)
{
	strcpy_s(objFile, objectFile);
	this->scale = scale;

	position = { 0,-1500,2000 };
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

void BlenderObject::Load()
{
	// Load object
	FILE* f;
	fopen_s(&f, objFile, "r");
	char sym[100], buffer[100];

	if (f != NULL)
	{
		// Load vertices and texture points
		std::vector<Point3D> vertices;
		std::vector<Point3D> texturePoints;
		double u, v, x, y, z;
		int a, b, c;
		char s;
		Point3D p0, p1, p2, p3;

		while (fscanf(f, " %s", sym) != EOF)
		{
			if (sym[0] == 'v') // 3D point
			{
				if (sym[1] == 't') {
					fscanf(f, "%lf%lf", &u, &v);
					texturePoints.push_back({ 0, 0, 0, u, v });
				}
				else if (sym[1] == '\0') {
					fscanf(f, "%lf%lf%lf", &x, &y, &z);
					vertices.push_back({ x,y,z,0,0 });
				}
				else {
					fscanf(f, " ");
					fgets(buffer, 100, f);
				}
			}
			else if (sym[0] == 'f') {
				fscanf(f, "%d%c%d%c%d", &a, &s, &b, &s, &c);
				p0 = { vertices[a - 1].x * scale, vertices[a - 1].y * scale, vertices[a - 1].z * scale, texturePoints[b - 1].u, texturePoints[b - 1].v };
				fscanf(f, "%d%c%d%c%d", &a, &s, &b, &s, &c);
				p1 = { vertices[a - 1].x * scale, vertices[a - 1].y * scale, vertices[a - 1].z * scale, texturePoints[b - 1].u, texturePoints[b - 1].v };
				fscanf(f, "%d%c%d%c%d", &a, &s, &b, &s, &c);
				p2 = {vertices[a - 1].x * scale, vertices[a - 1].y * scale, vertices[a - 1].z * scale, texturePoints[b - 1].u, texturePoints[b - 1].v};
				triangles.push_back(new Triangle(p0, p2, p1));
				if (fgetc(f) != '\n') {
					fscanf(f, "%d%c%d%c%d", &a, &s, &b, &s, &c);
					p3 = { vertices[a - 1].x * scale, vertices[a - 1].y * scale, vertices[a - 1].z * scale, texturePoints[b - 1].u, texturePoints[b - 1].v };
					triangles.push_back(new Triangle(p0, p1, p3));
				}
			}
			else {
				fscanf(f," ");
				fgets(buffer, 100, f);
			}
		}
	}
	printf("%u\n", triangles.size());
	fclose(f);
}


void BlenderObject::Logic(double elapsedTime)
{
	rotation.y += 1 * elapsedTime;
}

void BlenderObject::draw(SDL_Renderer* renderer, Point3D pos, double camDeg, double camDepth, Engine* engine, bool clean)
{
	Uint32* bmp = engine->getPixels();
	if (clean) {
		memset(bmp, 0, WIDTH * HEIGHT * 4);
		memset(engine->getZBuffer(), 0, WIDTH * HEIGHT * sizeof(double));
	}

	std::vector<Triangle*> allTriangles;
	for (int i = 0; i < triangles.size(); ++i) {
		triangles[i]->calculateWorldPoints(rotation, position, engine);
		triangles[i]->calculateCameraPoints(pos, camDeg, engine);

		std::vector<Triangle*> clippedTriangles = triangles[i]->GetZClippedTriangles();
		for (int j = 0; j < clippedTriangles.size(); ++j) {
			allTriangles.push_back(clippedTriangles[j]);
		}
		//allTriangles.push_back(faces[i]);
	}

	//std::sort(allTriangles.begin(), allTriangles.end(), Triangle::compare); 


	for (int i = 0; i < allTriangles.size(); ++i) {
		allTriangles[i]->calculateDrawPoints(rotation, position, camDepth, engine);
		if (allTriangles[i]->getNormalZ() < 0) {
			std::vector<Triangle*> clippedTriangles = allTriangles[i]->GetClippedTriangles();
			for (int j = 0; j < clippedTriangles.size(); ++j) {
				clippedTriangles[j]->draw(bmp, img, engine->getZBuffer());
				delete clippedTriangles[j];
			}
		}
		delete allTriangles[i];
	}
	/*
	img.texture = SDL_CreateTextureFromSurface(renderer, engine->getSurface());
	if (img.texture == NULL) {
		std::cout << SDL_GetError() << std::endl;
		exit(1);
	}

	SDL_RenderCopy(renderer, img.texture, NULL, NULL);
	SDL_DestroyTexture(img.texture);*/
}