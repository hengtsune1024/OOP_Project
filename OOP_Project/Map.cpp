#include "Map.h"

Map::Map() : lines(NUM_LINE),
number_of_lines(NUM_LINE), camDepth(DEFAULT_CAMERA_DEPTH), posX(INITIAL_POS* SEGMENT_LENGTH),
velAngular(0), velLinear(0), roadDegree(0), camDegree(0), accLinear(0), camHeight(CAMERA_HEIGHT)
{}

Map::Map(SDL_Renderer* renderer) : car("../images/pooh/", 22, renderer), lines(NUM_LINE),
	number_of_lines(NUM_LINE), camDepth(DEFAULT_CAMERA_DEPTH), posX(INITIAL_POS* SEGMENT_LENGTH), 
	velAngular(0), velLinear(0), roadDegree(0), camDegree(0), accLinear(0), camHeight(CAMERA_HEIGHT)
{
	double x = 0, dx = 0;
	for (int i = 0; i < NUM_LINE; ++i) {

		if (i > 100 && i < 700)		// range of turing road
			lines[i].setCurve(0.9);
		else if (i > 800 && i < 1400)
			lines[i].setCurve(-1.5);
		else if (i > 1500 && i < 2000)
			lines[i].setCurve(1.2);
		else if (i > 2200 && i < 2800)
			lines[i].setCurve(-1.3);
		else
			lines[i].setCurve(0);

		if (i > 300 && i < 1054)		//range of road up and down
			lines[i].sety(sin((i - 300) / 30.0) * CAMERA_HEIGHT);
		else if (i > 1200 && i < 2896)
			lines[i].sety(sin((i - 1200) / 20.0) * CAMERA_HEIGHT);
		else
			lines[i].sety(0);

		x += dx;
		lines[i].setx(x);

		lines[i].setz(i * SEGMENT_LENGTH);

		dx += lines[i].getCurve();
	}

	for (int i = INITIAL_POS - 10; i < INITIAL_POS + 22; ++i)
		lines[i].setType(ENDPOINT);
	for (int i = FINAL_POS - 12; i < FINAL_POS + 20; ++i)
		lines[i].setType(ENDPOINT);

	posY = lines[INITIAL_POS].getx();
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

void Map::rush()
{
	if (car.getFullEnergy()) {

		velLinear = RUSHBEGIN_SPEED;
		camDepth = BEGINRUSH_CAMDEPTH;
		car.rush(true);
		std::cout << "[Map] rush start" << endl;
	}
	else {
		std::cout << "[Map] not enough energy :" << car.getEnergy() << endl;
	}
}

void Map::draw(SDL_Renderer* renderer) 
{
	int startpos = posX / SEGMENT_LENGTH;
	int camH = camHeight + lines[startpos].gety();
	int maxy = HEIGHT;

	//road and ground
	Uint32 grass, rumble, road;

	boxColor(renderer, 0, HEIGHT / 2, WIDTH, HEIGHT, 0xff10c810);
	for (int i = startpos - 50; i < startpos + 300; ++i) {

		if (i < 0){
			i = -1;
			continue;
		}
		else if (i >= number_of_lines)
			break;

		Line& l = lines[i % number_of_lines];
		Line p = lines[(i - 1 + number_of_lines) % number_of_lines];
		l.project(posY, camH, posX, camDegree, camDepth);

		if (l.getW() < 1e-6 && l.getW() > -1e-6)
			continue;

		if (l.getY() >= maxy)
			continue;

		maxy = l.getY();

		grass = (i >> 2) & 1 ? 0xff10c810 : 0xff009A00;
		drawQuad(renderer, { grass,  WIDTH / 2, p.getY(), WIDTH / 2, WIDTH / 2, l.getY(), WIDTH / 2 });

		switch (lines[i].getType()) 
		{
			case NORMAL:

				rumble = (i >> 2) & 1 ? 0xffffffff : 0xff000000;
				road = (i >> 2) & 1 ? 0xff6b6b6b : 0xff696969;
				drawQuad(renderer, { rumble, p.getX(), p.getY(), p.getW() * 1.2, l.getX(), l.getY(), l.getW() * 1.2 });
				drawQuad(renderer, { road, p.getX(), p.getY(), p.getW(), l.getX(), l.getY(), l.getW() });
				break;

			case ENDPOINT:

				double width_scale = 1.2;
				for (int j = 0; j <= 7; ++j) {

					width_scale = 1.2 * (15 - (j << 1)) / 15;
					rumble = ((i >> 2) + j) & 1 ? 0xffffffff : 0xff000000;

					drawQuad(renderer, { rumble,p.getX(), p.getY(), p.getW() * width_scale, l.getX(), l.getY(), l.getW() * width_scale });
				}
				break;
		}
	}

	//car
	car.draw(renderer);

}

Uint32 Map::move(Uint32 interval, void* para) {
	Map* mp = (Map*)para;

	//current index of road line
	int startpos = mp->posX / SEGMENT_LENGTH;

	//degree between road vector and normal line (same direction as camera degree)
	mp->roadDegree = atan((mp->lines[startpos + 1].getx() - mp->lines[startpos].getx()) / (mp->lines[startpos + 1].getz() - mp->lines[startpos].getz()));
	
	double velX, velY;
	velX = mp->velLinear * cos(mp->camDegree);
	velY = mp->velLinear * sin(mp->camDegree);
	
	//move in x-direction
	mp->posX += velX;
	if (mp->posX < 0 || mp->posX >= mp->number_of_lines * SEGMENT_LENGTH)
		mp->posX -= velX;	


	//move in y-direction
	mp->posY += velY;
	
	if ((mp->posY < mp->lines[startpos].getx() - 1.5 * ROAD_WIDTH && velY < 0) || (mp->posY > mp->lines[startpos].getx() + 1.5 * ROAD_WIDTH && velY > 0)) 
	{
		
		mp->posY -= velY;
		mp->posX -= velX;

		double velProjected = mp->velLinear * cos(mp->roadDegree - mp->camDegree);
		mp->posX += velProjected * cos(mp->roadDegree);
		mp->posY += velProjected * sin(mp->roadDegree);
	}

	if (startpos % 100 == 0)
		std::cout << startpos <<" "<<velX <<" "<<mp->camDegree << endl;

	//rotate camera
	mp->camDegree += mp->velAngular;
	
	if((mp->camDegree <= mp->roadDegree - MAX_ROTATE_DEGREE && mp->velAngular < 0) || (mp->camDegree >= mp->roadDegree + MAX_ROTATE_DEGREE && mp->velAngular >= 0))
		mp->camDegree -= mp->velAngular;

	return interval;
}

Uint32 Map::accelerate(Uint32 interval, void* para) 
{
	Map* mp = (Map*)para;

	if (mp->car.getRushing()) 
	{
		mp->velLinear -= AFTERRUSH_SPEED_DECREASE;
		if (mp->velLinear < MAX_FORWARD_SPEED) {
			mp->velLinear = MAX_FORWARD_SPEED;
			mp->car.rush(false);
		}
		mp->camDepth += AFTERRUSH_CAMDEPTH_RECOVER;
		if (mp->camDepth > DEFAULT_CAMERA_DEPTH) {
			mp->camDepth = DEFAULT_CAMERA_DEPTH;
		}
	}
	else 
	{
		mp->velLinear += mp->accLinear;
		if (mp->velLinear > MAX_FORWARD_SPEED || mp->velLinear < -MAX_BACKWARD_SPEED) {
			mp->velLinear -= mp->accLinear;
		}

		if ((mp->accLinear == -FRICTION_ACC && mp->velLinear < 0) || (mp->accLinear == FRICTION_ACC && mp->velLinear > 0)) {
			mp->velLinear = 0;
			mp->accLinear = 0;
		}

	}

	return interval;
}



void Map::turn(int d)
{
	car.turn(d);
}

void Map::startTimer() {

	moveTimer = SDL_AddTimer(MOVE_INTERVAL, move, this);
	accelerateTimer = SDL_AddTimer(ACCELERATE_INTERVAL, accelerate, this);

	car.startTimer(CAR_INTERVAL);
}

void Map::removeTimer() {
	SDL_RemoveTimer(moveTimer);
	SDL_RemoveTimer(accelerateTimer);
	car.stopTimer();
}

/*
void Map::init() {
}

	if ((mp->camDegree <= mp->roadDegree - MAX_ROTATE_DEGREE || mp->velAngular >= 0) && (mp->camDegree >= mp->roadDegree + MAX_ROTATE_DEGREE || mp->velAngular < 0)
		&& (mp->camDegree >= mp->roadDegree + MAX_ROTATE_DEGREE || mp->camDegree <= mp->roadDegree - MAX_ROTATE_DEGREE))
			mp->camDegree -= mp->velAngular;
	


*/