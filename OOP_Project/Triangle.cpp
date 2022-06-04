#include "Triangle.h"

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
	//rotate
	for (int i = 0; i < 3; ++i)
		worldPoints[i] = engine->Rotate(points[i], rotation);
	//translation
	for (int i = 0; i < 3; ++i)
		worldPoints[i] = engine->Translate(worldPoints[i], position);
}

void Triangle::calculateCameraPoints(Point3D pos, double camDeg, Engine* engine) {

	//translation
	pos.x = -pos.x, pos.y = -pos.y, pos.z = -pos.z;
	for (int i = 0; i < 3; ++i){
		cameraPoints[i] = engine->Translate(worldPoints[i], pos);
	}
	//rotate
	for (int i = 0; i < 3; ++i)
		cameraPoints[i] = engine->Rotate(cameraPoints[i], { 0,-camDeg,0 });

	//average Z
	averageZ = (cameraPoints[0].z + cameraPoints[1].z + cameraPoints[2].z) / 3.0;
}

void Triangle::calculateDrawPoints(const Point3D& rotation, const Point3D& position,double camDepth, Engine* engine)
{
	//apply perspective
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
			vs = drawPoints[0].v + (1.0 * y - p0y) / (p1y - p0y) * (drawPoints[1].v - drawPoints[0].v);
			ws = drawPoints[0].w + (1.0 * y - p0y) / (p1y - p0y) * (drawPoints[1].w - drawPoints[0].w);

			ue = drawPoints[0].u + (1.0 * y - p0y) / (p2y - p0y) * (drawPoints[2].u - drawPoints[0].u);
			ve = drawPoints[0].v + (1.0 * y - p0y) / (p2y - p0y) * (drawPoints[2].v - drawPoints[0].v);
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
				u = us * img.width;
				ustep = (ue - us) / (x2 - x1) * img.width;
				v = vs * img.height;
				vstep = (ve - vs) / (x2 - x1) * img.height;
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
				u = us * img.width;
				ustep = (ue - us) / (x2 - x1) * img.width;
				v = vs * img.height;
				vstep = (ve - vs) / (x2 - x1) * img.height;
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

bool Triangle::compare(Triangle* a, Triangle* b) {
	return a->averageZ > b->averageZ;
}

std::vector<Triangle*> Triangle::GetClippedTriangles()
{
	std::vector<Triangle*> toReturn;
	toReturn.push_back(new Triangle(drawPoints[0], drawPoints[1], drawPoints[2]));

	int noTriangles;
	std::vector<Point3D> insidePoints;
	std::vector<Point3D> outsidePoints;

	// LEFT
	noTriangles = toReturn.size();
	for (int i = 0; i < noTriangles; i++) {

		Triangle* currentTriangle = toReturn.front();
		toReturn.erase(toReturn.begin());

		insidePoints.clear();
		outsidePoints.clear();
		for (int i = 0; i < 3; i++)
		{
			if (currentTriangle->drawPoints[i].x < 0)
				outsidePoints.push_back(currentTriangle->drawPoints[i]);
			else
				insidePoints.push_back(currentTriangle->drawPoints[i]);
		}
		delete currentTriangle;
		if (outsidePoints.size() == 0)
		{
			toReturn.push_back(new Triangle(insidePoints[0], insidePoints[1], insidePoints[2]));
		}
		else if (outsidePoints.size() == 1)
		{
			Point3D extraPoint1;
			extraPoint1.x = 0;
			extraPoint1.y = outsidePoints[0].y + (0 - outsidePoints[0].x) * (insidePoints[0].y - outsidePoints[0].y) / (insidePoints[0].x - outsidePoints[0].x);
			extraPoint1.z = outsidePoints[0].z + (0 - outsidePoints[0].x) * (insidePoints[0].z - outsidePoints[0].z) / (insidePoints[0].x - outsidePoints[0].x);
			extraPoint1.u = outsidePoints[0].u + (0 - outsidePoints[0].x) * (insidePoints[0].u - outsidePoints[0].u) / (insidePoints[0].x - outsidePoints[0].x);
			extraPoint1.v = outsidePoints[0].v + (0 - outsidePoints[0].x) * (insidePoints[0].v - outsidePoints[0].v) / (insidePoints[0].x - outsidePoints[0].x);
			extraPoint1.w = outsidePoints[0].w + (0 - outsidePoints[0].x) * (insidePoints[0].w - outsidePoints[0].w) / (insidePoints[0].x - outsidePoints[0].x);

			Point3D extraPoint2;
			extraPoint2.x = 0;
			extraPoint2.y = outsidePoints[0].y + (0 - outsidePoints[0].x) * (insidePoints[1].y - outsidePoints[0].y) / (insidePoints[1].x - outsidePoints[0].x);
			extraPoint2.z = outsidePoints[0].z + (0 - outsidePoints[0].x) * (insidePoints[1].z - outsidePoints[0].z) / (insidePoints[1].x - outsidePoints[0].x);
			extraPoint2.u = outsidePoints[0].u + (0 - outsidePoints[0].x) * (insidePoints[1].u - outsidePoints[0].u) / (insidePoints[1].x - outsidePoints[0].x);
			extraPoint2.v = outsidePoints[0].v + (0 - outsidePoints[0].x) * (insidePoints[1].v - outsidePoints[0].v) / (insidePoints[1].x - outsidePoints[0].x);
			extraPoint2.w = outsidePoints[0].w + (0 - outsidePoints[0].x) * (insidePoints[1].w - outsidePoints[0].w) / (insidePoints[1].x - outsidePoints[0].x);

			toReturn.push_back(new Triangle(extraPoint1, insidePoints[0], insidePoints[1]));

			toReturn.push_back(new Triangle(extraPoint2, extraPoint1, insidePoints[1]));
		}
		else if (outsidePoints.size() == 2)
		{
			Point3D extraPoint1;
			extraPoint1.x = 0;
			extraPoint1.y = outsidePoints[0].y + (0 - outsidePoints[0].x) * (insidePoints[0].y - outsidePoints[0].y) / (insidePoints[0].x - outsidePoints[0].x);
			extraPoint1.z = outsidePoints[0].z + (0 - outsidePoints[0].x) * (insidePoints[0].z - outsidePoints[0].z) / (insidePoints[0].x - outsidePoints[0].x);
			extraPoint1.u = outsidePoints[0].u + (0 - outsidePoints[0].x) * (insidePoints[0].u - outsidePoints[0].u) / (insidePoints[0].x - outsidePoints[0].x);
			extraPoint1.v = outsidePoints[0].v + (0 - outsidePoints[0].x) * (insidePoints[0].v - outsidePoints[0].v) / (insidePoints[0].x - outsidePoints[0].x);
			extraPoint1.w = outsidePoints[0].w + (0 - outsidePoints[0].x) * (insidePoints[0].w - outsidePoints[0].w) / (insidePoints[0].x - outsidePoints[0].x);

			Point3D extraPoint2;
			extraPoint2.x = 0;
			extraPoint2.y = outsidePoints[1].y + (0 - outsidePoints[1].x) * (insidePoints[0].y - outsidePoints[1].y) / (insidePoints[0].x - outsidePoints[1].x);
			extraPoint2.z = outsidePoints[1].z + (0 - outsidePoints[1].x) * (insidePoints[0].z - outsidePoints[1].z) / (insidePoints[0].x - outsidePoints[1].x);
			extraPoint2.u = outsidePoints[1].u + (0 - outsidePoints[1].x) * (insidePoints[0].u - outsidePoints[1].u) / (insidePoints[0].x - outsidePoints[1].x);
			extraPoint2.v = outsidePoints[1].v + (0 - outsidePoints[1].x) * (insidePoints[0].v - outsidePoints[1].v) / (insidePoints[0].x - outsidePoints[1].x);
			extraPoint2.w = outsidePoints[1].w + (0 - outsidePoints[1].x) * (insidePoints[0].w - outsidePoints[1].w) / (insidePoints[0].x - outsidePoints[1].x);

			toReturn.push_back(new Triangle(extraPoint1, extraPoint2, insidePoints[0]));
		}
	}

	// RIGHT
	noTriangles = toReturn.size();
	for (int i = 0; i < noTriangles; i++) {

		Triangle* currentTriangle = toReturn.front();
		toReturn.erase(toReturn.begin());

		insidePoints.clear();
		outsidePoints.clear();
		for (int i = 0; i < 3; i++)
		{
			if (currentTriangle->drawPoints[i].x >= WIDTH)
				outsidePoints.push_back(currentTriangle->drawPoints[i]);
			else
				insidePoints.push_back(currentTriangle->drawPoints[i]);
		}
		delete currentTriangle;
		if (outsidePoints.size() == 0)
		{
			toReturn.push_back(new Triangle(insidePoints[0], insidePoints[1], insidePoints[2]));
		}
		else if (outsidePoints.size() == 1)
		{
			Point3D extraPoint1;
			extraPoint1.x = WIDTH - 1;
			extraPoint1.y = outsidePoints[0].y + (WIDTH - 1 - outsidePoints[0].x) * (insidePoints[0].y - outsidePoints[0].y) / (insidePoints[0].x - outsidePoints[0].x);
			extraPoint1.z = outsidePoints[0].z + (WIDTH - 1 - outsidePoints[0].x) * (insidePoints[0].z - outsidePoints[0].z) / (insidePoints[0].x - outsidePoints[0].x);
			extraPoint1.u = outsidePoints[0].u + (WIDTH - 1 - outsidePoints[0].x) * (insidePoints[0].u - outsidePoints[0].u) / (insidePoints[0].x - outsidePoints[0].x);
			extraPoint1.v = outsidePoints[0].v + (WIDTH - 1 - outsidePoints[0].x) * (insidePoints[0].v - outsidePoints[0].v) / (insidePoints[0].x - outsidePoints[0].x);
			extraPoint1.w = outsidePoints[0].w + (WIDTH - 1 - outsidePoints[0].x) * (insidePoints[0].w - outsidePoints[0].w) / (insidePoints[0].x - outsidePoints[0].x);

			Point3D extraPoint2;
			extraPoint2.x = WIDTH - 1;
			extraPoint2.y = outsidePoints[0].y + (WIDTH - 1 - outsidePoints[0].x) * (insidePoints[1].y - outsidePoints[0].y) / (insidePoints[1].x - outsidePoints[0].x);
			extraPoint2.z = outsidePoints[0].z + (WIDTH - 1 - outsidePoints[0].x) * (insidePoints[1].z - outsidePoints[0].z) / (insidePoints[1].x - outsidePoints[0].x);
			extraPoint2.u = outsidePoints[0].u + (WIDTH - 1 - outsidePoints[0].x) * (insidePoints[1].u - outsidePoints[0].u) / (insidePoints[1].x - outsidePoints[0].x);
			extraPoint2.v = outsidePoints[0].v + (WIDTH - 1 - outsidePoints[0].x) * (insidePoints[1].v - outsidePoints[0].v) / (insidePoints[1].x - outsidePoints[0].x);
			extraPoint2.w = outsidePoints[0].w + (WIDTH - 1 - outsidePoints[0].x) * (insidePoints[1].w - outsidePoints[0].w) / (insidePoints[1].x - outsidePoints[0].x);

			toReturn.push_back(new Triangle(extraPoint1, insidePoints[0], insidePoints[1]));

			toReturn.push_back(new Triangle(extraPoint2, extraPoint1, insidePoints[1]));
		}
		else if (outsidePoints.size() == 2)
		{
			Point3D extraPoint1;
			extraPoint1.x = WIDTH - 1;
			extraPoint1.y = outsidePoints[0].y + (WIDTH - 1 - outsidePoints[0].x) * (insidePoints[0].y - outsidePoints[0].y) / (insidePoints[0].x - outsidePoints[0].x);
			extraPoint1.z = outsidePoints[0].z + (WIDTH - 1 - outsidePoints[0].x) * (insidePoints[0].z - outsidePoints[0].z) / (insidePoints[0].x - outsidePoints[0].x);
			extraPoint1.u = outsidePoints[0].u + (WIDTH - 1 - outsidePoints[0].x) * (insidePoints[0].u - outsidePoints[0].u) / (insidePoints[0].x - outsidePoints[0].x);
			extraPoint1.v = outsidePoints[0].v + (WIDTH - 1 - outsidePoints[0].x) * (insidePoints[0].v - outsidePoints[0].v) / (insidePoints[0].x - outsidePoints[0].x);
			extraPoint1.w = outsidePoints[0].w + (WIDTH - 1 - outsidePoints[0].x) * (insidePoints[0].w - outsidePoints[0].w) / (insidePoints[0].x - outsidePoints[0].x);

			Point3D extraPoint2;
			extraPoint2.x = WIDTH - 1;
			extraPoint2.y = outsidePoints[1].y + (WIDTH - 1 - outsidePoints[1].x) * (insidePoints[0].y - outsidePoints[1].y) / (insidePoints[0].x - outsidePoints[1].x);
			extraPoint2.z = outsidePoints[1].z + (WIDTH - 1 - outsidePoints[1].x) * (insidePoints[0].z - outsidePoints[1].z) / (insidePoints[0].x - outsidePoints[1].x);
			extraPoint2.u = outsidePoints[1].u + (WIDTH - 1 - outsidePoints[1].x) * (insidePoints[0].u - outsidePoints[1].u) / (insidePoints[0].x - outsidePoints[1].x);
			extraPoint2.v = outsidePoints[1].v + (WIDTH - 1 - outsidePoints[1].x) * (insidePoints[0].v - outsidePoints[1].v) / (insidePoints[0].x - outsidePoints[1].x);
			extraPoint2.w = outsidePoints[1].w + (WIDTH - 1 - outsidePoints[1].x) * (insidePoints[0].w - outsidePoints[1].w) / (insidePoints[0].x - outsidePoints[1].x);

			toReturn.push_back(new Triangle(extraPoint1, extraPoint2, insidePoints[0]));
		}
	}

	// TOP
	noTriangles = toReturn.size();
	for (int i = 0; i < noTriangles; i++) {

		Triangle* currentTriangle = toReturn.front();
		toReturn.erase(toReturn.begin());

		insidePoints.clear();
		outsidePoints.clear();
		for (int i = 0; i < 3; i++)
		{
			if (currentTriangle->drawPoints[i].y < 0)
				outsidePoints.push_back(currentTriangle->drawPoints[i]);
			else
				insidePoints.push_back(currentTriangle->drawPoints[i]);
		}
		delete currentTriangle;
		if (outsidePoints.size() == 0)
		{
			toReturn.push_back(new Triangle(insidePoints[0], insidePoints[1], insidePoints[2]));
		}
		else if (outsidePoints.size() == 1)
		{
			Point3D extraPoint1;
			extraPoint1.x = outsidePoints[0].x - outsidePoints[0].y * (insidePoints[0].x - outsidePoints[0].x) / (insidePoints[0].y - outsidePoints[0].y);
			extraPoint1.y = 0;
			extraPoint1.z = outsidePoints[0].z - outsidePoints[0].y * (insidePoints[0].z - outsidePoints[0].z) / (insidePoints[0].y - outsidePoints[0].y);
			extraPoint1.u = outsidePoints[0].u - outsidePoints[0].y * (insidePoints[0].u - outsidePoints[0].u) / (insidePoints[0].y - outsidePoints[0].y);
			extraPoint1.v = outsidePoints[0].v - outsidePoints[0].y * (insidePoints[0].v - outsidePoints[0].v) / (insidePoints[0].y - outsidePoints[0].y);
			extraPoint1.w = outsidePoints[0].w - outsidePoints[0].y * (insidePoints[0].w - outsidePoints[0].w) / (insidePoints[0].y - outsidePoints[0].y);

			Point3D extraPoint2;
			extraPoint2.x = outsidePoints[0].x - outsidePoints[0].y * (insidePoints[1].x - outsidePoints[0].x) / (insidePoints[1].y - outsidePoints[0].y);
			extraPoint2.y = 0;
			extraPoint2.z = outsidePoints[0].z - outsidePoints[0].y * (insidePoints[1].z - outsidePoints[0].z) / (insidePoints[1].y - outsidePoints[0].y);
			extraPoint2.u = outsidePoints[0].u - outsidePoints[0].y * (insidePoints[1].u - outsidePoints[0].u) / (insidePoints[1].y - outsidePoints[0].y);
			extraPoint2.v = outsidePoints[0].v - outsidePoints[0].y * (insidePoints[1].v - outsidePoints[0].v) / (insidePoints[1].y - outsidePoints[0].y);
			extraPoint2.w = outsidePoints[0].w - outsidePoints[0].y * (insidePoints[1].w - outsidePoints[0].w) / (insidePoints[1].y - outsidePoints[0].y);

			toReturn.push_back(new Triangle(extraPoint1, insidePoints[0], insidePoints[1]));

			toReturn.push_back(new Triangle(extraPoint2, extraPoint1, insidePoints[1]));
		}
		else if (outsidePoints.size() == 2)
		{
			Point3D extraPoint1;
			extraPoint1.x = outsidePoints[0].x - outsidePoints[0].y * (insidePoints[0].x - outsidePoints[0].x) / (insidePoints[0].y - outsidePoints[0].y);
			extraPoint1.y = 0;
			extraPoint1.z = outsidePoints[0].z - outsidePoints[0].y * (insidePoints[0].z - outsidePoints[0].z) / (insidePoints[0].y - outsidePoints[0].y);
			extraPoint1.u = outsidePoints[0].u - outsidePoints[0].y * (insidePoints[0].u - outsidePoints[0].u) / (insidePoints[0].y - outsidePoints[0].y);
			extraPoint1.v = outsidePoints[0].v - outsidePoints[0].y * (insidePoints[0].v - outsidePoints[0].v) / (insidePoints[0].y - outsidePoints[0].y);
			extraPoint1.w = outsidePoints[0].w - outsidePoints[0].y * (insidePoints[0].w - outsidePoints[0].w) / (insidePoints[0].y - outsidePoints[0].y);

			Point3D extraPoint2;
			extraPoint2.x = outsidePoints[1].x - outsidePoints[1].y * (insidePoints[0].x - outsidePoints[1].x) / (insidePoints[0].y - outsidePoints[1].y);
			extraPoint2.y = 0;
			extraPoint2.z = outsidePoints[1].z - outsidePoints[1].y * (insidePoints[0].z - outsidePoints[1].z) / (insidePoints[0].y - outsidePoints[1].y);
			extraPoint2.u = outsidePoints[1].u - outsidePoints[1].y * (insidePoints[0].u - outsidePoints[1].u) / (insidePoints[0].y - outsidePoints[1].y);
			extraPoint2.v = outsidePoints[1].v - outsidePoints[1].y * (insidePoints[0].v - outsidePoints[1].v) / (insidePoints[0].y - outsidePoints[1].y);
			extraPoint2.w = outsidePoints[1].w - outsidePoints[1].y * (insidePoints[0].w - outsidePoints[1].w) / (insidePoints[0].y - outsidePoints[1].y);

			toReturn.push_back(new Triangle(extraPoint1, extraPoint2, insidePoints[0]));
		}
	}

	// BOTTOM
	noTriangles = toReturn.size();
	for (int i = 0; i < noTriangles; i++) {

		Triangle* currentTriangle = toReturn.front();
		toReturn.erase(toReturn.begin());

		insidePoints.clear();
		outsidePoints.clear();
		for (int i = 0; i < 3; i++)
		{
			if (currentTriangle->drawPoints[i].y >= HEIGHT)
				outsidePoints.push_back(currentTriangle->drawPoints[i]);
			else
				insidePoints.push_back(currentTriangle->drawPoints[i]);
		}
		delete currentTriangle;
		if (outsidePoints.size() == 0)
		{
			toReturn.push_back(new Triangle(insidePoints[0], insidePoints[1], insidePoints[2]));
		}
		else if (outsidePoints.size() == 1)
		{
			Point3D extraPoint1;
			extraPoint1.x = outsidePoints[0].x + (HEIGHT - 1 - outsidePoints[0].y) * (insidePoints[0].x - outsidePoints[0].x) / (insidePoints[0].y - outsidePoints[0].y);
			extraPoint1.y = HEIGHT - 1;
			extraPoint1.z = outsidePoints[0].z + (HEIGHT - 1 - outsidePoints[0].y) * (insidePoints[0].z - outsidePoints[0].z) / (insidePoints[0].y - outsidePoints[0].y);
			extraPoint1.u = outsidePoints[0].u + (HEIGHT - 1 - outsidePoints[0].y) * (insidePoints[0].u - outsidePoints[0].u) / (insidePoints[0].y - outsidePoints[0].y);
			extraPoint1.v = outsidePoints[0].v + (HEIGHT - 1 - outsidePoints[0].y) * (insidePoints[0].v - outsidePoints[0].v) / (insidePoints[0].y - outsidePoints[0].y);
			extraPoint1.w = outsidePoints[0].w + (HEIGHT - 1 - outsidePoints[0].y) * (insidePoints[0].w - outsidePoints[0].w) / (insidePoints[0].y - outsidePoints[0].y);

			Point3D extraPoint2;
			extraPoint2.x = outsidePoints[0].x + (HEIGHT - 1 - outsidePoints[0].y) * (insidePoints[1].x - outsidePoints[0].x) / (insidePoints[1].y - outsidePoints[0].y);
			extraPoint2.y = HEIGHT - 1;
			extraPoint2.z = outsidePoints[0].z + (HEIGHT - 1 - outsidePoints[0].y) * (insidePoints[1].z - outsidePoints[0].z) / (insidePoints[1].y - outsidePoints[0].y);
			extraPoint2.u = outsidePoints[0].u + (HEIGHT - 1 - outsidePoints[0].y) * (insidePoints[1].u - outsidePoints[0].u) / (insidePoints[1].y - outsidePoints[0].y);
			extraPoint2.v = outsidePoints[0].v + (HEIGHT - 1 - outsidePoints[0].y) * (insidePoints[1].v - outsidePoints[0].v) / (insidePoints[1].y - outsidePoints[0].y);
			extraPoint2.w = outsidePoints[0].w + (HEIGHT - 1 - outsidePoints[0].y) * (insidePoints[1].w - outsidePoints[0].w) / (insidePoints[1].y - outsidePoints[0].y);

			toReturn.push_back(new Triangle(extraPoint1, insidePoints[0], insidePoints[1]));

			toReturn.push_back(new Triangle(extraPoint2, extraPoint1, insidePoints[1]));
		}
		else if (outsidePoints.size() == 2)
		{
			Point3D extraPoint1;
			extraPoint1.x = outsidePoints[0].x + (HEIGHT - 1 - outsidePoints[0].y) * (insidePoints[0].x - outsidePoints[0].x) / (insidePoints[0].y - outsidePoints[0].y);
			extraPoint1.y = HEIGHT - 1;
			extraPoint1.z = outsidePoints[0].z + (HEIGHT - 1 - outsidePoints[0].y) * (insidePoints[0].z - outsidePoints[0].z) / (insidePoints[0].y - outsidePoints[0].y);
			extraPoint1.u = outsidePoints[0].u + (HEIGHT - 1 - outsidePoints[0].y) * (insidePoints[0].u - outsidePoints[0].u) / (insidePoints[0].y - outsidePoints[0].y);
			extraPoint1.v = outsidePoints[0].v + (HEIGHT - 1 - outsidePoints[0].y) * (insidePoints[0].v - outsidePoints[0].v) / (insidePoints[0].y - outsidePoints[0].y);
			extraPoint1.w = outsidePoints[0].w + (HEIGHT - 1 - outsidePoints[0].y) * (insidePoints[0].w - outsidePoints[0].w) / (insidePoints[0].y - outsidePoints[0].y);

			Point3D extraPoint2;
			extraPoint2.x = outsidePoints[1].x + (HEIGHT - 1 - outsidePoints[1].y) * (insidePoints[0].x - outsidePoints[1].x) / (insidePoints[0].y - outsidePoints[1].y);
			extraPoint2.y = HEIGHT - 1;
			extraPoint2.z = outsidePoints[1].z + (HEIGHT - 1 - outsidePoints[1].y) * (insidePoints[0].z - outsidePoints[1].z) / (insidePoints[0].y - outsidePoints[1].y);
			extraPoint2.u = outsidePoints[1].u + (HEIGHT - 1 - outsidePoints[1].y) * (insidePoints[0].u - outsidePoints[1].u) / (insidePoints[0].y - outsidePoints[1].y);
			extraPoint2.v = outsidePoints[1].v + (HEIGHT - 1 - outsidePoints[1].y) * (insidePoints[0].v - outsidePoints[1].v) / (insidePoints[0].y - outsidePoints[1].y);
			extraPoint2.w = outsidePoints[1].w + (HEIGHT - 1 - outsidePoints[1].y) * (insidePoints[0].w - outsidePoints[1].w) / (insidePoints[0].y - outsidePoints[1].y);

			toReturn.push_back(new Triangle(extraPoint1, extraPoint2, insidePoints[0]));
		}
	}


	return toReturn;
}


std::vector<Triangle*> Triangle::GetZClippedTriangles()
{
	std::vector<Triangle*> toReturn;
	toReturn.push_back(new Triangle(cameraPoints[0], cameraPoints[1], cameraPoints[2]));

	int noTriangles;
	std::vector<Point3D> insidePoints;
	std::vector<Point3D> outsidePoints;

	// Z
	noTriangles = toReturn.size();
	for (int i = 0; i < noTriangles; i++) {

		Triangle* currentTriangle = toReturn.front();
		toReturn.erase(toReturn.begin());

		insidePoints.clear();
		outsidePoints.clear();

		bool pointsAreOutside[3];
		for (int i = 0; i < 3; i++)
		{
			pointsAreOutside[i] = currentTriangle->cameraPoints[i].z < 0;
			if (pointsAreOutside[i])
				outsidePoints.push_back(currentTriangle->cameraPoints[i]);
			else
				insidePoints.push_back(currentTriangle->cameraPoints[i]);
		}
		delete currentTriangle;
		if (outsidePoints.size() == 0)
		{
			toReturn.push_back(new Triangle(insidePoints[0], insidePoints[1], insidePoints[2]));
		}
		else if (outsidePoints.size() == 1)
		{
			Point3D extraPoint1;
			extraPoint1.x = outsidePoints[0].x + (0 - outsidePoints[0].z) * (insidePoints[0].x - outsidePoints[0].x) / (insidePoints[0].z - outsidePoints[0].z);
			extraPoint1.y = outsidePoints[0].y + (0 - outsidePoints[0].z) * (insidePoints[0].y - outsidePoints[0].y) / (insidePoints[0].z - outsidePoints[0].z);
			extraPoint1.z = 0;
			extraPoint1.u = outsidePoints[0].u + (0 - outsidePoints[0].z) * (insidePoints[0].u - outsidePoints[0].u) / (insidePoints[0].z - outsidePoints[0].z);
			extraPoint1.v = outsidePoints[0].v + (0 - outsidePoints[0].z) * (insidePoints[0].v - outsidePoints[0].v) / (insidePoints[0].z - outsidePoints[0].z);
			extraPoint1.w = outsidePoints[0].w + (0 - outsidePoints[0].z) * (insidePoints[0].w - outsidePoints[0].w) / (insidePoints[0].z - outsidePoints[0].z);

			Point3D extraPoint2;
			extraPoint2.x = outsidePoints[0].x + (0 - outsidePoints[0].z) * (insidePoints[1].x - outsidePoints[0].x) / (insidePoints[1].z - outsidePoints[0].z);
			extraPoint2.y = outsidePoints[0].y + (0 - outsidePoints[0].z) * (insidePoints[1].y - outsidePoints[0].y) / (insidePoints[1].z - outsidePoints[0].z);
			extraPoint2.z = 0;
			extraPoint2.u = outsidePoints[0].u + (0 - outsidePoints[0].z) * (insidePoints[1].u - outsidePoints[0].u) / (insidePoints[1].z - outsidePoints[0].z);
			extraPoint2.v = outsidePoints[0].v + (0 - outsidePoints[0].z) * (insidePoints[1].v - outsidePoints[0].v) / (insidePoints[1].z - outsidePoints[0].z);
			extraPoint2.w = outsidePoints[0].w + (0 - outsidePoints[0].z) * (insidePoints[1].w - outsidePoints[0].w) / (insidePoints[1].z - outsidePoints[0].z);

			if (pointsAreOutside[0]) {
				toReturn.push_back(new Triangle(extraPoint1, insidePoints[0], insidePoints[1]));
				toReturn.push_back(new Triangle(extraPoint2, extraPoint1, insidePoints[1]));
			}
			else if (pointsAreOutside[1]) {
				toReturn.push_back(new Triangle(extraPoint1, insidePoints[1], insidePoints[0]));
				toReturn.push_back(new Triangle(extraPoint2, extraPoint1, insidePoints[0]));
			}
			else if (pointsAreOutside[2]) {
				toReturn.push_back(new Triangle(extraPoint1, insidePoints[0], insidePoints[1]));
				toReturn.push_back(new Triangle(extraPoint2, extraPoint1, insidePoints[1]));
			}
		}
		else if (outsidePoints.size() == 2)
		{
			Point3D extraPoint1;
			extraPoint1.x = outsidePoints[0].x + (0 - outsidePoints[0].z) * (insidePoints[0].x - outsidePoints[0].x) / (insidePoints[0].z - outsidePoints[0].z);
			extraPoint1.y = outsidePoints[0].y + (0 - outsidePoints[0].z) * (insidePoints[0].y - outsidePoints[0].y) / (insidePoints[0].z - outsidePoints[0].z);
			extraPoint1.z = 0;
			extraPoint1.u = outsidePoints[0].u + (0 - outsidePoints[0].z) * (insidePoints[0].u - outsidePoints[0].u) / (insidePoints[0].z - outsidePoints[0].z);
			extraPoint1.v = outsidePoints[0].v + (0 - outsidePoints[0].z) * (insidePoints[0].v - outsidePoints[0].v) / (insidePoints[0].z - outsidePoints[0].z);
			extraPoint1.w = outsidePoints[0].w + (0 - outsidePoints[0].z) * (insidePoints[0].w - outsidePoints[0].w) / (insidePoints[0].z - outsidePoints[0].z);

			Point3D extraPoint2;
			extraPoint2.x = outsidePoints[1].x + (0 - outsidePoints[1].z) * (insidePoints[0].x - outsidePoints[1].x) / (insidePoints[0].z - outsidePoints[1].z);
			extraPoint2.y = outsidePoints[1].y + (0 - outsidePoints[1].z) * (insidePoints[0].y - outsidePoints[1].y) / (insidePoints[0].z - outsidePoints[1].z);
			extraPoint2.z = 0;
			extraPoint2.u = outsidePoints[1].u + (0 - outsidePoints[1].z) * (insidePoints[0].u - outsidePoints[1].u) / (insidePoints[0].z - outsidePoints[1].z);
			extraPoint2.v = outsidePoints[1].v + (0 - outsidePoints[1].z) * (insidePoints[0].v - outsidePoints[1].v) / (insidePoints[0].z - outsidePoints[1].z);
			extraPoint2.w = outsidePoints[1].w + (0 - outsidePoints[1].z) * (insidePoints[0].w - outsidePoints[1].w) / (insidePoints[0].z - outsidePoints[1].z);

			if (!pointsAreOutside[0]) {
				toReturn.push_back(new Triangle(extraPoint2, insidePoints[0], extraPoint1));
			}
			else if (!pointsAreOutside[1]) {
				toReturn.push_back(new Triangle(extraPoint1, insidePoints[0], extraPoint2));
			}
			else if (!pointsAreOutside[2]) {
				toReturn.push_back(new Triangle(extraPoint2, insidePoints[0], extraPoint1));
			}

		}
	}

	return toReturn;
}
