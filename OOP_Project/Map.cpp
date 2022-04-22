#include "Map.h"

Map::Map() : lines(NUM_LINE),
number_of_lines(NUM_LINE), camDepth(DEFAULT_CAMERA_DEPTH), posX(INITIAL_POS* SEGMENT_LENGTH),
velAngular(0), velLinear(0), roadDegree(0), camDegree(0), accLinear(0), camHeight(CAMERA_HEIGHT), velM(1)
{}

Map::Map(SDL_Renderer* renderer) : car("../images/pooh/", 22, renderer), lines(NUM_LINE), tree("../images/1.png", renderer),
	number_of_lines(NUM_LINE), camDepth(DEFAULT_CAMERA_DEPTH), posX(INITIAL_POS* SEGMENT_LENGTH), 
	velAngular(0), velLinear(0), roadDegree(0), camDegree(0), accLinear(0), camHeight(CAMERA_HEIGHT), velM(1), virus("../images/coronavirus/", 15, renderer)
{
	double x = 0, dx = 0;

	for (int i = 0; i < NUM_LINE; ++i) {

		//curve
		if (i > 100 && i <= 300)		// range of turing road
			lines[i].setCurve(0.9);
		else if (i > 300 && i < 600)
			lines[i].setCurve(8.5);
		else if (i > 700 && i < 1000)
			lines[i].setCurve(-8.5);
		else if (i > 1200 && i < 1400)
			lines[i].setCurve(-1.5);
		else if (i > 1500 && i < 2000)
			lines[i].setCurve(1.2);
		else if (i > 2200 && i < 2800)
			lines[i].setCurve(-1.3);
		
			
		// y
		if (i > 300 && i < 1054)		//range of road up and down
			lines[i].sety(sin((i - 300) / 30.0) * CAMERA_HEIGHT);
		else if (i > 1200 && i < 2896)
			lines[i].sety(sin((i - 1200) / 20.0) * (CAMERA_HEIGHT * 0.6));
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

		if (i == 300)
			virus.setTrap(&lines[i]);
		
	}

	//type
	for (int i = INITIAL_POS - 10; i < INITIAL_POS + 22; ++i)
		lines[i].setType(ENDPOINT);
	for (int i = FINAL_POS - 12; i < FINAL_POS + 20; ++i)
		lines[i].setType(ENDPOINT);

	for (int i = 650; i < 650 + ACCROAD_LENGHT; ++i)
		lines[i].setType(ACCELERATE_RIGHT);
	for (int i = 1800; i < 1800 + ACCROAD_LENGHT; ++i)
		lines[i].setType(ACCELERATE_LEFT);
	
	for(int i=295;i<300;i++)
		lines[i].setType(TRAPAREA);

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
	virus.quit();
	tree.close();
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
	Uint32 grass, rumble, road, laneLine;
	laneLine = 0xffffffff;
	bool f = true;

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
		l.project(posY, camH, posX, camDegree, camDepth, roadDegree);
		//l.project(lines[startpos+5].getx(), camH, lines[startpos+5].getz(), camDegree, camDepth, roadDegree);
		if (l.getW() < 1e-6 && l.getW() > -1e-6)
			continue;

		l.setClip(maxy);
		if (l.getY() >= maxy)
			continue;

		maxy = l.getY();

		//grass
		grass = (i >> 2) & 1 ? 0xff10c810 : 0xff009A00;
		drawQuad(renderer, { grass,  WIDTH / 2, p.getY(), WIDTH / 2, WIDTH / 2, l.getY(), WIDTH / 2 });

		//road type 
		switch (lines[i].getType()) 
		{
			case NORMAL:
			case TRAPAREA:
				rumble = (i >> 2) & 1 ? 0xffffffff : 0xff000000;
				road = (i >> 2) & 1 ? 0xff6b6b6b : 0xff696969;
				drawQuad(renderer, { rumble, p.getX(), p.getY(), p.getW() * 1.2, l.getX(), l.getY(), l.getW() * 1.2 });
				drawQuad(renderer, { road, p.getX(), p.getY(), p.getW(), l.getX(), l.getY(), l.getW() });

				if ((i >> 3) & 1) {
					drawQuad(renderer, { laneLine, p.getX(), p.getY(), p.getW() * LANELINE_WIDTH / ROAD_WIDTH, l.getX(), l.getY(),l.getW() * LANELINE_WIDTH / ROAD_WIDTH });
				}
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

				if ((i >> 3) & 1) {
					drawQuad(renderer, { laneLine, p.getX(), p.getY(), p.getW() * LANELINE_WIDTH / ROAD_WIDTH, l.getX(), l.getY(),l.getW() * LANELINE_WIDTH / ROAD_WIDTH });
				}
			}
				break;
			
			/*case TRAPAREA:
			{

			}*/

		}
	}

	//sprite
	for (int i = startpos + 300; i > startpos; --i) {

		if (i >= number_of_lines) {
			i = number_of_lines;
			continue;
		}
		else if (i < 1)
			break;

		lines[i].drawSprite(renderer);
		//filledCircleColor(renderer, lines[i].getX(), lines[i].getY(), 2, 0xffffffff);
		
		virus.draw(renderer, &lines[i]);
		//lines[i].drawActSprite(renderer, 0);
	}

	//car
	car.draw(renderer);

}

Uint32 Map::move(Uint32 interval, void* para) {
	Map* mp = (Map*)para;
	
	//velocity modification
	int startpos = mp->posX / SEGMENT_LENGTH;
	mp->roadDegree = atan((mp->lines[startpos + 1].getx() - mp->lines[startpos].getx()) / SEGMENT_LENGTH);

	mp->velM = (sin(mp->roadDegree) * (mp->lines[startpos + 1].getx() - mp->lines[startpos].getx()) + cos(mp->roadDegree) * SEGMENT_LENGTH) / SEGMENT_LENGTH;

	//speed punishment
	double punish = 1;
		//cout << mp->lines[startpos + 1].getx() - mp->lines[startpos].getx() << " " << velM << " ";
	if (mp->posY > mp->lines[startpos].getx() + ROAD_WIDTH * mp->velM || mp->posY < mp->lines[startpos].getx() - ROAD_WIDTH * mp->velM) {
		punish =   (ROAD_WIDTH * mp->velM)/ (mp->posY - mp->lines[startpos].getx());
		if (punish < 0)
			punish = -punish;
	}

	double velX, velY;
	velX = mp->velLinear * cos(mp->camDegree) * mp->velM * punish;
	velY = mp->velLinear * sin(mp->camDegree) * mp->velM * punish;
	
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
	
	if ((mp->posY < mp->lines[startpos].getx() - ROAD_BORDER * mp->velM) || (mp->posY > mp->lines[startpos].getx() + ROAD_BORDER * mp->velM))
	{
		mp->posY -= velY;
		mp->posX -= velX;

		int originpos = mp->posX / SEGMENT_LENGTH;		//startpos after moving, endpos is original
		if (originpos == startpos)
			++originpos;

		double roadD = atan((mp->lines[startpos].getx() - mp->lines[originpos].getx()) / (mp->lines[startpos].getz() - mp->lines[originpos].getz()));

		double velProjected = mp->velLinear * cos(roadD - mp->camDegree) * mp->velM * punish;
		mp->posX += velProjected * cos(roadD);
		mp->posY += velProjected * sin(roadD);

		originpos = mp->posX / SEGMENT_LENGTH;
		if (mp->posY < mp->lines[originpos].getx() - ROAD_BORDER * mp->velM)
			mp->posY = mp->lines[originpos].getx() - ROAD_BORDER * mp->velM;
		else if (mp->posY > mp->lines[originpos].getx() + ROAD_BORDER * mp->velM)
			mp->posY = mp->lines[originpos].getx() + ROAD_BORDER * mp->velM;
		
	}
	//rotate camera
	mp->camDegree += mp->velAngular / mp->velM;
	
	if ((mp->camDegree <= mp->roadDegree - MAX_ROTATE_DEGREE / mp->velM && mp->velAngular <= 0) || (mp->camDegree >= mp->roadDegree + MAX_ROTATE_DEGREE / mp->velM && mp->velAngular >= 0)) {
		mp->camDegree -= mp->velAngular / mp->velM;
		if (mp->camDegree <= mp->roadDegree - MAX_ROTATE_DEGREE / mp->velM) {
			mp->camDegree = mp->roadDegree - MAX_ROTATE_DEGREE / mp->velM;
		}
		else if (mp->camDegree >= mp->roadDegree + MAX_ROTATE_DEGREE / mp->velM) {
			mp->camDegree = mp->roadDegree + MAX_ROTATE_DEGREE / mp->velM;
		}
	}

	//special road
	if (mp->car.getRushing() != ACCROAD && (mp->lines[startpos].getType() == ACCELERATE_LEFT || mp->lines[startpos].getType() == ACCELERATE_RIGHT)) {
		if (mp->lines[startpos].getType() == ACCELERATE_LEFT && (mp->posY < mp->lines[startpos].getx() && mp->posY > mp->lines[startpos].getx() - ROAD_WIDTH * mp->velM)) {
			mp->rush(ACCROAD);
		}
		else if (mp->lines[startpos].getType() == ACCELERATE_RIGHT && (mp->posY > mp->lines[startpos].getx() && mp->posY < mp->lines[startpos].getx() + ROAD_WIDTH * mp->velM)) {
			mp->rush(ACCROAD);
		}
	}

	//trap
	if (mp->lines[startpos].getType() == TRAPAREA && mp->posY < mp->lines[startpos].getx() + TRAP_WIDTH * mp->velM && mp->posY > mp->lines[startpos].getx() - TRAP_WIDTH * mp->velM)
		mp->virus.gettrap(STAIN);

	
	
	//cout << mp->camDegree - mp->roadDegree << endl;
	return interval;
}

Uint32 Map::accelerate(Uint32 interval, void* para) 
{
	Map* mp = (Map*)para;

	if (mp->car.getRushing()) //excpet RushType == NONE(0), other types will go here
	{
		double speedDecrease = AFTERRUSH_SPEED_DECREASE;
		if (mp->accLinear < 0)
			++speedDecrease;
		if (mp->accLinear < -FRICTION_ACC)
			++speedDecrease;

		mp->velLinear -= speedDecrease;
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

	virus.startTimer(TRAP_INTERVAL);
	car.startTimer(CAR_INTERVAL);
}

void Map::removeTimer() {
	SDL_RemoveTimer(moveTimer);
	SDL_RemoveTimer(accelerateTimer);
}

/*
void Map::init() {
}

	if ((mp->camDegree <= mp->roadDegree - MAX_ROTATE_DEGREE || mp->velAngular >= 0) && (mp->camDegree >= mp->roadDegree + MAX_ROTATE_DEGREE || mp->velAngular < 0)
		&& (mp->camDegree >= mp->roadDegree + MAX_ROTATE_DEGREE || mp->camDegree <= mp->roadDegree - MAX_ROTATE_DEGREE))
			mp->camDegree -= mp->velAngular;
	


*/