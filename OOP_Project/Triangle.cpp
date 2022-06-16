#include "Triangle.h"

Triangle::Triangle() :points{ 0 }, drawPoints{ 0 }, cameraPoints{ 0 }
{}

Triangle::Triangle(Point3D p0, Point3D p1, Point3D p2) {
	points[0] = p0;
	points[1] = p1;
	points[2] = p2;
	cameraPoints[0] = p0;
	cameraPoints[1] = p1;
	cameraPoints[2] = p2;
	drawPoints[0] = p0;
	drawPoints[1] = p1;
	drawPoints[2] = p2;
}

void Triangle::calculateWorldPoints(const Point3D& rotation, const Point3D& position, Engine* engine)
{
	//rotatation
	for (int i = 0; i < 3; ++i)
		worldPoints[i] = engine->Rotate(points[i], rotation);
	//translation
	for (int i = 0; i < 3; ++i)
		worldPoints[i] = engine->Translate(worldPoints[i], position);
}

void Triangle::calculateCameraPoints(Point3D pos, double camRot, Engine* engine) {

	//translation
	pos.x = -pos.x, pos.y = -pos.y, pos.z = -pos.z;
	for (int i = 0; i < 3; ++i){
		cameraPoints[i] = engine->Translate(worldPoints[i], pos);
	}
	//rotatation
	for (int i = 0; i < 3; ++i)
		cameraPoints[i] = engine->Rotate(cameraPoints[i], {0,-camRot,0});
}

void Triangle::calculateDrawPoints(const Point3D& rotation, const Point3D& position,double camDepth, Engine* engine)
{
	//projection
	for (int i = 0; i < 3; ++i)
		drawPoints[i] = engine->ApplyPerspective(cameraPoints[i], camDepth);
		
	//center screen
	for (int i = 0; i < 3; ++i)
		drawPoints[i] = engine->CenterScreen(drawPoints[i]);

	//normal z
	normalZ = (drawPoints[1].x - drawPoints[0].x) * (drawPoints[2].y - drawPoints[0].y) - (drawPoints[1].y - drawPoints[0].y) * (drawPoints[2].x - drawPoints[0].x);
}

void Triangle::draw(Uint32* bitmap, Image3D& img, double zbuffer[], double maxy)
{
	Point3D aux;
	for (int i = 0; i < 2; ++i)
		for (int j = i + 1; j < 3; ++j)
			if (drawPoints[i].y > drawPoints[j].y) {
				aux = drawPoints[i];
				drawPoints[i] = drawPoints[j];
				drawPoints[j] = aux;
			}

	int p0x = drawPoints[0].x;
	int p0y = drawPoints[0].y;
	int p1x = drawPoints[1].x;
	int p1y = drawPoints[1].y;
	int p2x = drawPoints[2].x;
	int p2y = drawPoints[2].y;

	double slope1, slope2;
	int x1, x2, y, aux1;
	double vs, ve, us, ue, ws, we, zs, ze, aux2;
	double u, ustep, v, vstep, w, wstep, z, zstep;

	int width = img.getWidth(), height = img.getHeight();

	if (p0y < p1y) {
		slope1 = (1.0 * p1x - p0x) / (p1y - p0y);
		slope2 = (1.0 * p2x - p0x) / (p2y - p0y);
		for (int i = 0; i <= p1y - p0y; i++) {
			x1 = p0x + i * slope1;
			x2 = p0x + i * slope2;
			y = p0y + i;
			if (y > maxy)
				break;

			us = drawPoints[0].u + (1.0 * y - p0y) / (p1y - p0y) * (drawPoints[1].u - drawPoints[0].u);
			ue = drawPoints[0].u + (1.0 * y - p0y) / (p2y - p0y) * (drawPoints[2].u - drawPoints[0].u);

			vs = drawPoints[0].v + (1.0 * y - p0y) / (p1y - p0y) * (drawPoints[1].v - drawPoints[0].v);
			ve = drawPoints[0].v + (1.0 * y - p0y) / (p2y - p0y) * (drawPoints[2].v - drawPoints[0].v);

			ws = drawPoints[0].w + (1.0 * y - p0y) / (p1y - p0y) * (drawPoints[1].w - drawPoints[0].w);
			we = drawPoints[0].w + (1.0 * y - p0y) / (p2y - p0y) * (drawPoints[2].w - drawPoints[0].w);

			zs = drawPoints[0].z + (1.0 * y - p0y) / (p1y - p0y) * (drawPoints[1].z - drawPoints[0].z);
			ze = drawPoints[0].z + (1.0 * y - p0y) / (p2y - p0y) * (drawPoints[2].z - drawPoints[0].z);


			if (x1 > x2) {
				aux1 = x1;
				x1 = x2;
				x2 = aux1;
				aux2 = us;
				us = ue;
				ue = aux2;
				aux2 = vs;
				vs = ve;
				ve = aux2;
				aux2 = ws;
				ws = we;
				we = aux2;
				aux2 = zs;
				zs = ze;
				ze = aux2;
			}
			if (x2 > x1) {
				u = us * width;
				ustep = (ue - us) / (x2 - x1) * width;
				v = vs * height;
				vstep = (ve - vs) / (x2 - x1) * height;
				w = ws;
				wstep = (we - ws) / (x2 - x1);
				z = zs;
				zstep = (ze - zs) / (x2 - x1);
				for (int x = x1; x <= x2; x++) {
					u += ustep;
					v += vstep;
					w += wstep;
					z += zstep;
					if (zbuffer[WIDTH * y + x] == 0 || zbuffer[WIDTH * y + x] > z) {
						bitmap[WIDTH * y + x] = img.getColor(u / w, v / w);
						zbuffer[WIDTH * y + x] = z;
					}
				}
			}
		}
	}
	if (p1y < p2y) {
		slope1 = (1.0 * p2x - p1x) / (p2y - p1y);
		slope2 = (1.0 * p2x - p0x) / (p2y - p0y);
		double sx = p2x - (p2y - p1y) * slope2;
		for (int i = 0; i <= p2y - p1y; i++) {
			x1 = p1x + i * slope1;
			x2 = sx + i * slope2;
			y = p1y + i;
			if (y > maxy)
				break;

			us = drawPoints[1].u + (1.0 * y - p1y) / (p2y - p1y) * (drawPoints[2].u - drawPoints[1].u);
			vs = drawPoints[1].v + (1.0 * y - p1y) / (p2y - p1y) * (drawPoints[2].v - drawPoints[1].v);
			ws = drawPoints[1].w + (1.0 * y - p1y) / (p2y - p1y) * (drawPoints[2].w - drawPoints[1].w);

			ue = drawPoints[0].u + (1.0 * y - p0y) / (p2y - p0y) * (drawPoints[2].u - drawPoints[0].u);
			ve = drawPoints[0].v + (1.0 * y - p0y) / (p2y - p0y) * (drawPoints[2].v - drawPoints[0].v);
			we = drawPoints[0].w + (1.0 * y - p0y) / (p2y - p0y) * (drawPoints[2].w - drawPoints[0].w);

			zs = drawPoints[1].z + (1.0 * y - p1y) / (p2y - p1y) * (drawPoints[2].z - drawPoints[1].z);
			ze = drawPoints[0].z + (1.0 * y - p0y) / (p2y - p0y) * (drawPoints[2].z - drawPoints[0].z);

			if (x1 > x2) {
				aux1 = x1;
				x1 = x2;
				x2 = aux1;
				aux2 = us;
				us = ue;
				ue = aux2;
				aux2 = vs;
				vs = ve;
				ve = aux2;
				aux2 = ws;
				ws = we;
				we = aux2;
				aux2 = zs;
				zs = ze;
				ze = aux2;
			}
			if (x2 > x1) {
				u = us * width;
				ustep = (ue - us) / (x2 - x1) * width;
				v = vs * height;
				vstep = (ve - vs) / (x2 - x1) * height;
				w = ws;
				wstep = (we - ws) / (x2 - x1);
				z = zs;
				zstep = (ze - zs) / (x2 - x1);
				for (int x = x1; x <= x2; x++) {
					u += ustep;
					v += vstep;
					w += wstep;
					z += zstep;
					if (zbuffer[WIDTH * y + x] == 0 || zbuffer[WIDTH * y + x] > z) {
						bitmap[WIDTH * y + x] = img.getColor(u / w, v / w);
						zbuffer[WIDTH * y + x] = z;
					}
				}
			}
		}
	}
}


std::vector<Triangle*> Triangle::GetClippedTriangles()
{
	std::vector<Triangle*> toReturn;
	toReturn.push_back(new Triangle(drawPoints[0], drawPoints[1], drawPoints[2]));

	int size;
	std::vector<Point3D> inside;
	std::vector<Point3D> outside;
	Point3D extraPoint1, extraPoint2;

	// LEFT
	size = toReturn.size();
	for (int i = 0; i < size; i++) {

		Triangle* currentTriangle = toReturn.front();
		toReturn.erase(toReturn.begin());

		inside.clear();
		outside.clear();
		for (int i = 0; i < 3; i++)
		{
			if (currentTriangle->drawPoints[i].x < 0)
				outside.push_back(currentTriangle->drawPoints[i]);
			else
				inside.push_back(currentTriangle->drawPoints[i]);
		}
		delete currentTriangle;
		if (outside.size() == 0)
		{
			toReturn.push_back(new Triangle(inside[0], inside[1], inside[2]));
		}
		else if (outside.size() == 1)
		{
			extraPoint1.x = 0;
			extraPoint1.y = outside[0].y + (0 - outside[0].x) * (inside[0].y - outside[0].y) / (inside[0].x - outside[0].x);
			extraPoint1.z = outside[0].z + (0 - outside[0].x) * (inside[0].z - outside[0].z) / (inside[0].x - outside[0].x);
			extraPoint1.u = outside[0].u + (0 - outside[0].x) * (inside[0].u - outside[0].u) / (inside[0].x - outside[0].x);
			extraPoint1.v = outside[0].v + (0 - outside[0].x) * (inside[0].v - outside[0].v) / (inside[0].x - outside[0].x);
			extraPoint1.w = outside[0].w + (0 - outside[0].x) * (inside[0].w - outside[0].w) / (inside[0].x - outside[0].x);

			extraPoint2.x = 0;
			extraPoint2.y = outside[0].y + (0 - outside[0].x) * (inside[1].y - outside[0].y) / (inside[1].x - outside[0].x);
			extraPoint2.z = outside[0].z + (0 - outside[0].x) * (inside[1].z - outside[0].z) / (inside[1].x - outside[0].x);
			extraPoint2.u = outside[0].u + (0 - outside[0].x) * (inside[1].u - outside[0].u) / (inside[1].x - outside[0].x);
			extraPoint2.v = outside[0].v + (0 - outside[0].x) * (inside[1].v - outside[0].v) / (inside[1].x - outside[0].x);
			extraPoint2.w = outside[0].w + (0 - outside[0].x) * (inside[1].w - outside[0].w) / (inside[1].x - outside[0].x);

			toReturn.push_back(new Triangle(extraPoint1, inside[0], inside[1]));

			toReturn.push_back(new Triangle(extraPoint2, extraPoint1, inside[1]));
		}
		else if (outside.size() == 2)
		{
			extraPoint1.x = 0;
			extraPoint1.y = outside[0].y + (0 - outside[0].x) * (inside[0].y - outside[0].y) / (inside[0].x - outside[0].x);
			extraPoint1.z = outside[0].z + (0 - outside[0].x) * (inside[0].z - outside[0].z) / (inside[0].x - outside[0].x);
			extraPoint1.u = outside[0].u + (0 - outside[0].x) * (inside[0].u - outside[0].u) / (inside[0].x - outside[0].x);
			extraPoint1.v = outside[0].v + (0 - outside[0].x) * (inside[0].v - outside[0].v) / (inside[0].x - outside[0].x);
			extraPoint1.w = outside[0].w + (0 - outside[0].x) * (inside[0].w - outside[0].w) / (inside[0].x - outside[0].x);

			extraPoint2.x = 0;
			extraPoint2.y = outside[1].y + (0 - outside[1].x) * (inside[0].y - outside[1].y) / (inside[0].x - outside[1].x);
			extraPoint2.z = outside[1].z + (0 - outside[1].x) * (inside[0].z - outside[1].z) / (inside[0].x - outside[1].x);
			extraPoint2.u = outside[1].u + (0 - outside[1].x) * (inside[0].u - outside[1].u) / (inside[0].x - outside[1].x);
			extraPoint2.v = outside[1].v + (0 - outside[1].x) * (inside[0].v - outside[1].v) / (inside[0].x - outside[1].x);
			extraPoint2.w = outside[1].w + (0 - outside[1].x) * (inside[0].w - outside[1].w) / (inside[0].x - outside[1].x);

			toReturn.push_back(new Triangle(extraPoint1, extraPoint2, inside[0]));
		}
	}

	// RIGHT
	size = toReturn.size();
	for (int i = 0; i < size; i++) {

		Triangle* currentTriangle = toReturn.front();
		toReturn.erase(toReturn.begin());

		inside.clear();
		outside.clear();
		for (int i = 0; i < 3; i++)
		{
			if (currentTriangle->drawPoints[i].x >= WIDTH)
				outside.push_back(currentTriangle->drawPoints[i]);
			else
				inside.push_back(currentTriangle->drawPoints[i]);
		}
		delete currentTriangle;
		if (outside.size() == 0)
		{
			toReturn.push_back(new Triangle(inside[0], inside[1], inside[2]));
		}
		else if (outside.size() == 1)
		{
			extraPoint1.x = WIDTH - 1;
			extraPoint1.y = outside[0].y + (WIDTH - 1 - outside[0].x) * (inside[0].y - outside[0].y) / (inside[0].x - outside[0].x);
			extraPoint1.z = outside[0].z + (WIDTH - 1 - outside[0].x) * (inside[0].z - outside[0].z) / (inside[0].x - outside[0].x);
			extraPoint1.u = outside[0].u + (WIDTH - 1 - outside[0].x) * (inside[0].u - outside[0].u) / (inside[0].x - outside[0].x);
			extraPoint1.v = outside[0].v + (WIDTH - 1 - outside[0].x) * (inside[0].v - outside[0].v) / (inside[0].x - outside[0].x);
			extraPoint1.w = outside[0].w + (WIDTH - 1 - outside[0].x) * (inside[0].w - outside[0].w) / (inside[0].x - outside[0].x);

			extraPoint2.x = WIDTH - 1;
			extraPoint2.y = outside[0].y + (WIDTH - 1 - outside[0].x) * (inside[1].y - outside[0].y) / (inside[1].x - outside[0].x);
			extraPoint2.z = outside[0].z + (WIDTH - 1 - outside[0].x) * (inside[1].z - outside[0].z) / (inside[1].x - outside[0].x);
			extraPoint2.u = outside[0].u + (WIDTH - 1 - outside[0].x) * (inside[1].u - outside[0].u) / (inside[1].x - outside[0].x);
			extraPoint2.v = outside[0].v + (WIDTH - 1 - outside[0].x) * (inside[1].v - outside[0].v) / (inside[1].x - outside[0].x);
			extraPoint2.w = outside[0].w + (WIDTH - 1 - outside[0].x) * (inside[1].w - outside[0].w) / (inside[1].x - outside[0].x);

			toReturn.push_back(new Triangle(extraPoint1, inside[0], inside[1]));

			toReturn.push_back(new Triangle(extraPoint2, extraPoint1, inside[1]));
		}
		else if (outside.size() == 2)
		{
			extraPoint1.x = WIDTH - 1;
			extraPoint1.y = outside[0].y + (WIDTH - 1 - outside[0].x) * (inside[0].y - outside[0].y) / (inside[0].x - outside[0].x);
			extraPoint1.z = outside[0].z + (WIDTH - 1 - outside[0].x) * (inside[0].z - outside[0].z) / (inside[0].x - outside[0].x);
			extraPoint1.u = outside[0].u + (WIDTH - 1 - outside[0].x) * (inside[0].u - outside[0].u) / (inside[0].x - outside[0].x);
			extraPoint1.v = outside[0].v + (WIDTH - 1 - outside[0].x) * (inside[0].v - outside[0].v) / (inside[0].x - outside[0].x);
			extraPoint1.w = outside[0].w + (WIDTH - 1 - outside[0].x) * (inside[0].w - outside[0].w) / (inside[0].x - outside[0].x);

			extraPoint2.x = WIDTH - 1;
			extraPoint2.y = outside[1].y + (WIDTH - 1 - outside[1].x) * (inside[0].y - outside[1].y) / (inside[0].x - outside[1].x);
			extraPoint2.z = outside[1].z + (WIDTH - 1 - outside[1].x) * (inside[0].z - outside[1].z) / (inside[0].x - outside[1].x);
			extraPoint2.u = outside[1].u + (WIDTH - 1 - outside[1].x) * (inside[0].u - outside[1].u) / (inside[0].x - outside[1].x);
			extraPoint2.v = outside[1].v + (WIDTH - 1 - outside[1].x) * (inside[0].v - outside[1].v) / (inside[0].x - outside[1].x);
			extraPoint2.w = outside[1].w + (WIDTH - 1 - outside[1].x) * (inside[0].w - outside[1].w) / (inside[0].x - outside[1].x);

			toReturn.push_back(new Triangle(extraPoint1, extraPoint2, inside[0]));
		}
	}

	// TOP
	size = toReturn.size();
	for (int i = 0; i < size; i++) {

		Triangle* currentTriangle = toReturn.front();
		toReturn.erase(toReturn.begin());

		inside.clear();
		outside.clear();
		for (int i = 0; i < 3; i++)
		{
			if (currentTriangle->drawPoints[i].y < 0)
				outside.push_back(currentTriangle->drawPoints[i]);
			else
				inside.push_back(currentTriangle->drawPoints[i]);
		}
		delete currentTriangle;
		if (outside.size() == 0)
		{
			toReturn.push_back(new Triangle(inside[0], inside[1], inside[2]));
		}
		else if (outside.size() == 1)
		{
			extraPoint1.x = outside[0].x - outside[0].y * (inside[0].x - outside[0].x) / (inside[0].y - outside[0].y);
			extraPoint1.y = 0;
			extraPoint1.z = outside[0].z - outside[0].y * (inside[0].z - outside[0].z) / (inside[0].y - outside[0].y);
			extraPoint1.u = outside[0].u - outside[0].y * (inside[0].u - outside[0].u) / (inside[0].y - outside[0].y);
			extraPoint1.v = outside[0].v - outside[0].y * (inside[0].v - outside[0].v) / (inside[0].y - outside[0].y);
			extraPoint1.w = outside[0].w - outside[0].y * (inside[0].w - outside[0].w) / (inside[0].y - outside[0].y);

			extraPoint2.x = outside[0].x - outside[0].y * (inside[1].x - outside[0].x) / (inside[1].y - outside[0].y);
			extraPoint2.y = 0;
			extraPoint2.z = outside[0].z - outside[0].y * (inside[1].z - outside[0].z) / (inside[1].y - outside[0].y);
			extraPoint2.u = outside[0].u - outside[0].y * (inside[1].u - outside[0].u) / (inside[1].y - outside[0].y);
			extraPoint2.v = outside[0].v - outside[0].y * (inside[1].v - outside[0].v) / (inside[1].y - outside[0].y);
			extraPoint2.w = outside[0].w - outside[0].y * (inside[1].w - outside[0].w) / (inside[1].y - outside[0].y);

			toReturn.push_back(new Triangle(extraPoint1, inside[0], inside[1]));

			toReturn.push_back(new Triangle(extraPoint2, extraPoint1, inside[1]));
		}
		else if (outside.size() == 2)
		{
			extraPoint1.x = outside[0].x - outside[0].y * (inside[0].x - outside[0].x) / (inside[0].y - outside[0].y);
			extraPoint1.y = 0;
			extraPoint1.z = outside[0].z - outside[0].y * (inside[0].z - outside[0].z) / (inside[0].y - outside[0].y);
			extraPoint1.u = outside[0].u - outside[0].y * (inside[0].u - outside[0].u) / (inside[0].y - outside[0].y);
			extraPoint1.v = outside[0].v - outside[0].y * (inside[0].v - outside[0].v) / (inside[0].y - outside[0].y);
			extraPoint1.w = outside[0].w - outside[0].y * (inside[0].w - outside[0].w) / (inside[0].y - outside[0].y);

			extraPoint2.x = outside[1].x - outside[1].y * (inside[0].x - outside[1].x) / (inside[0].y - outside[1].y);
			extraPoint2.y = 0;
			extraPoint2.z = outside[1].z - outside[1].y * (inside[0].z - outside[1].z) / (inside[0].y - outside[1].y);
			extraPoint2.u = outside[1].u - outside[1].y * (inside[0].u - outside[1].u) / (inside[0].y - outside[1].y);
			extraPoint2.v = outside[1].v - outside[1].y * (inside[0].v - outside[1].v) / (inside[0].y - outside[1].y);
			extraPoint2.w = outside[1].w - outside[1].y * (inside[0].w - outside[1].w) / (inside[0].y - outside[1].y);

			toReturn.push_back(new Triangle(extraPoint1, extraPoint2, inside[0]));
		}
	}

	// BOTTOM
	size = toReturn.size();
	for (int i = 0; i < size; i++) {

		Triangle* currentTriangle = toReturn.front();
		toReturn.erase(toReturn.begin());

		inside.clear();
		outside.clear();
		for (int i = 0; i < 3; i++)
		{
			if (currentTriangle->drawPoints[i].y >= HEIGHT)
				outside.push_back(currentTriangle->drawPoints[i]);
			else
				inside.push_back(currentTriangle->drawPoints[i]);
		}
		delete currentTriangle;
		if (outside.size() == 0)
		{
			toReturn.push_back(new Triangle(inside[0], inside[1], inside[2]));
		}
		else if (outside.size() == 1)
		{
			Point3D extraPoint1;
			extraPoint1.x = outside[0].x + (HEIGHT - 1 - outside[0].y) * (inside[0].x - outside[0].x) / (inside[0].y - outside[0].y);
			extraPoint1.y = HEIGHT - 1;
			extraPoint1.z = outside[0].z + (HEIGHT - 1 - outside[0].y) * (inside[0].z - outside[0].z) / (inside[0].y - outside[0].y);
			extraPoint1.u = outside[0].u + (HEIGHT - 1 - outside[0].y) * (inside[0].u - outside[0].u) / (inside[0].y - outside[0].y);
			extraPoint1.v = outside[0].v + (HEIGHT - 1 - outside[0].y) * (inside[0].v - outside[0].v) / (inside[0].y - outside[0].y);
			extraPoint1.w = outside[0].w + (HEIGHT - 1 - outside[0].y) * (inside[0].w - outside[0].w) / (inside[0].y - outside[0].y);

			Point3D extraPoint2;
			extraPoint2.x = outside[0].x + (HEIGHT - 1 - outside[0].y) * (inside[1].x - outside[0].x) / (inside[1].y - outside[0].y);
			extraPoint2.y = HEIGHT - 1;
			extraPoint2.z = outside[0].z + (HEIGHT - 1 - outside[0].y) * (inside[1].z - outside[0].z) / (inside[1].y - outside[0].y);
			extraPoint2.u = outside[0].u + (HEIGHT - 1 - outside[0].y) * (inside[1].u - outside[0].u) / (inside[1].y - outside[0].y);
			extraPoint2.v = outside[0].v + (HEIGHT - 1 - outside[0].y) * (inside[1].v - outside[0].v) / (inside[1].y - outside[0].y);
			extraPoint2.w = outside[0].w + (HEIGHT - 1 - outside[0].y) * (inside[1].w - outside[0].w) / (inside[1].y - outside[0].y);

			toReturn.push_back(new Triangle(extraPoint1, inside[0], inside[1]));

			toReturn.push_back(new Triangle(extraPoint2, extraPoint1, inside[1]));
		}
		else if (outside.size() == 2)
		{
			Point3D extraPoint1;
			extraPoint1.x = outside[0].x + (HEIGHT - 1 - outside[0].y) * (inside[0].x - outside[0].x) / (inside[0].y - outside[0].y);
			extraPoint1.y = HEIGHT - 1;
			extraPoint1.z = outside[0].z + (HEIGHT - 1 - outside[0].y) * (inside[0].z - outside[0].z) / (inside[0].y - outside[0].y);
			extraPoint1.u = outside[0].u + (HEIGHT - 1 - outside[0].y) * (inside[0].u - outside[0].u) / (inside[0].y - outside[0].y);
			extraPoint1.v = outside[0].v + (HEIGHT - 1 - outside[0].y) * (inside[0].v - outside[0].v) / (inside[0].y - outside[0].y);
			extraPoint1.w = outside[0].w + (HEIGHT - 1 - outside[0].y) * (inside[0].w - outside[0].w) / (inside[0].y - outside[0].y);

			Point3D extraPoint2;
			extraPoint2.x = outside[1].x + (HEIGHT - 1 - outside[1].y) * (inside[0].x - outside[1].x) / (inside[0].y - outside[1].y);
			extraPoint2.y = HEIGHT - 1;
			extraPoint2.z = outside[1].z + (HEIGHT - 1 - outside[1].y) * (inside[0].z - outside[1].z) / (inside[0].y - outside[1].y);
			extraPoint2.u = outside[1].u + (HEIGHT - 1 - outside[1].y) * (inside[0].u - outside[1].u) / (inside[0].y - outside[1].y);
			extraPoint2.v = outside[1].v + (HEIGHT - 1 - outside[1].y) * (inside[0].v - outside[1].v) / (inside[0].y - outside[1].y);
			extraPoint2.w = outside[1].w + (HEIGHT - 1 - outside[1].y) * (inside[0].w - outside[1].w) / (inside[0].y - outside[1].y);

			toReturn.push_back(new Triangle(extraPoint1, extraPoint2, inside[0]));
		}
	}

	return toReturn;
}

std::vector<Triangle*> Triangle::GetZClippedTriangles()
{
	std::vector<Triangle*> toReturn;
	toReturn.push_back(new Triangle(cameraPoints[0], cameraPoints[1], cameraPoints[2]));

	int noTriangles;
	std::vector<Point3D> inside;
	std::vector<Point3D> outside;

	Point3D extraPoint1;
	Point3D extraPoint2;

	// Z
	noTriangles = toReturn.size();
	for (int i = 0; i < noTriangles; i++) {

		Triangle* currentTriangle = toReturn.front();
		toReturn.erase(toReturn.begin());

		inside.clear();
		outside.clear();

		bool pointsAreOutside[3];
		for (int i = 0; i < 3; i++)
		{
			pointsAreOutside[i] = currentTriangle->cameraPoints[i].z < 0;
			if (pointsAreOutside[i])
				outside.push_back(currentTriangle->cameraPoints[i]);
			else
				inside.push_back(currentTriangle->cameraPoints[i]);
		}
		delete currentTriangle;
		if (outside.size() == 0)
		{
			toReturn.push_back(new Triangle(inside[0], inside[1], inside[2]));
		}
		else if (outside.size() == 1)
		{
			extraPoint1.x = outside[0].x + (0 - outside[0].z) * (inside[0].x - outside[0].x) / (inside[0].z - outside[0].z);
			extraPoint1.y = outside[0].y + (0 - outside[0].z) * (inside[0].y - outside[0].y) / (inside[0].z - outside[0].z);
			extraPoint1.z = 0;
			extraPoint1.u = outside[0].u + (0 - outside[0].z) * (inside[0].u - outside[0].u) / (inside[0].z - outside[0].z);
			extraPoint1.v = outside[0].v + (0 - outside[0].z) * (inside[0].v - outside[0].v) / (inside[0].z - outside[0].z);
			extraPoint1.w = outside[0].w + (0 - outside[0].z) * (inside[0].w - outside[0].w) / (inside[0].z - outside[0].z);

			extraPoint2.x = outside[0].x + (0 - outside[0].z) * (inside[1].x - outside[0].x) / (inside[1].z - outside[0].z);
			extraPoint2.y = outside[0].y + (0 - outside[0].z) * (inside[1].y - outside[0].y) / (inside[1].z - outside[0].z);
			extraPoint2.z = 0;
			extraPoint2.u = outside[0].u + (0 - outside[0].z) * (inside[1].u - outside[0].u) / (inside[1].z - outside[0].z);
			extraPoint2.v = outside[0].v + (0 - outside[0].z) * (inside[1].v - outside[0].v) / (inside[1].z - outside[0].z);
			extraPoint2.w = outside[0].w + (0 - outside[0].z) * (inside[1].w - outside[0].w) / (inside[1].z - outside[0].z);

			if (pointsAreOutside[0]) {
				toReturn.push_back(new Triangle(extraPoint1, inside[0], inside[1]));
				toReturn.push_back(new Triangle(extraPoint2, extraPoint1, inside[1]));
			}
			else if (pointsAreOutside[1]) {
				toReturn.push_back(new Triangle(extraPoint1, inside[1], inside[0]));
				toReturn.push_back(new Triangle(extraPoint2, extraPoint1, inside[0]));
			}
			else if (pointsAreOutside[2]) {
				toReturn.push_back(new Triangle(extraPoint1, inside[0], inside[1]));
				toReturn.push_back(new Triangle(extraPoint2, extraPoint1, inside[1]));
			}
		}
		else if (outside.size() == 2)
		{
			extraPoint1.x = outside[0].x + (0 - outside[0].z) * (inside[0].x - outside[0].x) / (inside[0].z - outside[0].z);
			extraPoint1.y = outside[0].y + (0 - outside[0].z) * (inside[0].y - outside[0].y) / (inside[0].z - outside[0].z);
			extraPoint1.z = 0;
			extraPoint1.u = outside[0].u + (0 - outside[0].z) * (inside[0].u - outside[0].u) / (inside[0].z - outside[0].z);
			extraPoint1.v = outside[0].v + (0 - outside[0].z) * (inside[0].v - outside[0].v) / (inside[0].z - outside[0].z);
			extraPoint1.w = outside[0].w + (0 - outside[0].z) * (inside[0].w - outside[0].w) / (inside[0].z - outside[0].z);

			extraPoint2.x = outside[1].x + (0 - outside[1].z) * (inside[0].x - outside[1].x) / (inside[0].z - outside[1].z);
			extraPoint2.y = outside[1].y + (0 - outside[1].z) * (inside[0].y - outside[1].y) / (inside[0].z - outside[1].z);
			extraPoint2.z = 0;
			extraPoint2.u = outside[1].u + (0 - outside[1].z) * (inside[0].u - outside[1].u) / (inside[0].z - outside[1].z);
			extraPoint2.v = outside[1].v + (0 - outside[1].z) * (inside[0].v - outside[1].v) / (inside[0].z - outside[1].z);
			extraPoint2.w = outside[1].w + (0 - outside[1].z) * (inside[0].w - outside[1].w) / (inside[0].z - outside[1].z);

			if (!pointsAreOutside[0]) {
				toReturn.push_back(new Triangle(extraPoint2, inside[0], extraPoint1));
			}
			else if (!pointsAreOutside[1]) {
				toReturn.push_back(new Triangle(extraPoint1, inside[0], extraPoint2));
			}
			else if (!pointsAreOutside[2]) {
				toReturn.push_back(new Triangle(extraPoint2, inside[0], extraPoint1));
			}

		}
	}

	return toReturn;
}
