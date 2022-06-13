#include "Map.h"

Uint32 Map::grass;
Uint32 Map::rumble;
Uint32 Map::road;
Uint32 Map::laneLine;
SDL_Rect Map::viewPort0 = { WIDTH / 2,0,WIDTH,HEIGHT };
SDL_Rect Map::viewPort1 = { 0,0,WIDTH,HEIGHT };
SDL_Rect Map::viewPort2 = { WIDTH,0,WIDTH,HEIGHT };
unsigned long long Map::type = 0;

Map::Map(SDL_Renderer* renderer, bool dual) : lines(NUM_LINE), number_of_lines(NUM_LINE), dualMode(dual), endtype(PLAYING), record(0),
	car1(new RacingCar("../images/car/car.txt", "../images/car/car", renderer, &lines[INITIAL_POS])),
	car2(dual ? new RacingCar("../images/car/car.txt", "../images/car/car", renderer, &lines[INITIAL_POS]) : NULL),
	streetlight("../images/streetlight.png", renderer), moon("../images/moon.png", renderer),
	cube("../images/cube/cube.txt", "../images/cube/cube", &lines, CUBE_SIZE / 2.457335),
	virus(renderer), tools(renderer), rock("../images/rock/rock.txt", "../images/rock/rock.bmp")
{
	generateMap();

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

void Map::generateMap() 
{
	// index ranges from 0 to 10000, car starts at 30 and ends at 9500.
	// map object and road design take place from 100 to 9100.
	srand(std::time(NULL));

	//generate indices for map object
	vector<int> trapindex(NUM_TRAP, 0);
	vector<int> toolindex(NUM_TOOL, 0);
	vector<int> obstacleindex(NUM_OBSTACLE, 0);
	vector<int> physicalindex(NUM_PHYSICALITEM, 0);
	bool table[9000] = { false };

	int upper, lower, minDist, range;
	
	//road design
	struct Pair {
		int start;
		int end;
		union {
			double curve;
			struct {
				double height;
				double period;
			};
			unsigned int type;
		};
	} generator;

	// special roads
	// divide 9000 to 9 parts with length 1000
	minDist = 300;
	int previous = -500;
	for (int i = 0; i < 9; ++i)
	{
		// acceleration road
		do {
			generator.start = 1000 * (rand() / (RAND_MAX + 1.0)) + 1000 * i + 100;
		} while (generator.start - previous < minDist && generator.start - previous > -minDist);

		if (rand() & 1) {
			for (int j = generator.start; j <= generator.start + ACCROAD_LENGHT; ++j)
				lines[j].addType(ACCELERATE_RIGHT);
		}
		else {
			for (int j = generator.start; j <= generator.start + ACCROAD_LENGHT; ++j)
				lines[j].addType(ACCELERATE_LEFT);
		}
		previous = generator.start;

		// different friction
		// high_friction range between 50 and 300, low_friction range between 100 and 400
		for (int k = 0; k < 2; ++k)
		{
			switch (rand() % 5)
			{
			case 0:
				range = (200 - 50) * (rand() / (RAND_MAX + 1.0)) + 50;
				generator.start = (500 - range) * (rand() / (RAND_MAX + 1.0)) + 1000 * i + k * 500 + 100;
				generator.end = generator.start + range;
				for (int j = generator.start; j <= generator.end; ++j)
					lines[j].addType(HIGH_FRICTION);
				break;
			case 1:
			case 2:
				range = (400 - 100) * (rand() / (RAND_MAX + 1.0)) + 100;
				generator.start = (500 - range) * (rand() / (RAND_MAX + 1.0)) + 1000 * i + k * 500 + 100;
				generator.end = generator.start + range;
				for (int j = generator.start; j <= generator.end; ++j)
					lines[j].addType(LOW_FRICTION);
				break;
			}
		}
	}
	// xchange
	// divide 9000 to 10 parts with length 900, the ychange range is within 200 to 800
	double divert = 0, total;
	short sign = 1;
	for (int i = 0; i < 10; ++i) 
	{
		if (divert < 1e-6 && divert > -1e-6)
			sign = (rand() & 1) ? 1 : -1;
		else if (divert > 0)
			sign = -1;
		else
			sign = 1;
		do{
			generator.curve = sign * ((0.8 - 0.1) * (rand() / (RAND_MAX + 1.0)) + 0.1);
			range = (800 - 200) * (rand() / (RAND_MAX + 1.0)) + 200;
			generator.start = (900 - range) * (rand() / (RAND_MAX + 1.0)) + 900 * i + 100;
			generator.end = generator.start + range;
			total = range * (range - 1) * generator.curve / 2.0;
		} while (total <= -75000 || total >= 75000);
		divert += total;
		for (int j = generator.start; j <= generator.end; ++j) {
			lines[j].setCurve(generator.curve);
		}
	}
	// ychange
	// divide 9000 to 6 parts with length 1500
	// range between 900 adn 1400, height between 0.5 and 1.2 (CAMERA_HEIGHT), period between 20 and 40
	int n;
	bool side;
	double cos_, sin_, critV;
	for (int i = 0; i < 6; ++i) 
	{
		//if (rand() & 7) 
		if(false)
		{
			//sin function
			generator.height = ((1.2 - 0.5) * (rand() / (RAND_MAX + 1.0)) + 0.5) * CAMERA_HEIGHT;
			generator.period = (40 - 20) * (rand() / (RAND_MAX + 1.0)) + 20;
			n = ((1400 - 900) * (rand() / (RAND_MAX + 1.0)) + 900) / (2 * PI * generator.period);
			range = n * (2 * PI * generator.period);
			generator.start = (1500 - range) * (rand() / (RAND_MAX + 1.0)) + 1500 * i + 100;
			generator.end = generator.start + range;
			for (int j = generator.start; j <= generator.end; ++j)
			{
				cos_ = cos((j - generator.start) / generator.period);
				sin_ = sin((j - generator.start) / generator.period);
				critV = GRAVITY * (generator.period * generator.period + cos_ * cos_ * generator.height * generator.height) / (sin_ * generator.height) / lines[j].getRoadVelM() / lines[j].getRoadVelM() * 50;
				if (critV < 0)
					critV = -critV;
				lines[j].sety(sin_ * generator.height);
				lines[j].setCritVel(critV);
				lines[j].setSlope(lines[j].gety() - lines[j - 1].gety());

				if (lines[j].getSlope() > 1e-6)
					lines[j].addType(INCLINE_BACKWARD);
				else if (lines[j].getSlope() < -1e-6)
					lines[j].addType(INCLINE_FORWARD);
			}
		}
		else 
		{
			//line function
			side = rand() & 1;
			generator.height = ((3.0 - 1.0) * (rand() / (RAND_MAX + 1.0)) + 1.0) * CAMERA_HEIGHT;
			range = (300 - 100) * (rand() / (RAND_MAX + 1.0)) + 100;
			generator.start = (500 - range) * (rand() / (RAND_MAX + 1.0)) + 1500 * i + (side ? 1000 : 0) + 100;
			generator.end = generator.start + range;
			for (int j = generator.start; j <= generator.end; ++j)
			{
				lines[j].sety((j - generator.start) * generator.height / range);
				lines[j].setSlope(lines[j].gety() - lines[j - 1].gety());
				lines[j].addType(INCLINE_BACKWARD);
				lines[j].addType(INCLINE_PLANE);
				lines[j].setCritVel(-1);
				if (lines[j].getType() & HIGH_FRICTION)
					lines[j].deleteType(HIGH_FRICTION);
				else if(lines[j].getType() & LOW_FRICTION)
					lines[j].deleteType(LOW_FRICTION);
			}
			for (int j = generator.end - 5; j <= generator.end + 15; ++j){
				lines[j].addType(CLIFF);
			}
			for (int j = generator.end - 100 >= 15 ? generator.end - 100 - 15 : 0; j <= generator.end - 100 + 30 && j < 9000; ++j) {
				table[j] = true;
			}
			
			//one more sin function
			generator.height = ((1.2 - 0.5) * (rand() / (RAND_MAX + 1.0)) + 0.5) * CAMERA_HEIGHT;
			generator.period = (40 - 20) * (rand() / (RAND_MAX + 1.0)) + 20;
			n = ((900 - 400) * (rand() / (RAND_MAX + 1.0)) + 400) / (2 * PI * generator.period);
			range = n * (2 * PI * generator.period);
			generator.start = (1000 - range) * (rand() / (RAND_MAX + 1.0)) + 1500 * i + (side ? 0 : 500) + 100;
			generator.end = generator.start + range;
			for (int j = generator.start; j <= generator.end; ++j)
			{
				cos_ = cos((j - generator.start) / generator.period);
				sin_ = sin((j - generator.start) / generator.period);
				critV = GRAVITY * (generator.period * generator.period + cos_ * cos_ * generator.height * generator.height) / (sin_ * generator.height) / lines[j].getRoadVelM() / lines[j].getRoadVelM() * 50;
				if (critV < 0)
					critV = -critV;
				lines[j].sety(sin_ * generator.height);
				lines[j].setCritVel(critV);
				lines[j].setSlope(lines[j].gety() - lines[j - 1].gety());

				if (lines[j].getSlope() > 1e-6)
					lines[j].addType(INCLINE_BACKWARD);
				else if (lines[j].getSlope() < -1e-6)
					lines[j].addType(INCLINE_FORWARD);
			}
			
		}
	}
	// trap
	range = 9000 / NUM_TRAP;
	minDist = range / 2;
	for (int i = 0; i < NUM_TRAP; ++i) {
		lower = 100 + range * i + 1;
		if (i > 0 && lower < trapindex[i - 1] + minDist + 1)
			lower = trapindex[i - 1] + minDist + 1;
		upper = 100 + range * (i + 1);
		do {
			trapindex[i] = (upper - lower) * (rand() / (RAND_MAX + 1.0)) + lower;
		} while (table[trapindex[i] - 100]);
		for (int j = trapindex[i] - 100 >= 10 ? trapindex[i] - 100 - 10 : 0; j <= trapindex[i] - 100 + 10 && j < 9000; ++j)
			table[j] = true;
	}
	// tool
	range = 9000 / NUM_TOOL;
	minDist = range / 2;
	for (int i = 0; i < NUM_TOOL; ++i) {
		lower = 100 + range * i + 1;
		if (i > 0 && lower < toolindex[i - 1] + minDist + 1)
			lower = toolindex[i - 1] + minDist + 1;
		upper = 100 + range * (i + 1);
		do {
			toolindex[i] = (upper - lower) * (rand() / (RAND_MAX + 1.0)) + lower;
		} while (table[toolindex[i] - 100]);
		for (int j = toolindex[i] - 100 >= 10 ? toolindex[i] - 100 - 10 : 0; j <= toolindex[i] - 100 + 10 && j < 9000; ++j)
			table[j] = true;
	}
	// obstacle
	range = 9000 / NUM_OBSTACLE;
	minDist = range / 2;
	for (int i = 0; i < NUM_OBSTACLE; ++i) {
		lower = 100 + range * i + 1;
		if (i > 0 && lower < obstacleindex[i - 1] + minDist + 1)
			lower = obstacleindex[i - 1] + minDist + 1;
		upper = 100 + range * (i + 1);
		do {
			obstacleindex[i] = (upper - lower) * (rand() / (RAND_MAX + 1.0)) + lower;
		} while (table[obstacleindex[i] - 100]);
		for (int j = obstacleindex[i] - 100 >= 10 ? obstacleindex[i] - 100 - 10 : 0; j <= obstacleindex[i] - 100 + 10 && j < 9000; ++j)
			table[j] = true;
	}
	// physical item
	// divide 9000 to 10 parts with length 900, the ychange range is within 200 to 800
	range = 900;
	n = NUM_PHYSICALITEM / 10;
	for (int i = 0; i < 10; ++i) {
		lower = i * range + 100;
		upper = lower + range;
		for (int j = 0; j < n; ++j) {
			do {
				physicalindex[i * n + j] = (upper - lower) * (rand() / (RAND_MAX + 1.0)) + lower;
			} while (table[physicalindex[i * n + j] - 100]);
			for (int k = physicalindex[i * n + j] - 100 >= 1 ? physicalindex[i * n + j] - 100 - 1 : 0; k <= physicalindex[i * n + j] - 100 + 1 && k < 9000; ++k)
				table[k] = true;
		}
	}


	double z = 0, x = 0, dx = 0;
	for (int i = 0; i < NUM_LINE; ++i)
	{
		//z
		lines[i].setz(z);
		z += SEGMENT_LENGTH;

		//x
		x += dx;
		dx += lines[i].getCurve();
		lines[i].setx(x);

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
		else
			lines[i].addType(NORMAL);

	}

	//add type to lines and set objects' positions
	for (int i = 0; i < NUM_TRAP; ++i) 
	{
		virus.setTrap(&lines[trapindex[i]], trapindex[i], i);
		for (int j = trapindex[i] - TRAP_HALFLENGTH; j <= trapindex[i] + TRAP_HALFLENGTH; ++j)
			lines[j].addType(TRAPAREA);
	}
	for (int i = 0; i < NUM_TOOL; ++i) 
	{
		tools.setTool(&lines[toolindex[i]], toolindex[i], i);
		for (int j = toolindex[i] - TOOL_HALFLENGTH; j <= toolindex[i] + TOOL_HALFLENGTH; ++j)
			lines[j].addType(TOOLAREA);
	}
	for (int i = 0; i < NUM_OBSTACLE; ++i) 
	{
		rock.setObstacle(&lines[obstacleindex[i]], obstacleindex[i], i);
		for (int j = obstacleindex[i] - OBSTACLE_HALFLENGTH; j <= obstacleindex[i] + OBSTACLE_HALFLENGTH; ++j)
			lines[j].addType(OBSTACLEAREA);
	}
	for (int i = 0; i < NUM_PHYSICALITEM; ++i)
	{
		cube.setItem(&lines[physicalindex[i]], physicalindex[i], i);
	}
}

void Map::quit() {
	removeTimer();
	car1->quit();
	if (dualMode)
		car2->quit();
	streetlight.close();
	moon.close();
	cube.close();
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
	map->cube.logic(&(map->lines), &(map->rock));

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
	int startpos, camH,maxy, moonW;

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

		for (int i = startpos > 50 ? startpos - 50 : 1; i < startpos + 300 && i < NUM_LINE - 5; ++i)
		{
			Line& l = lines[i];
			const Line& p = lines[i - 1];

			if (i < startpos && (l.getType() & CLIFF))
				continue;

			l.project(m.posY, camH, m.posX, m.camDegree, m.camDepth);

			if (l.getW() < 1e-6 && l.getW() > -1e-6)
				continue;

			l.setClip(maxy);
			if (l.getY() >= maxy)
				continue;

			maxy = l.getY();

			//road type
			type = lines[i].getType();
			if ((type & LOW_FRICTION)) 
			{
				grass = (i >> 2) & 1 ? 0xff10c810 : 0xff009A00;
				drawQuad(renderer, { grass,  WIDTH / 2, p.getY(), WIDTH / 2, WIDTH / 2, l.getY(), WIDTH / 2 });
				rumble = (i >> 2) & 1 ? 0xffffffff : 0xff000000;
				road = (i >> 2) & 1 ? 0xffffff80 : 0xffffffff;
				drawQuad(renderer, { rumble, p.getX(), p.getY(), p.getW() * 1.2, l.getX(), l.getY(), l.getW() * 1.2 });
				drawQuad(renderer, { road, p.getX(), p.getY(), p.getW(), l.getX(), l.getY(), l.getW() });
			}
			else if ((type & HIGH_FRICTION)) 
			{
				grass = (i >> 2) & 1 ? 0xff10c810 : 0xff009A00;
				drawQuad(renderer, { grass,  WIDTH / 2, p.getY(), WIDTH / 2, WIDTH / 2, l.getY(), WIDTH / 2 });
				rumble = (i >> 2) & 1 ? 0xffffffff : 0xff000000;
				road = (i >> 2) & 1 ? 0xff00499A : 0xff00346E;
				drawQuad(renderer, { rumble, p.getX(), p.getY(), p.getW() * 1.2, l.getX(), l.getY(), l.getW() * 1.2 });
				drawQuad(renderer, { road, p.getX(), p.getY(), p.getW(), l.getX(), l.getY(), l.getW() });
			}
			else if ((type & ENDPOINT) || (type & STARTPOINT)) 
			{
				grass = (i >> 2) & 1 ? 0xff10c810 : 0xff009A00;
				drawQuad(renderer, { grass,  WIDTH / 2, p.getY(), WIDTH / 2, WIDTH / 2, l.getY(), WIDTH / 2 });
				double width_scale = 1.2;
				for (int j = 0; j <= 7; ++j) 
				{

					width_scale = 1.2 * (15 - (j << 1)) / 15;
					rumble = ((i >> 2) + j) & 1 ? 0xffffffff : 0xff000000;

					drawQuad(renderer, { rumble,p.getX(), p.getY(), p.getW() * width_scale, l.getX(), l.getY(), l.getW() * width_scale });
				}
			}
			else if (type & INCLINE_PLANE) 
			{
				grass = (i >> 2) & 1 ? 0xff10c810 : 0xff009A00;
				drawQuad(renderer, { grass,  WIDTH / 2, p.getY(), WIDTH / 2, WIDTH / 2, l.getY(), WIDTH / 2 });
				drawQuad(renderer, { 0xff411e02,p.getX(), p.getY(), p.getW() * 1.2, l.getX(), l.getY(), l.getW() * 1.2 });
				double width_scale = 1;
				for (int j = 0; j <= 7; ++j) 
				{
					width_scale = (15 - (j << 1)) / 15;
					road = j & 1 ? 0xff265ba0 : 0xff0d3c78;
					drawQuad(renderer, { road,p.getX(), p.getY(), p.getW() * width_scale, l.getX(), l.getY(), l.getW() * width_scale });
				}
			}
			else if ((type & NORMAL) || (type & TRAPAREA) || (type & TOOLAREA) || (type & OBSTACLEAREA)) 
			{
				grass = (i >> 2) & 1 ? 0xff10c810 : 0xff009A00;
				drawQuad(renderer, { grass,  WIDTH / 2, p.getY(), WIDTH / 2, WIDTH / 2, l.getY(), WIDTH / 2 });
				rumble = (i >> 2) & 1 ? 0xffffffff : 0xff000000;
				road = (i >> 2) & 1 ? 0xff6b6b6b : 0xff696969;
				drawQuad(renderer, { rumble, p.getX(), p.getY(), p.getW() * 1.2, l.getX(), l.getY(), l.getW() * 1.2 });
				drawQuad(renderer, { road, p.getX(), p.getY(), p.getW(), l.getX(), l.getY(), l.getW() });

				if ((i >> 3) & 1) {
					drawQuad(renderer, { laneLine, p.getX(), p.getY(), p.getW() * LANELINE_WIDTH / ROAD_WIDTH, l.getX(), l.getY(),l.getW() * LANELINE_WIDTH / ROAD_WIDTH });
				}
			}

			if ((type & ACCELERATE_RIGHT) || (type & ACCELERATE_LEFT)) 
			{
				Uint32 accRoad = ((i - (colorChange1 >> 3)) >> 1) & 1 ? 0xff00ffff : 0xff0000ff;

				int sign = (type & ACCELERATE_RIGHT) ? 1 : -1;

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
				virus.draw3D(pos, m.camDegree, m.camDepth, &engine, clean, i, lines[virus.getIndex(i)].getClip());


		for (int i = 0; i < NUM_PHYSICALITEM; ++i)
			if (startpos >= cube.getIndex(i) - 300 && startpos <= cube.getIndex(i))
				cube.draw3D(pos, m.camDegree, m.camDepth, &engine, clean, i, lines[cube.getIndex(i)].getClip());

		for (int i = 0; i < NUM_OBSTACLE; ++i)
			if (startpos >= rock.getIndex(i) - 300 && startpos <= rock.getIndex(i)) {
				rock.draw3D(pos, m.camDegree, m.camDepth, &engine, clean, i, lines[rock.getIndex(i)].getClip());
			}

		for (int i = 0; i < NUM_TOOL; ++i)
			if (startpos >= tools.getIndex(i) - 300 && startpos <= tools.getIndex(i)) {
				tools.draw3D(pos, m.camDegree, m.camDepth, &engine, clean, i, lines[tools.getIndex(i)].getClip());
			}

		if (otherCar != NULL && otherCar->getPosX() > m.posX - 50 * SEGMENT_LENGTH && otherCar->getPosX() - m.posX < 300 * SEGMENT_LENGTH) {
			car->setCurrentPos(&lines[(int)(m.posX / SEGMENT_LENGTH)]);
			otherCar->setCurrentPos(&lines[(int)(otherCar->getPosX() / SEGMENT_LENGTH)]);
			car->drawOtherCar(renderer, &engine, clean, lines[otherCar->getIndex()].getClip(), camH);
		}

		car->draw3D({ 0,0,0 }, car->getMotion().camDegree, car->getMotion().camDepth, &engine, clean, 0, HEIGHT);

		engine.drawAll(renderer);

		//car
		car->draw(renderer, &engine, clean);

		tools.drawmytool(renderer, (dualMode ? times - 1 : true));

		virus.drawStain(renderer, (dualMode ? times - 1 : true));	//only draws stain


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
	int startpos, front, back;
	double punish, midY, camH, dist, velX, velY;
	
	do {
		const Motion& motion = car->getMotion();
		startpos = motion.posX / SEGMENT_LENGTH;
		car->setCurrentPos(&(map->lines[startpos]));
		type = map->lines[startpos].getType();

		//perpendicular (z-direction)
		car->setCamHeight(motion.camHeight + motion.velPerpen);
		if (car->isInAir() && ((motion.camHeight + motion.baseHeight) - (CAMERA_HEIGHT + map->lines[startpos].gety()) < -1e-6)) {
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

		dist = CAR_HALF_LENGTH * cos(motion.axleDegree);
		front = (motion.posX + CAMERA_CARMIDPOINT_DIST + dist) / SEGMENT_LENGTH;
		back = (motion.posX + CAMERA_CARMIDPOINT_DIST - dist) / SEGMENT_LENGTH;
		

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

		velX = motion.velLinear * cos(motion.axleDegree) * punish * motion.velM;
		velY = motion.velLinear * sin(motion.axleDegree) * punish * motion.velM;

		//move in x-direction
		car->setPosX(motion.posX + velX);
		if (motion.posX < 30 * SEGMENT_LENGTH || motion.posX >(map->number_of_lines - 20) * SEGMENT_LENGTH || (velX < 0 && (map->lines[(int)(motion.posX / SEGMENT_LENGTH)].getType() & CLIFF)))
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

				int pos = motion.posX / SEGMENT_LENGTH;

				if (motion.posX < SEGMENT_LENGTH || motion.posX >(map->number_of_lines - 20) * SEGMENT_LENGTH || (velX < 0 && (map->lines[pos].getType() & CLIFF)) || (map->lines[pos].getType() & OBSTACLEAREA))
				{
					car->setPosY(motion.posY + velY);
					car->setPosX(motion.posX + velX);
				}
				map->carCollision(car);
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

				if ((critVel > -1e-6 && velX * velX > critVel) || (type & CLIFF)) {
					car->setVelPerpen(velX * (map->lines[startpos].getSlope() / sqrt(SEGMENT_LENGTH * SEGMENT_LENGTH + map->lines[startpos].getSlope() * map->lines[startpos].getSlope())));
				
					if (motion.velPerpen < GRAVITY * 5 && !(type & CLIFF)) {
						car->setVelPerpen(0);
					}
					else {
						car->setInAir(true, map->lines[startpos].gety());
						if ((type & CLIFF) && (velX < 20)){
							car->setPosX(motion.posX + 5 * SEGMENT_LENGTH);
						}
					}
				}
			}

			startpos = (motion.posX + CAMERA_CARMIDPOINT_DIST) / SEGMENT_LENGTH;
			midY = motion.posY + CAMERA_CARMIDPOINT_DIST * sin(motion.axleDegree);
			type = map->lines[startpos].getType();
			car->setCurrentPos(&(map->lines[startpos]));

			//rush
			if (car->getRushing() != ACCROAD && ((type & ACCELERATE_LEFT) || (type & ACCELERATE_RIGHT))) {
				if ((type & ACCELERATE_LEFT) && (midY < map->lines[startpos].getx() && midY > map->lines[startpos].getx() - ROAD_WIDTH * motion.velM)) {
					car->rush(ACCROAD);
				}
				else if ((type & ACCELERATE_RIGHT) && (midY > map->lines[startpos].getx() && midY < map->lines[startpos].getx() + ROAD_WIDTH * motion.velM)) {
					car->rush(ACCROAD);
				}
			}

			if (map->cube.collide(car)) 
			{
				car->setPosY(motion.posY - velY / 2);
				car->setPosX(motion.posX - velX / 2);
			}
			
		}

		startpos = (motion.posX + CAMERA_CARMIDPOINT_DIST) / SEGMENT_LENGTH;
		midY = motion.posY + CAMERA_CARMIDPOINT_DIST * sin(motion.axleDegree);
		type = map->lines[startpos].getType();
		car->setCurrentPos(&(map->lines[startpos]));
		camH = car->isInAir() ? motion.camHeight + motion.baseHeight : motion.camHeight + map->lines[startpos].gety();

		//trap
		if (type & TRAPAREA)
		{
			int index = map->virus.getNearestTrap(startpos);
			if (map->virus.hitTrap(midY, camH - CAMERA_HEIGHT, motion.velM, index))
				car->gettrap(map->virus.gettrap((map->dualMode ? times - 1 : true), index));
				//map->virus.gettrap(STAIN, (map->dualMode ? times - 1 : true), index);
		}
		//tool
		if (type & TOOLAREA)
		{
			int index = map->tools.getNearestTool(startpos);
			if (map->tools.hitTool(midY, camH - CAMERA_HEIGHT, motion.velM, index))
				map->tools.getTools((map->dualMode ? times - 1 : true), index);
		}

		//obstacle
		if (type & OBSTACLEAREA)
		{
			int index = map->rock.getNearestObstacle(startpos);
			if (!map->rock.getBroken(index) && map->rock.hitObstacle(midY,camH - CAMERA_HEIGHT, index))
			{
				car->touchobstacle(map->rock, index, map->lines);

				if (car->getHP() <= 0)
				{
					if (map->dualMode)
						map->endtype = (times == 2 ? PLAYER2 : PLAYER1);
					else
						map->endtype = FAILED;
					map->endtime = SDL_GetTicks64() + 3000;

				}
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

void Map::carCollision(RacingCar* car) 
{
	if (!dualMode)
		return;

	RacingCar* otherCar = car->getOtherCar();
	double dz = car->getPosX() - otherCar->getPosX();
	double e = 0.6;
	double cos_ = cos(car1->getAxleDegree() - car2->getAxleDegree());
	double v1 = car->getVelLinear();
	double v2 = otherCar->getVelLinear();
	bool car1rush = car->getRushing(), car2rush = otherCar->getRushing();
	bool car1invincible = car->getInvincible(), car2invincible = otherCar->getInvincible();

	//damage
	if (!car1invincible) {
		if (car1rush || car2rush)
			(*car) -= v1 * v1 / 2 / 100000;
		else
			(*car) -= v1 * v1 / 2 / 1000000;
		if (car->getHP() <= 0)
		{
			endtype = (car == car1) ? PLAYER2 : PLAYER1;
			endtime = SDL_GetTicks64() + 3000;
		}
	}
	if (!car2invincible) {
		if (car1rush || car2rush)
			(*otherCar) -= v1 * v1 / 2 / 100000;
		else
			(*otherCar) -= v1 * v1 / 2 / 1000000;
		if (car->getHP() <= 0)
		{
			endtype = (endtype == PLAYING) ? PLAYER1 : ALLDEAD;
			endtime = SDL_GetTicks64() + 3000;
		}
	}
	//velocity
	if (car1invincible ^ car2invincible) {
		if (car1invincible && car1rush) {
			if (dz < 0) {
				otherCar->setVelLinear(((1 - e) * v2 + (1 + e) * v1 * cos_) / 2.0);
				if (car->getPosY() > otherCar->getPosY()) {
					otherCar->setAxleDegree(-MAX_ROTATE_DEGREE);
				}
				else {
					otherCar->setAxleDegree(MAX_ROTATE_DEGREE);
				}
			}
		}
		else if (car2invincible && car2rush) {
			if (dz > 0) {
				car->setVelLinear(((1 - e) * v1 + (1 + e) * v2 * cos_) / 2.0);
				if (otherCar->getPosY() > car->getPosY()) {
					car->setAxleDegree(-MAX_ROTATE_DEGREE);
				}
				else {
					car->setAxleDegree(-MAX_ROTATE_DEGREE);
				}
			}
		}
		else {
			car->setVelLinear(((1 - e) * v1 + (1 + e) * v2 * cos_) / 2.0);
			otherCar->setVelLinear(((1 - e) * v2 + (1 + e) * v1 * cos_) / 2.0);
		}
	}
	else if (car1invincible && car2invincible)
	{
		if (car1rush == car2rush) {
			car->setVelLinear(((1 - e) * v1 + (1 + e) * v2 * cos_) / 2.0);
			otherCar->setVelLinear(((1 - e) * v2 + (1 + e) * v1 * cos_) / 2.0);
		}
		else if (car1rush && dz < 0) {
			otherCar->setVelLinear(((1 - e) * v2 + (1 + e) * v1 * cos_) / 2.0);
			if (car->getPosY() > otherCar->getPosY()) {
				otherCar->setAxleDegree(-MAX_ROTATE_DEGREE);
			}
			else {
				otherCar->setAxleDegree(MAX_ROTATE_DEGREE);
			}
		}
		else if (car2rush && dz > 0) {
			car->setVelLinear(((1 - e) * v1 + (1 + e) * v2 * cos_) / 2.0);
			if (otherCar->getPosY() > car->getPosY()) {
				car->setAxleDegree(-MAX_ROTATE_DEGREE);
			}
			else {
				car->setAxleDegree(-MAX_ROTATE_DEGREE);
			}
		}
	}
	else {
		car->setVelLinear(((1 - e) * v1 + (1 + e) * v2 * cos_) / 2.0);
		otherCar->setVelLinear(((1 - e) * v2 + (1 + e) * v1 * cos_) / 2.0);
	}
	//rush
	if (!car1invincible) {
		car->rush(NONE);
	}
	if (!car2invincible) {
		otherCar->rush(NONE);
	}
}

void Map::changecar()
{
	printf("%lf\n", car1->getPosX());
	printf("%lf\n", car2->getPosX());

	double data = car1->getPosX();
	car1->setPosX(car2->getPosX());
	car2->setPosX(data);
	data = car1->getPosY();
	car1->setPosY(car2->getPosY());
	car2->setPosY(data);
	data = car1->getCamHeight();
	car1->setCamHeight(car2->getCamHeight() * 1.5);
	car2->setCamHeight(data * 1.5);
	
	/*
	SDL_Delay(1000);
	car1 = car2;
	car2 = car;
	//printf("camH= %lf\n", car1->getCamHeight());
	car1->setCamHeight(car1->getCamHeight() * 1.5);
	car2->setCamHeight(car2->getCamHeight() * 1.5);
	*/
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
					//("Touch Obstacle\n");
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
		/*
						if (!car->getInvincible())
						{
							if (car->getRushing() || otherCar->getRushing())
								(*car) -= motion.velLinear * motion.velLinear / 2 / 100000;
							else
								(*car) -= motion.velLinear * motion.velLinear / 2 / 1000000;

							if (car->getHP() <= 0)
							{
								map->endtype = PLAYER2;
								map->endtime = SDL_GetTicks64() + 3000;
							}

							if (dz < 0 && car->getRushing())
								car->rush(NONE);

							car->setVelLinear(((1 - e) * v1 + (1 + e) * v2 * cos_) / 2.0);
						}
						else if (!car->getRushing()) {
							car->setVelLinear(((1 - e) * v1 + (1 + e) * v2 * cos_) / 2.0);
						}

						if (!otherCar->getInvincible())
						{
							if (car->getRushing() || otherCar->getRushing())
								(*otherCar) -= motion.velLinear * motion.velLinear / 2 / 100000;
							else
								(*otherCar) -= motion.velLinear * motion.velLinear / 2 / 1000000;
							if (otherCar->getHP() <= 0)
							{
								map->endtype = map->endtype == PLAYING ? PLAYER1 : ALLDEAD;
								map->endtime = SDL_GetTicks64() + 3000;
							}

							if (dz > 0 && otherCar->getRushing())
								car->rush(NONE);

							otherCar->setVelLinear(((1 - e) * v2 + (1 + e) * v1 * cos_) / 2.0);
						}*/