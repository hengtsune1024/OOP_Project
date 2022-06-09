#include "BlenderObject.h"

BlenderObject::BlenderObject(const char* objectFile, const char* textureFile, double scale)
{
	this->scale = scale;

	position = { 0,-CAMERA_HEIGHT,CAMERA_CARMIDPOINT_DIST };
	rotation = { 0,0,0 };
	img.surface = SDL_LoadBMP(textureFile);
	if (img.surface == NULL) {
		printf("error: surface cannot be created\n");
		printf("%s", textureFile);
		exit(1);
	}
	img.pixels = (Uint32*)img.surface->pixels;
	img.width = img.surface->w;
	img.height = img.surface->h;
	Load(objectFile);
}

BlenderObject::~BlenderObject() {
	for (int i = 0; i < triangles.size(); ++i)
		delete[]triangles[i];
	SDL_FreeSurface(img.surface);
}

void BlenderObject::Load(const char* objectFile)
{
	// Load object
	FILE* f;
	fopen_s(&f, objectFile, "r");
	char sym[100], buffer[100];
	double min = 1e10, max = -1e10;

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
					/*
					if (x > max)
						max = x;
					if (x < min)
						min = x;*/

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
				//if (fgetc(f) != '\n') {
				//	fscanf(f, "%d%c%d%c%d", &a, &s, &b, &s, &c);
				//	p3 = { vertices[a - 1].x * scale, vertices[a - 1].y * scale, vertices[a - 1].z * scale, texturePoints[b - 1].u, texturePoints[b - 1].v };
				//	triangles.push_back(new Triangle(p0, p1, p3));
				//}
			}
			else {
				fscanf(f," ");
				fgets(buffer, 100, f);
			}
		}
		fclose(f);
	}
	else {
		std::cout << "error: cannot open " << objectFile << std::endl;
		exit(1);
	}
	//printf("%u %lf %lf \n", triangles.size(),min*scale,max*scale);
}

/*
void BlenderObject::Logic(double elapsedTime)
{
	rotation.y += 1 * elapsedTime;
}*/

void BlenderObject::draw(Point3D pos, Point3D worldRot, double camDeg, double camDepth, Engine* engine, bool clean, double maxy)
{
	Uint32* bmp = engine->getPixels();
	if (clean) {
		memset(bmp, 0, WIDTH * HEIGHT * 4);
		memset(engine->getZBuffer(), 0, WIDTH * HEIGHT * sizeof(double));
	}

	std::vector<Triangle*> allTriangles;
	for (int i = 0; i < triangles.size(); ++i) {
		//axle rotate
		triangles[i]->calculateWorldPoints(worldRot, position, engine);
		triangles[i]->calculateCameraPoints(pos, camDeg, engine);

		std::vector<Triangle*> clippedTriangles = triangles[i]->GetZClippedTriangles();
		for (int j = 0; j < clippedTriangles.size(); ++j) {
			allTriangles.push_back(clippedTriangles[j]);
		}
	}

	for (int i = 0; i < allTriangles.size(); ++i) {
		//camera rotate
		allTriangles[i]->calculateDrawPoints({ 0,0,0 }, position, camDepth, engine);
		if (allTriangles[i]->getNormalZ() < 0) {
			std::vector<Triangle*> clippedTriangles = allTriangles[i]->GetClippedTriangles();
			for (int j = 0; j < clippedTriangles.size(); ++j) {
				clippedTriangles[j]->draw(bmp, img, engine->getZBuffer(), maxy);
				
				delete clippedTriangles[j];
			}
		}
		delete allTriangles[i];
	}
}