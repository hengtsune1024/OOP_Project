#include "Map.h"

Uint32 Map::grass;
Uint32 Map::rumble;
Uint32 Map::road;
Uint32 Map::laneLine;
SDL_Rect Map::viewPort0 = { WIDTH / 2,0,WIDTH,HEIGHT };
SDL_Rect Map::viewPort1 = { 0,0,WIDTH,HEIGHT };
SDL_Rect Map::viewPort2 = { WIDTH,0,WIDTH,HEIGHT };
unsigned long long Map::type = 0;

Map::Map() {}

Map::Map(SDL_Renderer* renderer, bool dual) : lines(NUM_LINE), number_of_lines(NUM_LINE), dualMode(dual), endtype(PLAYING), record(0),
	car1(new RacingCar("../images/car/car.txt", "../images/car/car.bmp", renderer, &lines[INITIAL_POS])),
	car2(dual ? new RacingCar("../images/car/car.txt", "../images/car/car.bmp", renderer, &lines[INITIAL_POS]) : NULL),
	streetlight("../images/streetlight.png", renderer), moon("../images/moon.png", renderer),
	//cube("../images/cube/cube.txt", "../images/cube/cube.bmp", &lines, CUBE_SIZE / 2.457335),
	virus(renderer,true), tools(renderer), rock("../images/rock/rock.txt", "../images/rock/rock.bmp")
{
	FILE* f = fopen("../bin/map.dat", "rb");
	fseek(f, 0, SEEK_SET);
	struct {
		double x, y, z;
		double slope;
		bool sprite;
		unsigned long long type = 0;
		double maxSpeed;
		double roadDeg;
		double roadVelM;
	} road;

	for (int i = 0; i < NUM_LINE; ++i) {
		fread(&road, sizeof(road), 1, f);
		lines[i].setAll(road.x, road.y, road.z, road.slope, road.type, road.roadDeg, road.roadVelM, road.maxSpeed);
		if (road.sprite) {
			lines[i].setSprite(&streetlight, 2.5);
		}

	}

	//type
	virus.setTrap(&lines[300], 300, 0);
	tools.setTool(&lines[200], 200, 0);
	rock.setObstacle(&lines[250], 250, 0);

	virus.setTrap(&lines[500], 500, 1);
	tools.setTool(&lines[600], 600, 1);
	rock.setObstacle(&lines[700], 700, 1);
	
	//3d object set position
	//cube.setPos({ lines[POS].getx(),lines[POS].gety()+CUBE_SIZE,lines[POS].getz(),0,0,0 });

	if (dualMode) {
		car1->setPosY(lines[INITIAL_POS].getx() - ROAD_WIDTH / 2);
		car2->setPosY(lines[INITIAL_POS].getx() + ROAD_WIDTH / 2);

		car1->setOtherCar(car2);
		car2->setOtherCar(car1);
	}
	else {
		car1->setPosY(lines[INITIAL_POS].getx());
	}

	std::cout << "[Map] Map initialized" << endl;
}

Map::~Map() {
	delete car1;
	std::cout << "[Map] delete car1" << endl;
	car1 = NULL;
	if (car2 != NULL) {
		delete car2;
		std::cout << "[Map] delete car2" << endl;
		car2 = NULL;
	}
}
/*
void Map::generateMap() 
{
	srand(std::time(NULL));
	// index ranges from 0 to 10000, car starts at 30 and ends at 9500.
	// map object and road design take place from 100 to 9100.

	int upper, lower, minDist, range;
	range = 9000 / NUM_TRAP;
	minDist = range / 2;
	for (int i = 0; i < NUM_TRAP; ++i) {
		lower = 100 + range * i + 1;
		if (i > 0 && lower < virus[i - 1].getIndex() + minDist + 1)
			lower = virus[i - 1].getIndex() + minDist + 1;
		upper = 100 + range * (i + 1);
		virus[i].setIndex((upper - lower) * (rand() / (RAND_MAX + 1.0)) + lower);
	}
	range = 9000 / NUM_TOOL;
	minDist = range / 2;
	for (int i = 0; i < NUM_TOOL; ++i) {
		lower = 100 + range * i + 1;
		if (i > 0 && lower < tools[i - 1].getIndex() + minDist + 1)
			lower = tools[i - 1].getIndex() + minDist + 1;
		upper = 100 + range * (i + 1);
		tools[i].setIndex((upper - lower) * (rand() / (RAND_MAX + 1.0)) + lower);
	}
	range = 9000 / NUM_OBSTACLE;
	minDist = range / 2;
	for (int i = 0; i < NUM_OBSTACLE; ++i) {
		lower = 100 + range * i + 1;
		if (i > 0 && lower < rock[i - 1].getIndex() + minDist + 1)
			lower = rock[i - 1].getIndex() + minDist + 1;
		upper = 100 + range * (i + 1);
		rock[i].setIndex((upper - lower) * (rand() / (RAND_MAX + 1.0)) + lower);
	}

	double x = 0, dx = 0, curve = 0;
	for (int i = 0; i < NUM_LINE; ++i) 
	{
		//road design
		//z
		lines[i].setz(i * SEGMENT_LENGTH);
		
		//x

		//y

		//streetlight
		if ((i & 31) == 0)
			lines[i].setSprite(&streetlight, 2.5);

		//road Degree
		if (i > 0)
			lines[i].setRoadDegree(atan((lines[i].getx() - lines[i - 1].getx()) / SEGMENT_LENGTH));

		//road modify
		if (i > 0)
			lines[i].setRoadVelM((sin(lines[i].getRoadDegree()) * (lines[i].getx() - lines[i - 1].getx()) + cos(lines[i].getRoadDegree()) * SEGMENT_LENGTH) / SEGMENT_LENGTH);

		// type
		if (i >= INITIAL_POS - 10 && i < INITIAL_POS + 22)
			lines[i].addType(STARTPOINT);
		else if (i >= FINAL_POS - 12 && i < FINAL_POS + 20)
			lines[i].addType(ENDPOINT);
	}

	for (int i = 0; i < NUM_TRAP; ++i) {
		virus[i].setTrap(&lines[virus[i].getIndex()]);
	}
	for (int i = 0; i < NUM_TOOL; ++i) {
		tools[i].setTool(&lines[tools[i].getIndex()]);
	}
	for (int i = 0; i < NUM_OBSTACLE; ++i) {
		rock[i].setObstacle(&lines[rock[i].getIndex()]);
	}
}*/

void Map::quit() {
	removeTimer();
	car1->quit();
	if (dualMode)
		car2->quit();
	streetlight.close();
	moon.close();
	//cube.close();
	virus.close();
	tools.close();
	rock.close();
	std::cout << "[Map] Map closed" << endl;
}

void Map::drawQuad(SDL_Renderer* renderer, Quad q) {
	Sint16 vx[4] = { q.x1 - q.w1,q.x2 - q.w2,q.x2 + q.w2,q.x1 + q.w1 };
	Sint16 vy[4] = { q.y1,q.y2 ,q.y2 ,q.y1 };
	filledPolygonColor(renderer, vx, vy, 4, q.color);
}

Uint32 Map::Objectlogic(Uint32 interval, void* para)
{
	Map* map = (Map*)para;
	//tools
	//traps
	map->tools.logic();
	map->virus.logic();

	//physical object
	//map->cube.logic();

	return interval;
}

void Map::draw(SDL_Renderer* renderer) 
{
	if (dualMode)
		SDL_RenderSetViewport(renderer, &viewPort1);
	else
		SDL_RenderSetViewport(renderer, &viewPort0);

	RacingCar* car = car1;
	RacingCar* otherCar = car2;
	int times = dualMode ? 2 : 1;
	int startpos, camH,maxy, moonW, critz(0);

	do {

		const Motion& m = car->getMotion();
		startpos = m.posX / SEGMENT_LENGTH;
		if (car->isInAir())
			camH = m.camHeight + m.baseHeight;
		else
			camH = m.camHeight + lines[startpos].gety();

		maxy = HEIGHT;

		//road and ground
		laneLine = 0xffffffff;
		static int colorChange1 = 0;

		moonW = moon.getWidth();

		SDL_Rect dst = { (1 - sin(m.camDegree) * 1.2) * WIDTH / 2 - moonW / 2,30,moonW,moon.getHeight() };
		if (dst.x > -moonW && dst.x < WIDTH)
			moon.draw(renderer, NULL, &dst);

		for (int i = startpos - 50; i < startpos + 300; ++i) {

			if (i < 1) {
				i = 0;
				continue;
			}
			else if (i >= number_of_lines)
				break;

			Line& l = lines[i];
			const Line& p = lines[i - 1];
			l.project(m.posY, camH, m.posX, m.camDegree, m.camDepth);
			//l.project(lines[startpos+5].getx(), camH, lines[startpos+5].getz(), camDegree, camDepth, roadDegree);
			if (l.getW() < 1e-6 && l.getW() > -1e-6)
				continue;

			l.setClip(maxy);
			if (l.getY() >= maxy)
				continue;

			maxy = l.getY();
			critz = l.getz();

			//grass
			grass = (i >> 2) & 1 ? 0xff10c810 : 0xff009A00;
			drawQuad(renderer, { grass,  WIDTH / 2, p.getY(), WIDTH / 2, WIDTH / 2, l.getY(), WIDTH / 2 });

			//road type
			type = lines[i].getType();
			if ((type & NORMAL) || (type & TRAPAREA) || (type & TOOLAREA) || (type & OBSTACLEAREA) || (type & CLIFF)) {
				rumble = (i >> 2) & 1 ? 0xffffffff : 0xff000000;
				road = (i >> 2) & 1 ? 0xff6b6b6b : 0xff696969;
				drawQuad(renderer, { rumble, p.getX(), p.getY(), p.getW() * 1.2, l.getX(), l.getY(), l.getW() * 1.2 });
				drawQuad(renderer, { road, p.getX(), p.getY(), p.getW(), l.getX(), l.getY(), l.getW() });

				if ((i >> 3) & 1) {
					drawQuad(renderer, { laneLine, p.getX(), p.getY(), p.getW() * LANELINE_WIDTH / ROAD_WIDTH, l.getX(), l.getY(),l.getW() * LANELINE_WIDTH / ROAD_WIDTH });
				}
			}
			else if ((type & LOW_FRICTION)) {
				rumble = (i >> 2) & 1 ? 0xffffffff : 0xff000000;
				road = (i >> 2) & 1 ? 0xffffff80 : 0xffffffff;
				drawQuad(renderer, { rumble, p.getX(), p.getY(), p.getW() * 1.2, l.getX(), l.getY(), l.getW() * 1.2 });
				drawQuad(renderer, { road, p.getX(), p.getY(), p.getW(), l.getX(), l.getY(), l.getW() });
			}
			else if ((type & HIGH_FRICTION)) {
				rumble = (i >> 2) & 1 ? 0xffffffff : 0xff000000;
				road = (i >> 2) & 1 ? 0xff00499A : 0xff00346E;
				drawQuad(renderer, { rumble, p.getX(), p.getY(), p.getW() * 1.2, l.getX(), l.getY(), l.getW() * 1.2 });
				drawQuad(renderer, { road, p.getX(), p.getY(), p.getW(), l.getX(), l.getY(), l.getW() });
			}
			else if ((type & ENDPOINT) || (type & STARTPOINT)) {
				double width_scale = 1.2;
				for (int j = 0; j <= 7; ++j) {

					width_scale = 1.2 * (15 - (j << 1)) / 15;
					rumble = ((i >> 2) + j) & 1 ? 0xffffffff : 0xff000000;

					drawQuad(renderer, { rumble,p.getX(), p.getY(), p.getW() * width_scale, l.getX(), l.getY(), l.getW() * width_scale });
				}
			}
			else if ((type & ACCELERATE_RIGHT) || (type & ACCELERATE_LEFT)) {
				rumble = (i >> 2) & 1 ? 0xffffffff : 0xff000000;
				road = (i >> 2) & 1 ? 0xff6b6b6b : 0xff696969;
				Uint32 accRoad = ((i - (colorChange1 >> 3)) >> 1) & 1 ? 0xff00ffff : 0xff0000ff;


				int sign = (type & ACCELERATE_RIGHT) ? 1 : -1;

				drawQuad(renderer, { rumble, p.getX(), p.getY(), p.getW() * 1.2, l.getX(), l.getY(), l.getW() * 1.2 });
				drawQuad(renderer, { road, p.getX(), p.getY(), p.getW(), l.getX(), l.getY(), l.getW() });

				if ((i >> 3) & 1) {
					drawQuad(renderer, { laneLine, p.getX(), p.getY(), p.getW() * LANELINE_WIDTH / ROAD_WIDTH, l.getX(), l.getY(),l.getW() * LANELINE_WIDTH / ROAD_WIDTH });
				}

				drawQuad(renderer, { accRoad, p.getX() + sign * p.getW() / 2, p.getY(), p.getW() / 2, l.getX() + sign * l.getW() / 2, l.getY(), l.getW() / 2 });
			}

			
		}

		colorChange1 = (colorChange1 + 2) & 31;

		//sprite lamps
		for (int i = startpos + 300; i > startpos; --i) {

			if (i >= number_of_lines) {
				i = number_of_lines;
				continue;
			}
			else if (i < 1)
				break;

			lines[i].drawSprite(renderer);
			//filledCircleColor(renderer, lines[i].getX(), lines[i].getY(), 2, 0xffffffff);

		}
		Point3D pos = { m.posY,1.0 * camH,m.posX };

		bool clean = true;

		for (int i = 0; i < NUM_TRAP; ++i)
			if (startpos >= virus.getIndex(i) - 300 && startpos <= virus.getIndex(i))
				virus.draw3D(pos, m.camDegree, m.camDepth, &engine, clean, i, HEIGHT);

		
		//if (startpos + 300 > POS && cube.getZ() - CUBE_SIZE > m.posX) {
		//	cube.draw3D(pos, m.camDegree, m.camDepth, &engine, clean, HEIGHT);
		//}

		for (int i = 0; i < NUM_OBSTACLE; ++i)
			if (startpos >= rock.getIndex(i) - 300 && startpos <= rock.getIndex(i)) {
				rock.draw3D(pos, m.camDegree, m.camDepth, &engine, clean, i);
			}

		for (int i = 0; i < NUM_TOOL; ++i)
			if (startpos >= tools.getIndex(i) - 300 && startpos <= tools.getIndex(i)) {
				tools.draw3D(pos, m.camDegree, m.camDepth, &engine, clean, i, HEIGHT);
			}

		if (otherCar != NULL && otherCar->getPosX() > m.posX - 50 * SEGMENT_LENGTH && otherCar->getPosX() - m.posX < 300 * SEGMENT_LENGTH) {
			if (otherCar->getPosX() > critz) {
				car->drawOtherCar(renderer, &engine, clean, maxy, camH);
			}
			else {
				car->drawOtherCar(renderer, &engine, clean, HEIGHT, camH);
			}
			clean = false;
		}

		

		//car
		car->draw(renderer, &engine, clean);

		engine.drawAll(renderer);

		/**************************/
		virus.drawStain(renderer, (dualMode ? times - 1 : true));	//only draws stain
		/**************************/

		tools.drawmytool(renderer, (dualMode ? times - 1 : true));


		if (dualMode) {
			car = car2;
			otherCar = car1;
			SDL_RenderSetViewport(renderer, &viewPort2);
		}

	} while (--times);

	if(dualMode){
		SDL_RenderSetViewport(renderer, NULL);
		thickLineColor(renderer, WIDTH, 0, WIDTH, HEIGHT, 5, 0xff0000ff);
	}
}

Uint32 Map::move(Uint32 interval, void* para) 
{
	Map* map = (Map*)para;
	RacingCar* car = map->car1;
	int times = map->dualMode ? 2 : 1;
	int startpos;
	double punish;
	
	do {
		const Motion& motion = car->getMotion();
		startpos = motion.posX / SEGMENT_LENGTH;
		car->setCurrentPos(&(map->lines[startpos]));
		type = map->lines[startpos].getType();

		//perpendicular (z-direction)
		car->setCamHeight(motion.camHeight + motion.velPerpen);
		if (car->isInAir() && ((motion.camHeight + motion.baseHeight) - (CAMERA_HEIGHT + map->lines[startpos].gety()) < -1e-6 && !(type & CLIFF))) {
			car->setCamHeight(CAMERA_HEIGHT);
			car->setInAir(false);
			if (car->getCurrentPos()->getSlope()) {
				car->setVelLinear(motion.velLinear + motion.velPerpen * car->getCurrentPos()->getSlope() / SEGMENT_LENGTH);
			}
			car->setVelPerpen(0);
		}
		else if (car->isInAir()) {
			//free fall
			car->setVelPerpen(motion.velPerpen - GRAVITY);
		}
		else {
			if (motion.camHeight - CAMERA_HEIGHT > 1e-6) {
				car->setInAir(true, map->lines[startpos].gety());
			}
			else {
				car->setCamHeight(CAMERA_HEIGHT);
			}
		}

		double dist = CAR_HALF_LENGTH * cos(motion.axleDegree);
		int front = (motion.posX + CAMERA_CARMIDPOINT_DIST + dist) / SEGMENT_LENGTH;
		int back = (motion.posX + CAMERA_CARMIDPOINT_DIST - dist) / SEGMENT_LENGTH;

		

		car->setRoadDegree(car->getCurrentPos()->getRoadDegree());

		//speed punishment
		punish = 1.0;
		if (motion.posY > map->lines[startpos].getx() + ROAD_WIDTH * motion.velM || motion.posY < map->lines[startpos].getx() - ROAD_WIDTH* motion.velM) {
			punish = (ROAD_WIDTH * motion.velM) / (motion.posY - map->lines[startpos].getx());
			if (punish < 0)
				punish = -punish;
			car->setOutofRoad(true);
		}
		else {
			car->setOutofRoad(false);
		}

		
		//friction
		type = map->lines[startpos].getType();

		if ((type & HIGH_FRICTION)) {
			if (!car->isOutofRoad() && (motion.roadMod - 0.6 > 1e-6 || motion.roadMod - 0.6 < -1e-6)) {
				if (motion.roadMod < 0.6)
					car->setRoadMod(motion.roadMod + 0.1);
				else
					car->setRoadMod(motion.roadMod - 0.1);
			}
		}
		else if ((type & LOW_FRICTION)) {
			if (!car->isOutofRoad() && (motion.roadMod - 1.3 > 1e-6 || motion.roadMod - 1.3 < -1e-6)) {
				if (motion.roadMod < 1.3)
					car->setRoadMod(motion.roadMod + 0.1);
				else
					car->setRoadMod(motion.roadMod - 0.1);
			}
		}
		else {
			if (motion.roadMod - 1 > 1e-6 || motion.roadMod - 1 < -1e-6) {
				if (motion.roadMod < 1)
					car->setRoadMod(motion.roadMod + 0.1);
				else
					car->setRoadMod(motion.roadMod - 0.1);
			}
		}

		car->setVelM(car->getRoadMod() * car->getCurrentPos()->getRoadVelM());


		//set car road type
		car->setFrictionType(type);

		double velX, velY;
		velX = motion.velLinear * cos(motion.axleDegree) * punish * motion.velM;
		velY = motion.velLinear * sin(motion.axleDegree) * punish * motion.velM;

		//move in x-direction
		car->setPosX(motion.posX + velX);
		if (motion.posX < 0 || motion.posX >(map->number_of_lines - 20) * SEGMENT_LENGTH || (velX < 0 && (map->lines[(int)(motion.posX / SEGMENT_LENGTH)].getType() & CLIFF)))
			car->setPosX(motion.posX - velX);

		/********* Do not move these codes ********/

		//current index of road line
		startpos = motion.posX / SEGMENT_LENGTH;
		car->setCurrentPos(&(map->lines[startpos]));

		//degree between road vector and normal line (same direction as camera degree)
		car->setRoadDegree(map->lines[startpos].getRoadDegree());

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

			double velProjected = motion.velLinear * cos(roadD - motion.axleDegree) * punish * motion.velM;
			car->setPosY(motion.posY + velProjected * sin(roadD));
			car->setPosX(motion.posX + velProjected * cos(roadD));
			//map->posX += velProjected * cos(roadD);
			//map->posY += velProjected * sin(roadD);

			originpos = motion.posX / SEGMENT_LENGTH;
			if (motion.posY < map->lines[originpos].getx() - ROAD_BORDER * motion.velM)
				car->setPosY(map->lines[originpos].getx() - ROAD_BORDER * motion.velM);
			//map->posY = map->lines[originpos].getx() - ROAD_BORDER * ;
			else if (motion.posY > map->lines[originpos].getx() + ROAD_BORDER * motion.velM)
				car->setPosY(map->lines[originpos].getx() + ROAD_BORDER * motion.velM);
			//map->posY = map->lines[originpos].getx() + ROAD_BORDER * ;

		}
		
		//collision with the other car
		if (map->dualMode)
		{
			if (car->collided()) 
			{
				car->setPosY(motion.posY - velY);
				car->setPosX(motion.posX - velX);

				double dz = map->car1->getPosX() - map->car2->getPosX();
				double e = 0.6;

				double cos_ = cos(map->car1->getAxleDegree() - map->car2->getAxleDegree());
				double v1 = map->car1->getVelLinear(), v2 = map->car2->getVelLinear() * cos_;
				double v = ((1 - e) * v1 + (1 + e) * v2) / 2.0;
				map->car1->setVelLinear(v);

				v1 = map->car1->getVelLinear() * cos_, v2 = map->car2->getVelLinear();
				v = ((1 - e) * v2 + (1 + e) * v1) / 2.0;
				map->car2->setVelLinear(v);

				if (dz < 0 && map->car1->getRushing()) {
					map->car1->rush(NONE);
				}
				else if (dz > 0 && map->car2->getRushing()) {
					map->car2->rush(NONE);
				}
			}
		}


		//rotate car
		car->setAxleDegree(motion.axleDegree + motion.velAngular);

		if ((motion.axleDegree <= motion.roadDegree - MAX_ROTATE_DEGREE / motion.velM && motion.velAngular <= 0) || (motion.axleDegree >= motion.roadDegree + MAX_ROTATE_DEGREE / motion.velM && motion.velAngular >= 0)) {
			car->setAxleDegree(motion.axleDegree - motion.velAngular);
			if (motion.axleDegree <= motion.roadDegree - MAX_ROTATE_DEGREE / motion.velM) {
				car->setAxleDegree(motion.roadDegree - MAX_ROTATE_DEGREE / motion.velM);
			}
			else if (motion.axleDegree >= motion.roadDegree + MAX_ROTATE_DEGREE / motion.velM) {
				car->setAxleDegree(motion.roadDegree + MAX_ROTATE_DEGREE / motion.velM);
			}
		}
		if (map->dualMode && car->collided()) {
			car->setAxleDegree(motion.axleDegree - motion.velAngular);
		}

		//rotate camera
		if (motion.camDegree > motion.axleDegree) {
			double rot = 0.06 / 0.78539 * (motion.camDegree - motion.axleDegree) + 0.01;
			car->setCamDegree(motion.camDegree - rot / motion.velM);
			if (motion.camDegree < motion.axleDegree)
				car->setCamDegree(motion.axleDegree);
		}
		else if (motion.camDegree < motion.axleDegree) {
			double rot = 0.06 / 0.78539 * (  motion.axleDegree- motion.camDegree) + 0.01;
			car->setCamDegree(motion.camDegree + rot / motion.velM);
			if (motion.camDegree > motion.axleDegree)
				car->setCamDegree(motion.axleDegree);
		}

		//update startpos and type
		startpos = motion.posX / SEGMENT_LENGTH;
		car->setCurrentPos(&(map->lines[startpos]));
		type = map->lines[startpos].getType();
		car->setFrictionType(type);

		// Xdegree
		if (!car->isInAir()) {
			if(!(map->lines[front].getType() & CLIFF))
				car->setXangle(atan((map->lines[front].gety() - map->lines[back].gety()) / (2 * dist)));
		}
		else if (!(type & INCLINE_PLANE)) {
			if (velX > 0) {
				car->setXangle(motion.Xangle - 0.02);
			}
			else {
				car->setXangle(motion.Xangle + 0.02);
			}
		}


		//special road
		if (!car->isInAir()) 
		{
			//fly
			//critVel=GRAVITY*|(1+y'^2)/y''|
			if (((type & INCLINE_BACKWARD) && motion.velLinear > 1e-6) || ((type & INCLINE_FORWARD) && motion.velLinear < -1e-6)) {
				double critVel = car->getCurrentPos()->getCritVel();
				if ((critVel > -1e-6 && velX * velX > critVel) || (map->lines[startpos].getType() & INCLINE_PLANE)) {
					car->setVelPerpen(velX * (map->lines[startpos].getSlope() / sqrt(SEGMENT_LENGTH * SEGMENT_LENGTH + map->lines[startpos].getSlope() * map->lines[startpos].getSlope())));
					cout << startpos << endl;
					if (motion.velPerpen < GRAVITY * 5 && !(type & CLIFF)) {
						car->setVelPerpen(0);
					}
					else {
						car->setInAir(true, map->lines[startpos].gety());
					}
				}
			}

			startpos = (motion.posX + CAMERA_CARMIDPOINT_DIST) / SEGMENT_LENGTH;
			double midY = motion.posY + CAMERA_CARMIDPOINT_DIST * sin(motion.axleDegree);
			type = map->lines[startpos].getType();

			//rush
			if (car->getRushing() != ACCROAD && ((type & ACCELERATE_LEFT) || (type & ACCELERATE_RIGHT))) {
				if ((type & ACCELERATE_LEFT) && (midY < map->lines[startpos].getx() && midY > map->lines[startpos].getx() - ROAD_WIDTH * motion.velM)) {
					car->rush(ACCROAD);
				}
				else if ((type & ACCELERATE_RIGHT) && (midY > map->lines[startpos].getx() && midY < map->lines[startpos].getx() + ROAD_WIDTH * motion.velM)) {
					car->rush(ACCROAD);
				}
			}
			//trap
			if (type & TRAPAREA)
			{
				if ((map->virus.getSide() && midY < map->lines[startpos].getx() + (ROAD_WIDTH / 2.0 + TRAP_WIDTH) * motion.velM && midY > map->lines[startpos].getx() + (ROAD_WIDTH / 2.0 - TRAP_WIDTH) * motion.velM) ||
					(!map->virus.getSide() && midY < map->lines[startpos].getx() + (-ROAD_WIDTH / 2.0 + TRAP_WIDTH) * motion.velM && midY > map->lines[startpos].getx() + (-ROAD_WIDTH / 2.0 - TRAP_WIDTH) * motion.velM))
					map->virus.gettrap(STAIN, (map->dualMode ? times - 1 : true), startpos);
			}
			
			//tool
			if ((type & TOOLAREA) && midY < map->lines[startpos].getx() + TOOL_WIDTH * motion.velM && midY > map->lines[startpos].getx() - TOOL_WIDTH * motion.velM)
			{
				map->tools.getTools((map->dualMode ? times - 1 : true), startpos);
			}

			//obstacle
			if ((type & OBSTACLEAREA) && midY < map->lines[startpos].getx() + OBSTACLE_WIDTH * motion.velM && midY > map->lines[startpos].getx() - OBSTACLE_WIDTH * motion.velM)
			{
				car->touchobstacle(map->rock);
				
				if (car->getHP() <= 0)
				{
					if (map->dualMode)
						map->endtype = (times == 2 ? PLAYER2 : PLAYER1);
					else
						map->endtype = FAILED;
					map->endtime = SDL_GetTicks64() + 3000;

				}
			}

			//arrive
			if ((type & ENDPOINT))
			{
				car->isarrive();
				if (map->dualMode)
				{
					if (!map->endtype)
					{
						map->endtype = (times == 1 ? PLAYER2 : PLAYER1);
						map->record = car->gettotaltime();
					}
				}
				else
				{
					map->endtype = VICTORY;
					map->record = car->gettotaltime();
				}
				map->endtime = SDL_GetTicks64() + 3000;
			}

			
			//map->cube.collide(car);
		}

		if (map->dualMode) 
		{
			car = map->car2;
		}

	} while (--times);
	
	return interval;
}

Uint32 Map::accelerate(Uint32 interval, void* para)
{
	Map* map = (Map*)para;
	RacingCar* car = map->car1;

	int times = map->dualMode ? 2 : 1;

	do {
		const Motion& motion = car->getMotion();
		car->brake();
		
		if (car->getRushing()) //excpet RushType == NONE(0), other types will go here
		{
			double speedDecrease = AFTERRUSH_SPEED_DECREASE;
			if (motion.accLinear < 0)
				++speedDecrease;
			if (motion.accLinear < -FRICTION_ACC)
				++speedDecrease;

			car->setVelLinear(motion.velLinear - speedDecrease);
			if (motion.velLinear < MAX_FORWARD_SPEED) {
				car->setVelLinear(MAX_FORWARD_SPEED);
				if (motion.accLinear == 0)
					car->brake(0);
				car->rush(NONE);
			}
			car->setCamDepth(motion.camDepth + AFTERRUSH_CAMDEPTH_RECOVER);
			if (motion.camDepth > DEFAULT_CAMERA_DEPTH) {
				car->setCamDepth(DEFAULT_CAMERA_DEPTH);
			}
		}
		else
		{
			car->setVelLinear(motion.velLinear + motion.accLinear);
			if (motion.velLinear > MAX_FORWARD_SPEED) {
				car->setVelLinear(MAX_FORWARD_SPEED);
			}
			else if (motion.velLinear < -MAX_BACKWARD_SPEED) {
				car->setVelLinear(-MAX_BACKWARD_SPEED);
			}
			bool incline = (car->getCurrentPos()->getType() & INCLINE_BACKWARD) || (car->getCurrentPos()->getType() & INCLINE_FORWARD);
			if (!incline && (((motion.accLinear < 0 && motion.accLinear >= -HIGH_FRICTION_ACC - 50) && motion.velLinear < 0) || ((motion.accLinear > 0 && motion.accLinear <= HIGH_FRICTION_ACC + 50) && motion.velLinear > 0))) {
				car->setVelLinear(0);
				car->brake(0);
			}
		}
		if (map->dualMode) {
			car = map->car2;
		}

	} while (--times);

	return interval;
}

void Map::startTimer() {

	moveTimer = SDL_AddTimer(MOVE_INTERVAL, move, this);
	accelerateTimer = SDL_AddTimer(ACCELERATE_INTERVAL, accelerate, this);
	mapObjectTimer = SDL_AddTimer(50, Objectlogic, this);

	car1->startTimer(CAR_INTERVAL);
	if (dualMode)
		car2->startTimer(CAR_INTERVAL);

}

void Map::removeTimer() {
	SDL_RemoveTimer(moveTimer);
	SDL_RemoveTimer(accelerateTimer);
	SDL_RemoveTimer(mapObjectTimer);
}

/*
	if (dualMode) {

		SDL_RenderSetViewport(renderer, &viewPort2);
		const Motion& m = car2->getMotioin();

		int startpos = m.posX / SEGMENT_LENGTH;
		int camH = m.camHeight + lines[startpos].gety();
		if (car2->isInAir())
			camH = m.camHeight + car2->baseHeight;
		int maxy = HEIGHT;

		//road and ground
		laneLine = 0xffffffff;
		static int colorChange1 = 0;

		//boxColor(renderer, 0, HEIGHT / 2, WIDTH, HEIGHT, 0xff10c810);
		double moonW = moon.getWidth();

		SDL_Rect dst = { (1 - sin(m.camDegree) * 1.2) * WIDTH / 2 - moonW / 2,30,moonW,moon.getHeight() };
		if (dst.x > -moonW && dst.x < WIDTH)
			moon.draw(renderer, NULL, &dst);

		for (int i = startpos - 50; i < startpos + 300; ++i) {

			if (i < 1) {
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
			type = lines[i].getType();
			if ((type & NORMAL) || (type & TRAPAREA) || (type & TOOLAREA) || (type & OBSTACLEAREA)) {
				rumble = (i >> 2) & 1 ? 0xffffffff : 0xff000000;
				road = (i >> 2) & 1 ? 0xff6b6b6b : 0xff696969;
				drawQuad(renderer, { rumble, p.getX(), p.getY(), p.getW() * 1.2, l.getX(), l.getY(), l.getW() * 1.2 });
				drawQuad(renderer, { road, p.getX(), p.getY(), p.getW(), l.getX(), l.getY(), l.getW() });

				if ((i >> 3) & 1) {
					drawQuad(renderer, { laneLine, p.getX(), p.getY(), p.getW() * LANELINE_WIDTH / ROAD_WIDTH, l.getX(), l.getY(),l.getW() * LANELINE_WIDTH / ROAD_WIDTH });
				}
			}
			else if ((type & LOW_FRICTION)) {
				rumble = (i >> 2) & 1 ? 0xffffffff : 0xff000000;
				road = (i >> 2) & 1 ? 0xffffff80 : 0xffffffff;
				drawQuad(renderer, { rumble, p.getX(), p.getY(), p.getW() * 1.2, l.getX(), l.getY(), l.getW() * 1.2 });
				drawQuad(renderer, { road, p.getX(), p.getY(), p.getW(), l.getX(), l.getY(), l.getW() });
			}
			else if ((type & HIGH_FRICTION)) {
				rumble = (i >> 2) & 1 ? 0xffffffff : 0xff000000;
				road = (i >> 2) & 1 ? 0xff00499A : 0xff00346E;
				drawQuad(renderer, { rumble, p.getX(), p.getY(), p.getW() * 1.2, l.getX(), l.getY(), l.getW() * 1.2 });
				drawQuad(renderer, { road, p.getX(), p.getY(), p.getW(), l.getX(), l.getY(), l.getW() });
			}
			else if ((type & ENDPOINT)) {
				double width_scale = 1.2;
				for (int j = 0; j <= 7; ++j) {

					width_scale = 1.2 * (15 - (j << 1)) / 15;
					rumble = ((i >> 2) + j) & 1 ? 0xffffffff : 0xff000000;

					drawQuad(renderer, { rumble,p.getX(), p.getY(), p.getW() * width_scale, l.getX(), l.getY(), l.getW() * width_scale });
				}
			}
			else if ((type & ACCELERATE_RIGHT) || (type & ACCELERATE_LEFT)) {
				rumble = (i >> 2) & 1 ? 0xffffffff : 0xff000000;
				road = (i >> 2) & 1 ? 0xff6b6b6b : 0xff696969;
				Uint32 accRoad = ((i - (colorChange1 >> 3)) >> 1) & 1 ? 0xff00ffff : 0xff0000ff;


				int sign = (type & ACCELERATE_RIGHT) ? 1 : -1;

				drawQuad(renderer, { rumble, p.getX(), p.getY(), p.getW() * 1.2, l.getX(), l.getY(), l.getW() * 1.2 });
				drawQuad(renderer, { road, p.getX(), p.getY(), p.getW(), l.getX(), l.getY(), l.getW() });

				if ((i >> 3) & 1) {
					drawQuad(renderer, { laneLine, p.getX(), p.getY(), p.getW() * LANELINE_WIDTH / ROAD_WIDTH, l.getX(), l.getY(),l.getW() * LANELINE_WIDTH / ROAD_WIDTH });
				}

				drawQuad(renderer, { accRoad, p.getX() + sign * p.getW() / 2, p.getY(), p.getW() / 2, l.getX() + sign * l.getW() / 2, l.getY(), l.getW() / 2 });
			}
		}
		colorChange1 = (colorChange1 + 2) & 31;

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

		}
		//virus.draw(renderer, &lines[i]);
		if (startpos <= 300 && startpos > 0)
			car2->getTrap()->drawImg(renderer, &lines[300]);
		//lines[i].drawActSprite(renderer, 0);
		if (startpos <= 250 && startpos > 0)
			car2->getObstacle()->drawImg(renderer, &lines[250]);


		if (startpos <= 200 && startpos > 0)
			car2->getTools()->drawImg(renderer, &lines[200]);

		//car
		car2->draw(renderer);

		car2->getTrap()->drawStain(renderer);	//only draws stain
		car2->getTools()->drawmytool(renderer);
	}
	*/
/*
		if (map->dualMode)
		{
			car = map->car2;
			const Motion& motion = map->car2->getMotioin();

			startpos = motion.posX / SEGMENT_LENGTH;
			car->setCurrentPos(&(map->lines[startpos]));

			//perpendicular
			car->setCamHeight(motion.camHeight + motion.velPerpen);
			if (car->isInAir() && (motion.camHeight + car->baseHeight) - (CAMERA_HEIGHT + map->lines[startpos].gety()) < -1e-6) {
				car->setCamHeight(CAMERA_HEIGHT);
				car->setInAir(false);
				if (car->getCurrentPos()->getSlope()) {
					car->setVelLinear(motion.velLinear + motion.velPerpen * car->getCurrentPos()->getSlope() / SEGMENT_LENGTH);
				}
				car->setVelPerpen(0);
			}
			else if (car->isInAir() && motion.velPerpen <= 0) {
				//free fall
				car->setVelPerpen(motion.velPerpen - GRAVITY);
			}
			else if (!car->isInAir()) {
				if (motion.camHeight - CAMERA_HEIGHT > 1e-6) {
					car->setInAir(true);
					car->baseHeight = map->lines[startpos].gety();
				}
				else {
					car->setCamHeight(CAMERA_HEIGHT);
				}
			}

			//velocity modification
			car->setRoadDegree(atan((map->lines[startpos + 1].getx() - map->lines[startpos].getx()) / SEGMENT_LENGTH));

			//speed punishment
			punish = 1.0;
			//cout << map->lines[startpos + 1].getx() - map->lines[startpos].getx() << " " << velM << " ";
			if (motion.posY > map->lines[startpos].getx() + ROAD_WIDTH * motion.velM || motion.posY < map->lines[startpos].getx() - ROAD_WIDTH * motion.velM) {
				punish = (ROAD_WIDTH * motion.velM) / (motion.posY - map->lines[startpos].getx());
				if (punish < 0)
					punish = -punish;
				car->setOutofRoad(true);
			}
			else {
				car->setOutofRoad(false);
			}
			type = map->lines[startpos].getType();
			if ((type & HIGH_FRICTION)) {
				if (!car->isOutofRoad() && (motion.roadMod - 0.6 > 1e-6 || motion.roadMod - 0.6 < -1e-6)) {
					if (motion.roadMod < 0.6)
						car->setRoadMod(motion.roadMod + 0.1);
					else
						car->setRoadMod(motion.roadMod - 0.1);
				}
			}
			else if ((type & LOW_FRICTION)) {
				if (!car->isOutofRoad() && (motion.roadMod - 1.3 > 1e-6 || motion.roadMod - 1.3 < -1e-6)) {
					if (motion.roadMod < 1.3)
						car->setRoadMod(motion.roadMod + 0.1);
					else
						car->setRoadMod(motion.roadMod - 0.1);
				}
			}
			else {
				if (motion.roadMod - 1 > 1e-6 || motion.roadMod - 1 < -1e-6) {
					if (motion.roadMod < 1)
						car->setRoadMod(motion.roadMod + 0.1);
					else
						car->setRoadMod(motion.roadMod - 0.1);
				}
			}

			car->setVelM(car->getRoadMod() * (sin(motion.roadDegree) * (map->lines[startpos + 1].getx() - map->lines[startpos].getx()) + cos(motion.roadDegree) * SEGMENT_LENGTH) / SEGMENT_LENGTH);

			//set car road type
			car->setRoadType(type);

			velX = motion.velLinear * cos(motion.camDegree) * motion.velM * punish;
			velY = motion.velLinear * sin(motion.camDegree) * motion.velM * punish;

			//move in x-direction
			car->setPosX(motion.posX + velX);
			if (motion.posX < 0 || motion.posX >(map->number_of_lines - 20) * SEGMENT_LENGTH)
				car->setPosX(motion.posX - velX);


			//current index of road line
			startpos = motion.posX / SEGMENT_LENGTH;

			//degree between road vector and normal line (same direction as camera degree)
			car->setRoadDegree(atan((map->lines[startpos + 1].getx() - map->lines[startpos].getx()) / (map->lines[startpos + 1].getz() - map->lines[startpos].getz())));


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

			//update startpos and type
			startpos = motion.posX / SEGMENT_LENGTH;
			type = map->lines[startpos].getType();
			car->setRoadType(type);

			if (!car->isInAir()) {
				//special road
				type = map->lines[startpos].getType();
				if (car->getRushing() != ACCROAD && ((type & ACCELERATE_LEFT) || (type & ACCELERATE_RIGHT))) {
					if ((type & ACCELERATE_LEFT) && (motion.posY < map->lines[startpos].getx() && motion.posY > map->lines[startpos].getx() - ROAD_WIDTH * motion.velM)) {
						car->rush(ACCROAD);
					}
					else if ((type & ACCELERATE_RIGHT) && (motion.posY > map->lines[startpos].getx() && motion.posY < map->lines[startpos].getx() + ROAD_WIDTH * motion.velM)) {
						car->rush(ACCROAD);
					}
				}
				//trap
				if ((type & TRAPAREA) && motion.posY < map->lines[startpos].getx() + TRAP_WIDTH * motion.velM && motion.posY > map->lines[startpos].getx() - TRAP_WIDTH * motion.velM)
					car->getTrap()->gettrap(STAIN);
				//tool
				if ((type & TOOLAREA) && motion.posY < map->lines[startpos].getx() + TOOL_WIDTH * motion.velM && motion.posY > map->lines[startpos].getx() - TOOL_WIDTH * motion.velM)
					car->getTools()->getTools();
				//obstacle
				if ((type & OBSTACLEAREA) && motion.posY < map->lines[startpos].getx() + TOOL_WIDTH * motion.velM && motion.posY > map->lines[startpos].getx() - TOOL_WIDTH * motion.velM)
				{
					car->touchobstacle();
					//printf("Touch Obstacle\n");
				}
			}

		}
		*/
/*
		//speed punishment
		double punish = 1.0;
		//cout << map->lines[startpos + 1].getx() - map->lines[startpos].getx() << " " << velM << " ";
		if (motion.posY > map->lines[startpos].getx() + ROAD_WIDTH * motion.velM || motion.posY < map->lines[startpos].getx() - ROAD_WIDTH * motion.velM) {
			punish = (ROAD_WIDTH * motion.velM) / (motion.posY - map->lines[startpos].getx());
			if (punish < 0)
				punish = -punish;
			car->setOutofRoad(true);
		}
		else {
			car->setOutofRoad(false);
		}
		*/
/*if (map->dualMode) {

		car = map->car2;
		const Motion& motion = map->car2->getMotion();

		car->brake();
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
					car->brake(0);
				//car->setAccLinear(-FRICTION_ACC);
			//map->accLinear = -FRICTION_ACC;
				car->rush(NONE);
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
			bool incline = (car->getCurrentPos()->getType() & INCLINE_BACKWARD) || (car->getCurrentPos()->getType() & INCLINE_FORWARD);
			if (!incline && (((motion.accLinear < 0 && motion.accLinear >= -HIGH_FRICTION_ACC - 50) && motion.velLinear < 0) || ((motion.accLinear > 0 && motion.accLinear <= HIGH_FRICTION_ACC + 50) && motion.velLinear > 0))) {
				car->setVelLinear(0);
				car->brake(0);
				//car->setAccLinear(0);
				//map->velLinear = 0;
				//map->accLinear = 0;
			}
		}
	}

	//cout << map->accLinear << " " << map->velLinear << endl;*/
/*
	camDepth(DEFAULT_CAMERA_DEPTH), posX(INITIAL_POS* SEGMENT_LENGTH),
	velAngular(0), velLinear(0), roadDegree(0), camDegree(0), accLinear(0), camHeight(CAMERA_HEIGHT), velM(1)
	// {INITIAL_POS* SEGMENT_LENGTH,0,0,0,0,0,0,DEFAULT_CAMERA_DEPTH,1,CAMERA_HEIGHT}
	*/
/*
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
}*/
/*
if (car->getRushing() != ACCROAD && ((type & ACCELERATE_LEFT) || (type & ACCELERATE_RIGHT))) {
				if ((type & ACCELERATE_LEFT) && (motion.posY < map->lines[startpos].getx() && motion.posY > map->lines[startpos].getx() - ROAD_WIDTH * motion.velM)) {
					car->rush(ACCROAD);
				}
				else if ((type & ACCELERATE_RIGHT) && (motion.posY > map->lines[startpos].getx() && motion.posY < map->lines[startpos].getx() + ROAD_WIDTH * motion.velM)) {
					car->rush(ACCROAD);
				}
			}
			//trap
			if ((type & TRAPAREA) && motion.posY < map->lines[startpos].getx() + TRAP_WIDTH * motion.velM && motion.posY > map->lines[startpos].getx() - TRAP_WIDTH * motion.velM)
			{
				car->getTrap()->gettrap(STAIN);

			}

			//tool
			if ((type & TOOLAREA) && motion.posY < map->lines[startpos].getx() + TOOL_WIDTH * motion.velM && motion.posY > map->lines[startpos].getx() - TOOL_WIDTH * motion.velM)
			{
				car->getTools()->getTools();
			}

			//obstacle
			if ((type & OBSTACLEAREA) && motion.posY < map->lines[startpos].getx() + OBSTACLE_WIDTH * motion.velM && motion.posY > map->lines[startpos].getx() - OBSTACLE_WIDTH * motion.velM)
			{
				car->touchobstacle();

				if (car->getHP() <= 0)
				{
					if (map->dualMode)
						map->endtype = (times == 2 ? PLAYER2 : PLAYER1);
					else
						map->endtype = FAILED;
					map->endtime = SDL_GetTicks64() + 3000;

				}
			}
*/
/*
	if (map->dualMode)
	{
		//collision
		double dx = map->car1->getPosY() - map->car2->getPosY();
		double dz = map->car1->getPosX() - map->car2->getPosX();

		if (dx * dx + dz * dz < 4.0 * (CAR_HALF_LENGTH * CAR_HALF_LENGTH + CAR_HALF_WIDTH * CAR_HALF_WIDTH) * 0.9)
		{
			bool col = false;
			double rd = map->car2->getAxleDegree() - map->car1->getAxleDegree();
			double cos_ = cos(rd), sin_ = sin(rd);//CAR_HALF_LENGTHcos_  CAR_HALF_WIDTH
			double rz[4] = { CAR_HALF_LENGTH * cos_ - CAR_HALF_WIDTH * sin_ - dz,CAR_HALF_LENGTH * cos_ + CAR_HALF_WIDTH * sin_ - dz ,
							-CAR_HALF_LENGTH * cos_ - CAR_HALF_WIDTH * sin_ - dz ,-CAR_HALF_LENGTH * cos_ + CAR_HALF_WIDTH * sin_ - dz };
			double rx[4] = { CAR_HALF_LENGTH * sin_ + CAR_HALF_WIDTH * cos_ - dx,CAR_HALF_LENGTH * sin_ - CAR_HALF_WIDTH * cos_ - dx,
							-CAR_HALF_LENGTH * sin_ + CAR_HALF_WIDTH * cos_ - dx,-CAR_HALF_LENGTH * sin_ - CAR_HALF_WIDTH * cos_ - dx };
			for (int i = 0; i < 4; ++i) {
				if (rz[i] < CAR_HALF_LENGTH && rz[i] > -CAR_HALF_LENGTH && rx[i] < CAR_HALF_WIDTH && rx[i] > -CAR_HALF_WIDTH) {
					//collided,
					double e = 0.6;

					double vz1 = map->car1->getVelLinear() * cos(map->car1->getAxleDegree()), vz2 = map->car2->getVelLinear() * cos(map->car2->getAxleDegree());
					double vx1 = map->car1->getVelLinear() * sin(map->car1->getAxleDegree()), vx2 = map->car2->getVelLinear() * sin(map->car2->getAxleDegree());
					double vx = ((1 - e) * vx1 + (1 + e) * vx2) / 2.0, vz = ((1 - e) * vz1 + (1 + e) * vz2) / 2.0;
					map->car1->setVelLinear(sqrt(vx * vx + vz * vz));
					vx = ((1 + e) * vx1 + (1 - e) * vx2) / 2.0, vz = ((1 + e) * vz1 + (1 - e) * vz2) / 2.0;
					map->car2->setVelLinear(sqrt(vx * vx + vz * vz));

					cos_ = cos(map->car1->getAxleDegree() - map->car2->getAxleDegree());
					double v1 = map->car1->getVelLinear(), v2 = map->car2->getVelLinear() * cos_;
					double v = ((1 - e) * v1 + (1 + e) * v2) / 2.0;
					map->car1->setVelLinear(v);

					v1 = map->car1->getVelLinear() * cos_, v2 = map->car2->getVelLinear();
					v = ((1 - e) * v2 + (1 + e) * v1) / 2.0;
					map->car2->setVelLinear(v);

					if (dz < 0 && map->car1->getRushing()) {
						map->car1->rush(NONE);
					}
					else if (dz > 0 && map->car2->getRushing()) {
						map->car2->rush(NONE);
					}
					col = true;
					break;
				}
			}

			if (!col)
			{
				rd = -rd;
				sin_ = -sin_;
				double rz[4] = { CAR_HALF_LENGTH * cos_ - CAR_HALF_WIDTH * sin_ - dz,CAR_HALF_LENGTH * cos_ + CAR_HALF_WIDTH * sin_ - dz ,
								-CAR_HALF_LENGTH * cos_ - CAR_HALF_WIDTH * sin_ - dz ,-CAR_HALF_LENGTH * cos_ + CAR_HALF_WIDTH * sin_ - dz };
				double rx[4] = { CAR_HALF_LENGTH * sin_ + CAR_HALF_WIDTH * cos_ - dx,CAR_HALF_LENGTH * sin_ - CAR_HALF_WIDTH * cos_ - dx,
								-CAR_HALF_LENGTH * sin_ + CAR_HALF_WIDTH * cos_ - dx,-CAR_HALF_LENGTH * sin_ - CAR_HALF_WIDTH * cos_ - dx };

				for (int i = 0; i < 4; ++i) {
					if (rz[i] < CAR_HALF_LENGTH && rz[i] > -CAR_HALF_LENGTH && rx[i] < CAR_HALF_WIDTH && rx[i] > -CAR_HALF_WIDTH) {
						//collided,
						double e = 0.6;
						cos_ = cos(map->car1->getAxleDegree() - map->car2->getAxleDegree());
						double v1 = map->car1->getVelLinear(), v2 = map->car2->getVelLinear() * cos_;
						double v = ((1 - e) * v1 + (1 + e) * v2) / 2.0;
						map->car1->setVelLinear(v);

						v1 = map->car1->getVelLinear() * cos_, v2 = map->car2->getVelLinear();
						v = ((1 - e) * v2 + (1 + e) * v1) / 2.0;
						map->car2->setVelLinear(v);

						if (dz < 0 && map->car1->getRushing()) {
							map->car1->rush(NONE);
						}
						else if (dz > 0 && map->car2->getRushing()) {
							map->car2->rush(NONE);
						}
						break;
					}
				}
			}

		}
	}*/

	/*
		double x = 0, dx = 0;
		for (int i = 0; i < NUM_LINE; ++i) {

			//curve, default = 0
			if (i > 100 && i <= 300)		// range of turing road
				lines[i].setCurve(0.3);
			else if (i > 300 && i < 600)
				lines[i].setCurve(-0.4);
			else if (i > 700 && i < 1000)
				lines[i].setCurve(-0.15);
			else if (i > 1200 && i < 1400)
				lines[i].setCurve(0.4);
			else if (i > 1500 && i < 2000)
				lines[i].setCurve(0.3);
			else if (i > 2200 && i < 2800)
				lines[i].setCurve(-0.35);


			// y, default = 0
			//range of road up and down

			if (i > 300 && i < 1054) {
				lines[i].sety(sin((i - 300) / 30.0) * CAMERA_HEIGHT);
				lines[i].setSlope(lines[i].gety() - lines[i - 1].gety());
			}
			else if (i > 1200 && i < 2896){
				lines[i].sety(sin((i - 1200) / 20.0) * (CAMERA_HEIGHT * 0.6));
				lines[i].setSlope(lines[i].gety() - lines[i - 1].gety());
			}
			else if (i > 3200 && i < 3300) {
				lines[i].sety((i - 3200) * CAMERA_HEIGHT * 3 / 100.0);
				lines[i].setSlope(lines[i].gety() - lines[i - 1].gety());
				lines[i].addType(INCLINE_PLANE);
			}
			if (lines[i].getSlope() > 1e-6) {
				lines[i].addType(INCLINE_BACKWARD);
			}
			else if (lines[i].getSlope() < -1e-6) {
				lines[i].addType(INCLINE_FORWARD);
			}

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

			// type
			//20 - 52
			if (i >= INITIAL_POS - 10 && i < INITIAL_POS + 22)
				lines[i].addType(STARTPOINT);
			else if (i >= FINAL_POS - 12 && i < FINAL_POS + 20)
				lines[i].addType(ENDPOINT);
			//650 - 670
			else if (i >= 650 && i < 650 + ACCROAD_LENGHT)
				lines[i].addType(ACCELERATE_RIGHT);
			//1800 - 1820
			else if (i >= 1800 && i < 1800 + ACCROAD_LENGHT)
				lines[i].addType(ACCELERATE_LEFT);
			//295 - 300
			else if (i >= 295 && i < 300)
				lines[i].addType(TRAPAREA);
			//195 - 200
			else if (i >= 195 && i < 200)
				lines[i].addType(TOOLAREA);
			//240 - 260
			else if (i >= 245 && i < 255)
				lines[i].addType(OBSTACLEAREA);
			//400 - 600
			else if (i >= 400 && i < 600)
				lines[i].addType(HIGH_FRICTION);
			//700 - 900
			else if (i >= 700 && i <= 900)
				lines[i].addType(LOW_FRICTION);
			else if (i >= 3300 && i <= 3320)
				lines[i].addType(CLIFF);
			else
				lines[i].addType(NORMAL);

		}
		*/