#include "Map.h"

Map::Map() {

}

Map::~Map() {

}

void Map::init() {
	Line tmp;
	double x = 0, dx = 0;
	for (int i = 0; i < NUM_LINE; ++i) {
		
		if (i > 100 && i < 700)		//range of turing road
			tmp.setCurve(0.9);
		else if (i > 800 && i < 1400)
			tmp.setCurve(-1.5);
		else
			tmp.setCurve(0);

		if (i > 300 && i < 1000)		//range of road up and down
			tmp.sety(sin(i / 30.0) * CAMERA_HEIGHT);
		else
			tmp.sety(0);

		/*
		if (i > 1100) line.curve = -0.7;

		if (i < 300 && i % 20 == 0) { line.spriteX = -2.5; line.sprite = object[5]; }
		if (i % 17 == 0) { line.spriteX = 2.0; line.sprite = object[6]; }
		if (i > 300 && i % 20 == 0) { line.spriteX = -0.7; line.sprite = object[4]; }
		if (i > 800 && i % 20 == 0) { line.spriteX = -1.2; line.sprite = object[1]; }
		if (i == 400) { line.spriteX = -1.2; line.sprite = object[7]; }

		*/
		x += dx;
		dx += tmp.getCurve();
		tmp.setx(x);
		tmp.setz(i * SEGMENT_LENGTH);
		lines.push_back(tmp);
	}
	camDegree = 0;
	N = lines.size();
	posX = 0;
	std::cout << "[Map] Map initialized" << endl;
}

void Map::quit() {
	removeTimer();
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
	int pub;

	lines[(startpos + N - 1)%N].project(posY, camH, posX, camDegree);

	for (int i = startpos; i < startpos + 300; ++i) {

		Line& l = lines[i % N];
		Line p = lines[(i - 1 + N) % N];
		l.project(posY, camH, posX , camDegree);
		grass = (i / 3) & 1 ? 0xff10c810 : 0xff009A00;
		drawQuad(renderer, { grass,  WIDTH / 2, p.getY(), WIDTH / 2, WIDTH / 2, l.getY(), WIDTH / 2 });

	}
	for (int i = startpos; i < startpos + 300; ++i) {

		if (i >= N)
			pub = N * SEGMENT_LENGTH;
		else
			pub = 0;
		Line& l = lines[i % N];
		l.project(posY, camH, posX - pub,camDegree);
		rumble = (i / 3) & 1 ? 0xffffffff : 0xff000000;
		road = (i / 3) & 1 ? 0xff6b6b6b : 0xff696969;

		if (l.getY() >= maxy)
			continue;
		maxy = l.getY();

		Line p = lines[(i - 1 + N) % N];
		double a[3] = { i,startpos, camDegree };
		drawQuad(renderer, { rumble, p.getX(), p.getY(), p.getW() * 1.2, l.getX(), l.getY(), l.getW() * 1.2});
		drawQuad(renderer, { road, p.getX(), p.getY(), p.getW(), l.getX(), l.getY(), l.getW()});
	}
}
Uint32 Map::move(Uint32 interval, void* para) {
	Map* mp = (Map*)para;

	double co = cos(mp->camDegree), si = sin(mp->camDegree);

	mp->posX += mp->velX* co;
	
	if(mp->posX >= mp->N*SEGMENT_LENGTH)
		mp->posX-= mp->N * SEGMENT_LENGTH;
	else if(mp->posX <0)
		mp->posX += mp->N * SEGMENT_LENGTH;

	mp->posY += mp->velX *si;
	
	mp->camDegree += mp->velY;
	if (mp->camDegree > 1.3 || mp->camDegree < -1.3)
		mp->camDegree -= mp->velY;

	return mp->moveInterval;
}
void Map::startTimer(Uint32 interval) {
	moveInterval = interval;
	moveTimer = SDL_AddTimer(interval, move, this);
}

void Map::removeTimer() {
	SDL_RemoveTimer(moveTimer);
}