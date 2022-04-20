#include "Map.h"

Map::Map() : lines(NUM_LINE),
number_of_lines(NUM_LINE), camDepth(DEFAULT_CAMERA_DEPTH), posX(INITIAL_POS* SEGMENT_LENGTH),
velAngular(0), velLinear(0), roadDegree(0), camDegree(0), accLinear(0), camHeight(CAMERA_HEIGHT)
{}

Map::Map(SDL_Renderer* renderer) : car("../images/pooh/", 22, renderer), lines(NUM_LINE), tree("../images/1.png", renderer),
	number_of_lines(NUM_LINE), camDepth(DEFAULT_CAMERA_DEPTH), posX(INITIAL_POS* SEGMENT_LENGTH), 
	velAngular(0), velLinear(0), roadDegree(0), camDegree(0), accLinear(0), camHeight(CAMERA_HEIGHT)
{
	double x = 0, dx = 0;

	for (int i = 0; i < NUM_LINE; ++i) {

		//curve
		if (i > 100 && i < 700)		// range of turing road
			lines[i].setCurve(0.9);
		else if (i > 800 && i < 1400)
			lines[i].setCurve(-1.5);
		else if (i > 1500 && i < 2000)
			lines[i].setCurve(1.2);
		else if (i > 2200 && i < 2800)
			lines[i].setCurve(-1.3);
		
		if (i > 300 && i < 600)
			lines[i].setCurve(8.5);
		else if (i > 700 && i < 1000)
			lines[i].setCurve(-8.5);
			
		// y
		if (i > 300 && i < 1054)		//range of road up and down
			lines[i].sety(sin((i - 300) / 30.0) * CAMERA_HEIGHT);
		else if (i > 1200 && i < 2896)
			lines[i].sety(sin((i - 1200) / 20.0) * CAMERA_HEIGHT);
		else
			lines[i].sety(0);

		// x
		x += dx;
		dx += lines[i].getCurve();
		lines[i].setx(x);

		// z
		lines[i].setz(i * SEGMENT_LENGTH);

		//sprite
		if ((i & 31) == 0) {    // same as i % 32
			lines[i].setSprite(&tree, 2.5);
		}
	}

	//type
	for (int i = INITIAL_POS - 10; i < INITIAL_POS + 22; ++i)
		lines[i].setType(ENDPOINT);
	for (int i = FINAL_POS - 12; i < FINAL_POS + 20; ++i)
		lines[i].setType(ENDPOINT);

	for (int i = 300; i < 300 + ACCROAD_LENGHT; ++i)
		lines[i].setType(ACCELERATE_RIGHT);
	for (int i = 1000; i < 1000 + ACCROAD_LENGHT; ++i)
		lines[i].setType(ACCELERATE_LEFT);

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

void Map::rush(RushType type)
{
	switch (type) 
	{
		case ENERGY:
			if (car.getFullEnergy()) {
				velLinear = ENERGY_RUSHBEGIN_SPEED;
				camDepth = BEGINRUSH_CAMDEPTH;
				car.rush(ENERGY);
				std::cout << "[Map] rush start" << endl;
			}
			else {
				std::cout << "[Map] not enough energy :" << car.getEnergy() << endl;
			}
			break;

		case ACCROAD:
			velLinear = ACCROAD_RUSHBEGIN_SPEED;
			camDepth = BEGINRUSH_CAMDEPTH;
			car.rush(ACCROAD);
			std::cout << "[Map] rush start" << endl;
			break;
		case TOOL:
			break;
		default:
			break;
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

		if (i < 1){
			i = 0;
			continue;
		}
		else if (i >= number_of_lines)
			break;

		Line& l = lines[i];
		Line p = lines[i - 1];
		l.project(posY, camH, posX, camDegree, camDepth);

		if (l.getW() < 1e-6 && l.getW() > -1e-6)
			continue;

		l.setClip(maxy);
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
			{
				double width_scale = 1.2;
				for (int j = 0; j <= 7; ++j) {

					width_scale = 1.2 * (15 - (j << 1)) / 15;
					rumble = ((i >> 2) + j) & 1 ? 0xffffffff : 0xff000000;

					drawQuad(renderer, { rumble,p.getX(), p.getY(), p.getW() * width_scale, l.getX(), l.getY(), l.getW() * width_scale });
				}
			}
				break;

			case ACCELERATE_RIGHT:
			case ACCELERATE_LEFT:
			{
				rumble = (i >> 2) & 1 ? 0xffffffff : 0xff000000;
				road = (i >> 2) & 1 ? 0xff6b6b6b : 0xff696969;
				Uint32 accRoad = (i >> 1) & 1 ? 0xff00ffff : 0xff0000ff;

				int sign = lines[i].getType() == ACCELERATE_RIGHT ? 1 : -1;

				drawQuad(renderer, { rumble, p.getX(), p.getY(), p.getW() * 1.2, l.getX(), l.getY(), l.getW() * 1.2 });
				drawQuad(renderer, { road, p.getX(), p.getY(), p.getW(), l.getX(), l.getY(), l.getW() });
				drawQuad(renderer, { accRoad, p.getX() + sign * p.getW() / 2, p.getY(), p.getW() / 2, l.getX() + sign * l.getW() / 2, l.getY(), l.getW() / 2 });
			}
				break;
		}
	}
	//sprite
	for (int i = startpos + 300; i > startpos; --i) {

		if (i < 1) {
			i = 0;
			continue;
		}
		else if (i >= number_of_lines)
			break;

		lines[i].drawSprite(renderer);
	}

	//car
	car.draw(renderer);

}

Uint32 Map::move(Uint32 interval, void* para) {
	Map* mp = (Map*)para;
	
	int startpos = mp->posX / SEGMENT_LENGTH;
	mp->roadDegree = atan((mp->lines[startpos + 1].getx() - mp->lines[startpos].getx()) / (mp->lines[startpos + 1].getz() - mp->lines[startpos].getz()));

	double velM = (sin(mp->roadDegree) * (mp->lines[startpos + 1].getx() - mp->lines[startpos].getx()) + cos(mp->roadDegree) * SEGMENT_LENGTH) / SEGMENT_LENGTH;
	cout << mp->lines[startpos + 1].getx() - mp->lines[startpos].getx() << " " << velM << " ";
	
	double velX, velY;
	
	velX = mp->velLinear * cos(mp->camDegree) * velM;
	velY = mp->velLinear * sin(mp->camDegree) * velM;
	
	//move in x-direction
	mp->posX += velX;
	if (mp->posX < 0 || mp->posX >= mp->number_of_lines * SEGMENT_LENGTH)
		mp->posX -= velX;	

	/********* Do not move these codes ********/

	//current index of road line
	startpos = mp->posX / SEGMENT_LENGTH;

	//degree between road vector and normal line (same direction as camera degree)
	mp->roadDegree = atan((mp->lines[startpos + 1].getx() - mp->lines[startpos].getx()) / (mp->lines[startpos + 1].getz() - mp->lines[startpos].getz()));

	 /********* Or there will be bugs ***********/

	//move in y-direction
	mp->posY += velY;
	
	if ((mp->posY < mp->lines[startpos].getx() - 2 * ROAD_WIDTH) || (mp->posY > mp->lines[startpos].getx() + 2 * ROAD_WIDTH)) 
	{
		
		mp->posY -= velY;
		mp->posX -= velX;
		//mp->posY = mp->lines[startpos].getx() + 2 * ROAD_WIDTH;
		//mp->posX = mp->lines[startpos].getz();
		
		double velProjected = mp->velLinear * cos(mp->roadDegree - mp->camDegree) * velM;
		mp->posX += velProjected * cos(mp->roadDegree);
		mp->posY += velProjected * sin(mp->roadDegree);
		
		int endpos = mp->posX / SEGMENT_LENGTH;
		if (mp->posY < mp->lines[endpos].getx() - 2 * ROAD_WIDTH)
			mp->posY = mp->lines[endpos].getx() - 2 * ROAD_WIDTH;
		else if (mp->posY > mp->lines[endpos].getx() + 2 * ROAD_WIDTH)
			mp->posY = mp->lines[endpos].getx() + 2 * ROAD_WIDTH;
		
	}
	

	//rotate camera
	mp->camDegree += mp->velAngular / velM ;
	
	if((mp->camDegree <= mp->roadDegree - MAX_ROTATE_DEGREE && mp->velAngular < 0) || (mp->camDegree >= mp->roadDegree + MAX_ROTATE_DEGREE && mp->velAngular >= 0))
		mp->camDegree -= mp->velAngular;

	cout << mp->velAngular / velM << endl;

	//special road
	if (mp->car.getRushing() != ACCROAD && (mp->lines[startpos].getType() == ACCELERATE_LEFT || mp->lines[startpos].getType() == ACCELERATE_RIGHT)) {
		if (mp->lines[startpos].getType() == ACCELERATE_LEFT && (mp->posY < mp->lines[startpos].getx() && mp->posY > mp->lines[startpos].getx() - ROAD_WIDTH)) {
			mp->rush(ACCROAD);
		}
		else if (mp->lines[startpos].getType() == ACCELERATE_RIGHT && (mp->posY > mp->lines[startpos].getx() && mp->posY < mp->lines[startpos].getx() + ROAD_WIDTH)) {
			mp->rush(ACCROAD);
		}
	}
	return interval;
}

Uint32 Map::accelerate(Uint32 interval, void* para) 
{
	Map* mp = (Map*)para;

	if (mp->car.getRushing()) //excpet RushType == NONE(0), other types will go here
	{
		mp->velLinear -= AFTERRUSH_SPEED_DECREASE;
		if (mp->velLinear < MAX_FORWARD_SPEED) {
			mp->velLinear = MAX_FORWARD_SPEED;
			if (mp->accLinear == 0)
				mp->accLinear = -FRICTION_ACC;
			mp->car.rush(NONE);
		}
		mp->camDepth += AFTERRUSH_CAMDEPTH_RECOVER;
		if (mp->camDepth > DEFAULT_CAMERA_DEPTH) {
			mp->camDepth = DEFAULT_CAMERA_DEPTH;
		}
	}
	else 
	{
		mp->velLinear += mp->accLinear;
		if (mp->velLinear > MAX_FORWARD_SPEED) {
			mp->velLinear = MAX_FORWARD_SPEED;
		}
		else if (mp->velLinear < -MAX_BACKWARD_SPEED) {
			mp->velLinear = -MAX_BACKWARD_SPEED;
		}

		if ((mp->accLinear == -FRICTION_ACC && mp->velLinear < 0) || (mp->accLinear == FRICTION_ACC && mp->velLinear > 0)) {
			mp->velLinear = 0;
			mp->accLinear = 0;
		}

	}
	//cout << mp->accLinear << " " << mp->velLinear << endl;
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