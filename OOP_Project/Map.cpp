#include "Map.h"

Map::Map() {

}

Map::Map(SDL_Renderer* renderer) :number_of_lines(NUM_LINE), car("../images/pooh/", 22, renderer), lines(NUM_LINE),
	camDegree(0), velAngular(0), velLinear(0), posX(INITIAL_POS* SEGMENT_LENGTH)
{
	double x = 0, dx = 0;
	for (int i = 0; i < NUM_LINE; ++i) {

		if (i > 100 && i < 700)		// range of turing road
			lines[i].setCurve(0.9);
		else if (i > 800 && i < 1400)
			lines[i].setCurve(-1.5);
		else
			lines[i].setCurve(0);

		if (i > 300 && i < 1000)		//range of road up and down
			lines[i].sety(sin(i / 30.0) * CAMERA_HEIGHT);
		else
			lines[i].sety(0);

		x += dx;
		dx += lines[i].getCurve();
		lines[i].setx(x);
		lines[i].setz(i * SEGMENT_LENGTH);
	}
	posY = lines[300].getx();
	std::cout << "[Map] Map initialized" << endl;


	car.setPosition(280, 380);
	car.turn(0);
}

Map::~Map() {

}

void Map::quit() {
	removeTimer();
	car.quit();
	//delete[]car;
	std::cout << "[Map] Map closed" << endl;
}

void Map::drawQuad(SDL_Renderer* renderer, Quad q) {
	Sint16 vx[4] = { q.x1 - q.w1,q.x2 - q.w2,q.x2 + q.w2,q.x1 + q.w1 };
	Sint16 vy[4] = { q.y1,q.y2 ,q.y2 ,q.y1 };
	filledPolygonColor(renderer, vx, vy, 4, q.color);
}

void Map::draw(SDL_Renderer* renderer) {
	int startpos = posX / SEGMENT_LENGTH;
	int camH = CAMERA_HEIGHT + lines[startpos].gety();
	int maxy = HEIGHT;

	Uint32 grass, rumble, road;

	for (int i = startpos - 50; i < startpos + 300; ++i) {

		if (i < 0){
			i = -1;
			continue;
		}
		else if (i >= number_of_lines)
			break;

		Line& l = lines[i % number_of_lines];
		Line p = lines[(i - 1 + number_of_lines) % number_of_lines];
		l.project(posY, camH, posX, camDegree);

		if (l.getW() < 1e-6 && l.getW() > -1e-6)
			continue;

		if (l.getY() >= maxy)
			continue;

		maxy = l.getY();

		grass = (i / 3) & 1 ? 0xff10c810 : 0xff009A00;
		rumble = (i / 3) & 1 ? 0xffffffff : 0xff000000;
		road = (i / 3) & 1 ? 0xff6b6b6b : 0xff696969;

		drawQuad(renderer, { grass,  WIDTH / 2, p.getY(), WIDTH / 2, WIDTH / 2, l.getY(), WIDTH / 2 });
		drawQuad(renderer, { rumble, p.getX(), p.getY(), p.getW() * 1.2, l.getX(), l.getY(), l.getW() * 1.2 });
		drawQuad(renderer, { road, p.getX(), p.getY(), p.getW(), l.getX(), l.getY(), l.getW() });
	}

	car.draw(renderer);

}

Uint32 Map::move(Uint32 interval, void* para) {
	Map* mp = (Map*)para;

	double velX = mp->velLinear * cos(mp->camDegree), velY = mp->velLinear * sin(mp->camDegree);

	//move in x-direction
	mp->posX += velX;
	if (mp->posX < 0 || mp->posX >= mp->number_of_lines * SEGMENT_LENGTH)
		mp->posX -= velX;	

	//current index of road line
	int startpos = mp->posX / SEGMENT_LENGTH;

	//degree between road vector and normal line (same direction as camera degree)
	mp->roadDegree = atan((mp->lines[startpos + 1].getx() - mp->lines[startpos].getx()) / (mp->lines[startpos + 1].getz() - mp->lines[startpos].getz()));

	//move in y-direction
	mp->posY += velY;
	if (mp->posY < mp->lines[startpos].getx() - 2 * ROAD_WIDTH || mp->posY > mp->lines[startpos].getx() + 2 * ROAD_WIDTH){
		
		mp->posY -= velY;
		mp->posX -= velX;

		double velProjected = mp->velLinear * cos(mp->roadDegree - mp->camDegree);
		mp->posX += velProjected * cos(mp->roadDegree);
		mp->posY += velProjected * sin(mp->roadDegree);
	}


	//rotate camera
	mp->camDegree += mp->velAngular;
	if (mp->camDegree >= mp->roadDegree + MAX_ROTATE_DEGREE || mp->camDegree  <= mp->roadDegree - MAX_ROTATE_DEGREE)
		mp->camDegree -= mp->velAngular;

	return mp->moveInterval;
}
void Map::turn(int d)
{
	car.turn(d);
}

void Map::startTimer(Uint32 interval) {
	moveInterval = interval;
	moveTimer = SDL_AddTimer(interval, move, this);

	car.startTimer(interval);
}

void Map::removeTimer() {
	SDL_RemoveTimer(moveTimer);
	car.stopTimer();
}

/*
void Map::init() {
}
*/