#include "Map.h"
/*
camDepth(DEFAULT_CAMERA_DEPTH), posX(INITIAL_POS* SEGMENT_LENGTH),
velAngular(0), velLinear(0), roadDegree(0), camDegree(0), accLinear(0), camHeight(CAMERA_HEIGHT), velM(1)
*/
// {INITIAL_POS* SEGMENT_LENGTH,0,0,0,0,0,0,DEFAULT_CAMERA_DEPTH,1,CAMERA_HEIGHT}

Uint32 Map::grass;
Uint32 Map::rumble;
Uint32 Map::road;
Uint32 Map::laneLine;
SDL_Rect Map::viewPort1 = { 0,0,WIDTH,HEIGHT };
SDL_Rect Map::viewPort2 = { WIDTH,0,WIDTH,HEIGHT };
unsigned long long Map::type = 0;

Map::Map() : lines(NUM_LINE), number_of_lines(NUM_LINE)
{}

Map::Map(SDL_Renderer* renderer, bool dual) : lines(NUM_LINE), number_of_lines(NUM_LINE), dualMode(dual),
	car1(new RacingCar("../images/RacingCar/racingcar", 13, renderer, &lines[INITIAL_POS])),
	car2(dual ? new RacingCar("../images/RacingCar/racingcar", 13, renderer, &lines[INITIAL_POS]) : NULL),
	streetlight("../images/streetlight.png", renderer), 
	moon("../images/moon.png", renderer)
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
		//range of road up and down
		if (i > 300 && i < 1054) {
			lines[i].sety(sin((i - 300) / 30.0) * CAMERA_HEIGHT);
			lines[i].setSlope(lines[i].gety() - lines[i - 1].gety());
		}
		else if (i > 1200 && i < 2896){
			lines[i].sety(sin((i - 1200) / 20.0) * (CAMERA_HEIGHT * 0.6));
			lines[i].setSlope(lines[i].gety() - lines[i - 1].gety());
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
		if ((i >= INITIAL_POS - 10 && i < INITIAL_POS + 22) || (i >= FINAL_POS - 12 && i < FINAL_POS + 20))
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
		//248 - 250
		else if (i >= 245 && i < 250)
			lines[i].addType(OBSTACLEAREA);
		//400 - 600
		else if (i >= 400 && i < 600)
			lines[i].addType(HIGH_FRICTION);
		//700 - 900
		else if (i >= 700 && i <= 900)
			lines[i].addType(LOW_FRICTION);
		else
			lines[i].addType(NORMAL);
		
	}
	//type

	car1->setTrap(&lines[300]);
	car1->setTool(&lines[200]);
	car1->setObstacle(&lines[250]);
	car1->setPosition(WIDTH / 2 - car1->getWidth() / 2, HEIGHT - car1->getHeight() + 20);
	car1->turn(0);



	if (dualMode) {
		car1->setPosY(lines[INITIAL_POS].getx() - ROAD_WIDTH / 2);

		car2->setTrap(&lines[300]);
		car2->setTool(&lines[200]);
		car2->setObstacle(&lines[250]);

		car2->setPosition(WIDTH / 2 - car2->getWidth() / 2, HEIGHT - car2->getHeight() + 20);
		car2->setPosY(lines[INITIAL_POS].getx() + ROAD_WIDTH / 2);
		car2->turn(0);
	}
	else {
		car1->setPosY(lines[INITIAL_POS].getx());
	}

	std::cout << "[Map] Map initialized" << endl;
}

Map::~Map() {
	delete car1;
	car1 = NULL;
	if (car2 != NULL) {
		delete car2;
		cout << "delete car2" << endl;
		car2 = NULL;
	}
}

void Map::quit() {
	removeTimer();
	car1->quit();
	if (dualMode)
		car2->quit();
	streetlight.close();
	std::cout << "[Map] Map closed" << endl;
}

void Map::drawQuad(SDL_Renderer* renderer, Quad q) {
	Sint16 vx[4] = { q.x1 - q.w1,q.x2 - q.w2,q.x2 + q.w2,q.x1 + q.w1 };
	Sint16 vy[4] = { q.y1,q.y2 ,q.y2 ,q.y1 };
	filledPolygonColor(renderer, vx, vy, 4, q.color);
}

void Map::draw(SDL_Renderer* renderer) 
{
	SDL_RenderSetViewport(renderer, &viewPort1);
	const Motion& m = car1->getMotioin();

	int startpos = m.posX / SEGMENT_LENGTH;
	int camH = m.camHeight + lines[startpos].gety();
	if (car1->isInAir())
		camH = m.camHeight + car1->baseHeight;
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
		car1->getTrap()->drawImg(renderer, &lines[300]);
	//lines[i].drawActSprite(renderer, 0);
	
	if (startpos <= 250 && startpos > 0)
		car1->getObstacle()->drawImg(renderer, &lines[250]);

	if (startpos <= 200 && startpos > 0)
		car1->getTools()->drawImg(renderer, &lines[200]);

	//car
	car1->draw(renderer);

	/**************************/
	car1->getTrap()->drawStain(renderer);	//only draws stain
	/**************************/

	car1->getTools()->drawmytool(renderer);

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

		/**************************/
		car2->getTrap()->drawStain(renderer);	//only draws stain
		/**************************/

		car2->getTools()->drawmytool(renderer);

		SDL_RenderSetViewport(renderer, NULL);
		thickLineColor(renderer, WIDTH, 0, WIDTH, HEIGHT, 5, 0xff0000ff);
	}
}

Uint32 Map::move(Uint32 interval, void* para) 
{
	Map* map = (Map*)para;

	RacingCar* car = map->car1;
	const Motion& motion = map->car1->getMotioin();

	int startpos = motion.posX / SEGMENT_LENGTH;
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
		car->setVelPerpen(motion.velPerpen - 100);
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
	double velX, velY;
	velX = motion.velLinear * cos(motion.camDegree) * motion.velM * punish;
	velY = motion.velLinear * sin(motion.camDegree) * motion.velM * punish;

	//move in x-direction
	car->setPosX(motion.posX + velX);
	if (motion.posX < 0 || motion.posX >(map->number_of_lines - 20) * SEGMENT_LENGTH)
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

	if (!car->isInAir()) {
		//special road
		type = map->lines[startpos].getType();
		if (!car->isInAir() && car->getRushing() != ACCROAD && ((type & ACCELERATE_LEFT) || (type & ACCELERATE_RIGHT))) {
			if ((type & ACCELERATE_LEFT) && (motion.posY < map->lines[startpos].getx() && motion.posY > map->lines[startpos].getx() - ROAD_WIDTH * motion.velM)) {
				car->rush(ACCROAD);
			}
			else if ((type & ACCELERATE_RIGHT) && (motion.posY > map->lines[startpos].getx() && motion.posY < map->lines[startpos].getx() + ROAD_WIDTH * motion.velM)) {
				car->rush(ACCROAD);
			}
		}
	}
	//trap
	if (!car->isInAir() && (type & TRAPAREA) && motion.posY < map->lines[startpos].getx() + TRAP_WIDTH * motion.velM && motion.posY > map->lines[startpos].getx() - TRAP_WIDTH * motion.velM)
		car->getTrap()->gettrap(STAIN);
	//tool
	if (!car->isInAir() && (type & TOOLAREA) && motion.posY < map->lines[startpos].getx() + TOOL_WIDTH * motion.velM && motion.posY > map->lines[startpos].getx() - TOOL_WIDTH * motion.velM)
		car->getTools()->getTools();

	if (!car->isInAir() && (type & OBSTACLEAREA) && motion.posY < map->lines[startpos].getx() + TOOL_WIDTH * motion.velM && motion.posY > map->lines[startpos].getx() - TOOL_WIDTH * motion.velM)
	{
		car->touchobstacle();
		//printf("Touch Obstacle\n");
	}

	if (map->dualMode) {

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
			car->setVelPerpen(motion.velPerpen - 100);
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
		if (!car->isInAir()) {
			//special road
			type = map->lines[startpos].getType();
			if (!car->isInAir() && car->getRushing() != ACCROAD && ((type & ACCELERATE_LEFT) || (type & ACCELERATE_RIGHT))) {
				if ((type & ACCELERATE_LEFT) && (motion.posY < map->lines[startpos].getx() && motion.posY > map->lines[startpos].getx() - ROAD_WIDTH * motion.velM)) {
					car->rush(ACCROAD);
				}
				else if ((type & ACCELERATE_RIGHT) && (motion.posY > map->lines[startpos].getx() && motion.posY < map->lines[startpos].getx() + ROAD_WIDTH * motion.velM)) {
					car->rush(ACCROAD);
				}
			}
		}
		//trap
		if (!car->isInAir() && map->lines[startpos].getType() == TRAPAREA && motion.posY < map->lines[startpos].getx() + TRAP_WIDTH * motion.velM && motion.posY > map->lines[startpos].getx() - TRAP_WIDTH * motion.velM)
			car->getTrap()->gettrap(STAIN);
		//tool
		if (!car->isInAir() && map->lines[startpos].getType() == TOOLAREA && motion.posY < map->lines[startpos].getx() + TOOL_WIDTH * motion.velM && motion.posY > map->lines[startpos].getx() - TOOL_WIDTH * motion.velM)
			car->getTools()->getTools();

		//obstacle
		if (!car->isInAir() && (type & OBSTACLEAREA) && motion.posY < map->lines[startpos].getx() + TOOL_WIDTH * motion.velM && motion.posY > map->lines[startpos].getx() - TOOL_WIDTH * motion.velM)
		{
			car->touchobstacle();
			printf("Touch Obstacle\n");
		}

	}
	//cout << map->camDegree - map->roadDegree << endl;
	return interval;
}

Uint32 Map::accelerate(Uint32 interval, void* para)
{
	Map* map = (Map*)para;

	RacingCar* car = map->car1;
	const Motion& motion = map->car1->getMotioin();
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
	//cout << car->getAccLinear() << endl;
	if (map->dualMode) {

		car = map->car2;
		const Motion& motion = map->car2->getMotioin();
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

	//cout << map->accLinear << " " << map->velLinear << endl;
	return interval;
}

void Map::startTimer() {

	moveTimer = SDL_AddTimer(MOVE_INTERVAL, move, this);
	accelerateTimer = SDL_AddTimer(ACCELERATE_INTERVAL, accelerate, this);

	car1->startTimer(CAR_INTERVAL);
	if (dualMode)
		car2->startTimer(CAR_INTERVAL);

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

void Map::turn(int d)
{
	car.turn(d);
}

switch (lines[i].getType())
		{
			case NORMAL:
			case TRAPAREA:
			case TOOLAREA:
				rumble = (i >> 2) & 1 ? 0xffffffff : 0xff000000;
				road = (i >> 2) & 1 ? 0xff6b6b6b : 0xff696969;
				drawQuad(renderer, { rumble, p.getX(), p.getY(), p.getW() * 1.2, l.getX(), l.getY(), l.getW() * 1.2 });
				drawQuad(renderer, { road, p.getX(), p.getY(), p.getW(), l.getX(), l.getY(), l.getW() });

				if ((i >> 3) & 1) {
					drawQuad(renderer, { laneLine, p.getX(), p.getY(), p.getW() * LANELINE_WIDTH / ROAD_WIDTH, l.getX(), l.getY(),l.getW() * LANELINE_WIDTH / ROAD_WIDTH });
				}
				break;

			case LOW_FRICTION:
				rumble = (i >> 2) & 1 ? 0xffffffff : 0xff000000;
				road = (i >> 2) & 1 ? 0xffffff80 : 0xffffffff;
				drawQuad(renderer, { rumble, p.getX(), p.getY(), p.getW() * 1.2, l.getX(), l.getY(), l.getW() * 1.2 });
				drawQuad(renderer, { road, p.getX(), p.getY(), p.getW(), l.getX(), l.getY(), l.getW() });

				break;

			case HIGH_FRICTION:
				rumble = (i >> 2) & 1 ? 0xffffffff : 0xff000000;
				road = (i >> 2) & 1 ? 0xff00499A : 0xff00346E;
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
				Uint32 accRoad = ((i - (colorChange1 >> 3)) >> 1) & 1 ? 0xff00ffff : 0xff0000ff;


				int sign = lines[i].getType() == ACCELERATE_RIGHT ? 1 : -1;

				drawQuad(renderer, { rumble, p.getX(), p.getY(), p.getW() * 1.2, l.getX(), l.getY(), l.getW() * 1.2 });
				drawQuad(renderer, { road, p.getX(), p.getY(), p.getW(), l.getX(), l.getY(), l.getW() });

				if ((i >> 3) & 1) {
					drawQuad(renderer, { laneLine, p.getX(), p.getY(), p.getW() * LANELINE_WIDTH / ROAD_WIDTH, l.getX(), l.getY(),l.getW() * LANELINE_WIDTH / ROAD_WIDTH });
				}

				drawQuad(renderer, { accRoad, p.getX() + sign * p.getW() / 2, p.getY(), p.getW() / 2, l.getX() + sign * l.getW() / 2, l.getY(), l.getW() / 2 });
			}
			break;
		}
}*/

