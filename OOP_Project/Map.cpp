#include "Map.h"
/*
camDepth(DEFAULT_CAMERA_DEPTH), posX(INITIAL_POS* SEGMENT_LENGTH),
velAngular(0), velLinear(0), roadDegree(0), camDegree(0), accLinear(0), camHeight(CAMERA_HEIGHT), velM(1)
*/
// {INITIAL_POS* SEGMENT_LENGTH,0,0,0,0,0,0,DEFAULT_CAMERA_DEPTH,1,CAMERA_HEIGHT}

Map::Map() : lines(NUM_LINE),
number_of_lines(NUM_LINE)
{}

Map::Map(SDL_Renderer* renderer) : car("../images/RacingCar/racingcar", 13, renderer), lines(NUM_LINE), virus("../images/coronavirus/", 15, renderer),
	streetlight("../images/streetlight.png", renderer), moon("../images/moon.png", renderer),
	number_of_lines(NUM_LINE)
{
	double x = 0, dx = 0;

	for (int i = 0; i < NUM_LINE; ++i) {

		//curve, default = 0
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
		
			
		// y, default = 0
		if (i > 300 && i < 1054)		//range of road up and down
			lines[i].sety(sin((i - 300) / 30.0) * CAMERA_HEIGHT);
		else if (i > 1200 && i < 2896)
			lines[i].sety(sin((i - 1200) / 20.0) * (CAMERA_HEIGHT * 0.6));

		// x
		x += dx;
		dx += lines[i].getCurve();
		lines[i].setx(x);

		// z
		lines[i].setz(i * SEGMENT_LENGTH);

		//sprite   
		if ((i & 31) == 0) {	// same as i % 32
			lines[i].setSprite(&streetlight, 2.5);
		}

		//if (i == 300)
		//	virus.setTrap(&lines[i]);
		
	}

	virus.setTrap(&lines[300]);

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

	car.setPosY(lines[INITIAL_POS].getx());

	std::cout << "[Map] Map initialized" << endl;

	car.setPosition(WIDTH / 2 - car.getWidth() / 2, HEIGHT - car.getHeight() + 20);
	car.turn(0);
}

Map::~Map() {

}

void Map::quit() {
	removeTimer();
	car.quit();
	virus.quit();
	streetlight.close();
	std::cout << "[Map] Map closed" << endl;
}

void Map::drawQuad(SDL_Renderer* renderer, Quad q) {
	Sint16 vx[4] = { q.x1 - q.w1,q.x2 - q.w2,q.x2 + q.w2,q.x1 + q.w1 };
	Sint16 vy[4] = { q.y1,q.y2 ,q.y2 ,q.y1 };
	filledPolygonColor(renderer, vx, vy, 4, q.color);
}
/*
void Map::rush(RushType type)
{
	switch (type) 
	{
		case ENERGY:
			if (car.getFullEnergy()) {
				car.setVelLinear(ENERGY_RUSHBEGIN_SPEED);
				car.setCamD(BEGINRUSH_CAMDEPTH);
				car.rush(ENERGY);
				std::cout << "[Map] rush start" << endl;
			}
			else {
				std::cout << "[Map] not enough energy :" << car.getEnergy() << endl;
			}
			break;

		case ACCROAD:
			car.setVelLinear(ACCROAD_RUSHBEGIN_SPEED);
			car.setCamD(BEGINRUSH_CAMDEPTH);
			car.rush(ACCROAD);
			std::cout << "[Map] rush start" << endl;
			break;
		case TOOL:
			break;
		default:
			break;
	}
}*/

void Map::draw(SDL_Renderer* renderer) 
{
	const Motion& m = car.getMotioin();

	int startpos = m.posX / SEGMENT_LENGTH;
	int camH = m.camHeight + lines[startpos].gety();
	int maxy = HEIGHT;

	//road and ground
	Uint32 grass, rumble, road, laneLine;
	laneLine = 0xffffffff;
	static int colorChange = 0;

	boxColor(renderer, 0, HEIGHT / 2, WIDTH, HEIGHT, 0xff10c810);
	double moonW = moon.getWidth();
	SDL_Rect dst = { (1 - sin(m.camDegree) * 1.2) * WIDTH / 2 - moonW / 2,30,moonW,moon.getHeight() };
	if (dst.x > -moonW && dst.x < WIDTH)
		moon.draw(renderer, NULL, &dst);

	for (int i = startpos - 50; i < startpos + 300; ++i) {

		if (i < 1){
			i = 0;
			continue;
		}
		else if (i >= number_of_lines)
			break;

		Line& l = lines[i];
		Line p = lines[i - 1];
		l.project(m.posY, camH, m.posX, m.camDegree, m.camDepth);
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
				Uint32 accRoad = ((i - (colorChange >> 3)) >> 1) & 1 ? 0xff00ffff : 0xff0000ff;
				

				int sign = lines[i].getType() == ACCELERATE_RIGHT ? 1 : -1;

				drawQuad(renderer, { rumble, p.getX(), p.getY(), p.getW() * 1.2, l.getX(), l.getY(), l.getW() * 1.2 });
				drawQuad(renderer, { road, p.getX(), p.getY(), p.getW(), l.getX(), l.getY(), l.getW() });

				if ((i >> 3) & 1) {
					drawQuad(renderer, { laneLine, p.getX(), p.getY(), p.getW() * LANELINE_WIDTH / ROAD_WIDTH, l.getX(), l.getY(),l.getW() * LANELINE_WIDTH / ROAD_WIDTH });
				}

				drawQuad(renderer, { accRoad, p.getX() + sign * p.getW() / 2, p.getY(), p.getW() / 2, l.getX() + sign * l.getW() / 2, l.getY(), l.getW() / 2 });
			}
				break;
			
			/*case TRAPAREA:
			{

			}*/

		}
	}
	colorChange = (colorChange + 2) & 31;

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
		
		//virus.draw(renderer, &lines[i]);
		virus.drawImg(renderer, &lines[i]);		//i changed drawImg in entity to public, so that we can directly use it here
		//lines[i].drawActSprite(renderer, 0);
	}

	//car
	car.draw(renderer);

	/**************************/
	virus.drawStain(renderer);	//only draws stain
	/**************************/


}

Uint32 Map::move(Uint32 interval, void* para) 
{
	Map* map = (Map*)para;
	RacingCar* car = &(map->car);
	const Motion& motion = map->car.getMotioin();

	//velocity modification
	int startpos = motion.posX / SEGMENT_LENGTH;
	car->setRoadDegree(atan((map->lines[startpos + 1].getx() - map->lines[startpos].getx()) / SEGMENT_LENGTH));

	car->setVelM((sin(motion.roadDegree) * (map->lines[startpos + 1].getx() - map->lines[startpos].getx()) + cos(motion.roadDegree) * SEGMENT_LENGTH) / SEGMENT_LENGTH);
	
	//speed punishment
	double punish = 1;
		//cout << map->lines[startpos + 1].getx() - map->lines[startpos].getx() << " " << velM << " ";
	if (motion.posY > map->lines[startpos].getx() + ROAD_WIDTH * motion.velM || motion.posY < map->lines[startpos].getx() - ROAD_WIDTH * motion.velM) {
		punish =   (ROAD_WIDTH * motion.velM)/ (motion.posY - map->lines[startpos].getx());
		if (punish < 0)
			punish = -punish;
	}

	double velX, velY;
	velX = motion.velLinear * cos(motion.camDegree) * motion.velM * punish;
	velY = motion.velLinear * sin(motion.camDegree) * motion.velM * punish;
	
	//move in x-direction
	car->setPosX(motion.posX + velX);
	if (motion.posX < 0 || motion.posX > (map->number_of_lines - 20) * SEGMENT_LENGTH)
		car->setPosX(motion.posX - velX);

	/********* Do not move these codes ********/

	//current index of road line
	startpos = motion.posX / SEGMENT_LENGTH;

	//degree between road vector and normal line (same direction as camera degree)
	car->setRoadDegree(atan((map->lines[startpos + 1].getx() - map->lines[startpos].getx()) / (map->lines[startpos + 1].getz() - map->lines[startpos].getz())));

	 /********* Or there will be bugs ***********/

	//move in y-direction
	car->setPosY(motion.posY + velY);
	
	if ((motion.posY < map->lines[startpos].getx() - ROAD_BORDER * motion.velM) || (motion.posY > map->lines[startpos].getx() + ROAD_BORDER * motion.velM))
	{
		car->setPosY(motion.posY - velY);
		car->setPosX(motion.posX - velX);

		int originpos = motion.posX / SEGMENT_LENGTH;		//startpos after moving, endpos is original
		if (originpos == startpos)
			++originpos;

		double roadD = atan((map->lines[startpos].getx() - map->lines[originpos].getx()) / (map->lines[startpos].getz() - map->lines[originpos].getz()));

		double velProjected = motion.velLinear * cos(roadD - motion.camDegree) * motion.velM * punish;
		car->setPosY(motion.posY + velProjected * sin(roadD));
		car->setPosX(motion.posX + velProjected * cos(roadD));
		//map->posX += velProjected * cos(roadD);
		//map->posY += velProjected * sin(roadD);

		originpos = motion.posX / SEGMENT_LENGTH;
		if (motion.posY < map->lines[originpos].getx() - ROAD_BORDER * motion.velM)
			car->setPosY(map->lines[originpos].getx() - ROAD_BORDER * motion.velM);
			//map->posY = map->lines[originpos].getx() - ROAD_BORDER * motion.velM;
		else if (motion.posY > map->lines[originpos].getx() + ROAD_BORDER * motion.velM)
			car->setPosY(map->lines[originpos].getx() + ROAD_BORDER * motion.velM);
			//map->posY = map->lines[originpos].getx() + ROAD_BORDER * motion.velM;
		
	}
	//rotate camera
	car->setCamDegree(motion.camDegree + motion.velAngular / motion.velM);
	//map->camDegree += motion.velAngular / motion.velM;
	
	if ((motion.camDegree <= motion.roadDegree - MAX_ROTATE_DEGREE / motion.velM && motion.velAngular <= 0) || (motion.camDegree >= motion.roadDegree + MAX_ROTATE_DEGREE / motion.velM && motion.velAngular >= 0)) {
		car->setCamDegree(motion.camDegree - motion.velAngular / motion.velM);
		//map->camDegree -= motion.velAngular / motion.velM;
		if (motion.camDegree <= motion.roadDegree - MAX_ROTATE_DEGREE / motion.velM) {
			car->setCamDegree(motion.roadDegree - MAX_ROTATE_DEGREE / motion.velM);
			//map->camDegree = motion.roadDegree - MAX_ROTATE_DEGREE / motion.velM;
		}
		else if (motion.camDegree >= motion.roadDegree + MAX_ROTATE_DEGREE / motion.velM) {
			car->setCamDegree(motion.roadDegree + MAX_ROTATE_DEGREE / motion.velM);
			//map->camDegree = motion.roadDegree + MAX_ROTATE_DEGREE / motion.velM;
		}
	}

	//special road
	if (car->getRushing() != ACCROAD && (map->lines[startpos].getType() == ACCELERATE_LEFT || map->lines[startpos].getType() == ACCELERATE_RIGHT)) {
		if (map->lines[startpos].getType() == ACCELERATE_LEFT && (motion.posY < map->lines[startpos].getx() && motion.posY > map->lines[startpos].getx() - ROAD_WIDTH * motion.velM)) {
			car->rush(ACCROAD);
		}
		else if (map->lines[startpos].getType() == ACCELERATE_RIGHT && (motion.posY > map->lines[startpos].getx() && motion.posY < map->lines[startpos].getx() + ROAD_WIDTH * motion.velM)) {
			car->rush(ACCROAD);
		}
	}

	//trap
	if (map->lines[startpos].getType() == TRAPAREA && motion.posY < map->lines[startpos].getx() + TRAP_WIDTH * motion.velM && motion.posY > map->lines[startpos].getx() - TRAP_WIDTH * motion.velM)
		map->virus.gettrap(STAIN);

	
	
	//cout << map->camDegree - map->roadDegree << endl;
	return interval;
}

Uint32 Map::accelerate(Uint32 interval, void* para) 
{
	Map* map = (Map*)para;
	RacingCar* car = &(map->car);
	const Motion& motion = map->car.getMotioin();

	if (car->getRushing()) //excpet RushType == NONE(0), other types will go here
	{
		double speedDecrease = AFTERRUSH_SPEED_DECREASE;
		if (motion.accLinear < 0)
			++speedDecrease;
		if (motion.accLinear < -FRICTION_ACC)
			++speedDecrease;

		car->setVelLinear(motion.velLinear - speedDecrease);
		//map->velLinear -= speedDecrease;
		if (motion.velLinear < MAX_FORWARD_SPEED) {
			car->setVelLinear(MAX_FORWARD_SPEED);
			//map->velLinear = MAX_FORWARD_SPEED;
			if (motion.accLinear == 0)
				car->setAccLinear(-FRICTION_ACC);
				//map->accLinear = -FRICTION_ACC;
			map->car.rush(NONE);
		}
		car->setCamDepth(motion.camDepth + AFTERRUSH_CAMDEPTH_RECOVER);
		//map->camDepth += AFTERRUSH_CAMDEPTH_RECOVER;
		if (motion.camDepth > DEFAULT_CAMERA_DEPTH) {
			car->setCamDepth(DEFAULT_CAMERA_DEPTH);
			//map->camDepth = DEFAULT_CAMERA_DEPTH;
		}
	}
	else 
	{
		car->setVelLinear(motion.velLinear + motion.accLinear);
		//map->velLinear += motion.accLinear;
		if (motion.velLinear > MAX_FORWARD_SPEED) {
			car->setVelLinear(MAX_FORWARD_SPEED);
			//map->velLinear = MAX_FORWARD_SPEED;
		}
		else if (motion.velLinear < -MAX_BACKWARD_SPEED) {
			car->setVelLinear(-MAX_BACKWARD_SPEED);
			//map->velLinear = -MAX_BACKWARD_SPEED;
		}

		if ((motion.accLinear == -FRICTION_ACC && motion.velLinear < 0) || (motion.accLinear == FRICTION_ACC && motion.velLinear > 0)) {
			car->setVelLinear(0);
			car->setAccLinear(0);
			//map->velLinear = 0;
			//map->accLinear = 0;
		}

	}
	//cout << map->accLinear << " " << map->velLinear << endl;
	return interval;
}


/*
void Map::turn(int d)
{
	car.turn(d);
}*/

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