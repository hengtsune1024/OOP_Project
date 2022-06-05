#include "RacingCar.h"
RacingCar::RacingCar() :
	isRushing(NONE), fullEnergy(true), energy(100.0), healthPoint(100.0),
	motion(MOTION_INIT), roadtype(NORMAL), outOfRoad(false), inAir(false), car3D("../images/car1.txt", "../images/car1.bmp", 500)
{}

RacingCar::~RacingCar() {
	/*
	if (image != NULL) {
		delete[]image;
		image = NULL;
	}
	*/
}

RacingCar::RacingCar(const char* obfpath, const char* imgpath, SDL_Renderer* renderer, Line* initpos) :
	virus("../images/coronavirus/", 15, renderer), tools("../images/star/", 5, renderer), rock("../images/rock/rock.txt", "../images/rock/rock.bmp"),
	isRushing(NONE), fullEnergy(true), energy(100.0), healthPoint(100.0), motion(MOTION_INIT), accState(0), roadtype(NORMAL),
	currentPos(initpos), car3D(obfpath, imgpath, 1000), theOtherCar(NULL)
{
	
}

void RacingCar::quit()
{
	// Remove timer in case the call back was not called	
	SDL_RemoveTimer(cartimer);
	SDL_RemoveTimer(chargeTimer);
	virus.quit();

}
void RacingCar::draw(SDL_Renderer* renderer,Engine* engine, bool clean)
{
	//car image

	car3D.draw({ 0,0,0 }, { -motion.Xangle,car3D.getRotation().y,0 }, 0, motion.camDepth, engine, clean, true, HEIGHT);

	//energy bottle
	roundedBoxColor(renderer, 10, 10, 10 + WIDTH / 4, 30, 2, 0xff828282);
	if(energy == 100)
		roundedBoxColor(renderer, 13, 13, 13 + (WIDTH / 4 - 6) * (energy / 100.0), 27, 2, 0xff00ff00);
	else if (energy > 2.5)
		roundedBoxColor(renderer, 13, 13, 13 + (WIDTH / 4 - 6) * (energy / 100.0), 27, 2, 0xff00ffff);
	else
		roundedBoxColor(renderer, 13, 13, 13 + (WIDTH / 4 - 6) * 0.02, 27, 1, 0xff0000ff);

	//HP bottle
	roundedBoxColor(renderer, 10, 40, 10 + WIDTH / 4, 60, 2, 0xff828282);
	if (healthPoint > 2.5)
		roundedBoxColor(renderer, 13, 43, 13 + (WIDTH / 4 - 6) * (healthPoint / 100.0), 57, 2, 0xff0000ff);
	else
		roundedBoxColor(renderer, 13, 43, 13 + (WIDTH / 4 - 6) * 0.02, 57, 1, 0xff0000ff);

	//tool column
	roundedRectangleRGBA(renderer, 25, 80, 60, 115, 1, 255, 0, 255, 255);
	roundedRectangleRGBA(renderer, 65, 80, 100, 115, 1, 255, 0, 255, 255);
	roundedRectangleRGBA(renderer, 105, 80, 140, 115, 1, 255, 0, 255, 255);
}

void RacingCar::drawOtherCar(SDL_Renderer* renderer, Engine* engine, bool clean, double maxy, double camH) {
	/*
	car3D.draw(
		{ motion.posY - theOtherCar->getPosY() ,camH - theOtherCar->getCamHeight() - theOtherCar->getCurrentPos()->gety(),motion.posX - theOtherCar->getPosX()},
		{ 0,(theOtherCar->motion.axleDegree - motion.axleDegree),0},
		motion.camDegree, motion.camDepth, engine, clean, maxy);*/
	theOtherCar->car3D.draw(
		{ motion.posY - theOtherCar->getPosY() ,currentPos->gety() - theOtherCar->getCurrentPos()->gety() + 0,motion.posX - theOtherCar->getPosX()},
		{ -theOtherCar->motion.Xangle,theOtherCar->motion.axleDegree,0 },
		motion.camDegree, motion.camDepth, engine, clean,false, maxy);
}

Uint32 RacingCar::changeData(Uint32 interval, void* param)
{
	RacingCar* p = (RacingCar*)param;
	
	
	Point3D rot = p->car3D.getRotation();
	double dif = p->motion.axleDegree - p->motion.camDegree;

	if (dif < -1e-6) {
		rot.y -= 0.06;
		if (rot.y < dif)
			rot.y = dif;
	}
	else if (dif > 1e-6) {
		rot.y += 0.06;
		if (rot.y > dif)
			rot.y = dif;
	}

	p->car3D.setRotation(rot);
	return interval;
}

void RacingCar::startTimer(Uint32 t)
{
	time = t;
	cartimer = SDL_AddTimer(time, changeData, this); // Set Timer callback
	virus.startTimer(TRAP_INTERVAL);
	tools.startTimer(50);
	chargeTimer = SDL_AddTimer(CHARGE_INTERVAL, charge, this);
}


void RacingCar::RacingCar::stopTimer()
{
	time = 0;
}
/*
void RacingCar::turn(int d)
{
	direct = d;
} */

void RacingCar::setFrictionType(unsigned long long rt) {
	if ((rt & NORMAL) || (rt & HIGH_FRICTION) || (rt & LOW_FRICTION)) {
		roadtype = rt;
	}
}

void RacingCar::brake(int type) 
{
	if (type == -1)
		type = accState;
	else
		accState = type;
	if (inAir) {
		motion.accLinear = 0;
		return;
	}
	//no acc
	if (type == 0) {
		int sign = motion.velLinear<1e-6 && motion.velLinear>-1e-6 ? 0 : (motion.velLinear > 0 ? -1 : 1);
		double acc;
		if ((roadtype & NORMAL) || outOfRoad) {
			motion.accLinear = sign * FRICTION_ACC;
			acc = FRICTION_ACC;
		}
		else if (roadtype & HIGH_FRICTION) {
			motion.accLinear = sign * HIGH_FRICTION_ACC;
			acc = HIGH_FRICTION_ACC;
		}
		else if (roadtype & LOW_FRICTION) {
			motion.accLinear = sign * LOW_FRICTION_ACC;
			acc = LOW_FRICTION_ACC;
		}
		if ((roadtype & INCLINE_BACKWARD) || (roadtype & INCLINE_FORWARD)) {
			double slope = -INCLINE_ACC * currentPos->getSlope() / SEGMENT_LENGTH;
			if (sign){
				motion.accLinear += slope;
				if ((motion.accLinear + motion.velLinear) * sign > 0) {
					motion.accLinear = 0;
					motion.velLinear = 0;
				}
			}
			else if (slope > acc || slope < -acc)
				motion.accLinear = slope > 0 ? slope - acc : slope + acc;
			else {
				motion.accLinear = 0;
				motion.velLinear = 0;
			}
			
		}
	}
	//foward
	else if (type == 1) {
		if ((roadtype & NORMAL) || outOfRoad) {
			motion.accLinear = ACCELERATION - FRICTION_ACC;
		}
		else if (roadtype & HIGH_FRICTION) {
			motion.accLinear = ACCELERATION - HIGH_FRICTION_ACC;
		}
		else if (roadtype & LOW_FRICTION) {
			motion.accLinear = ACCELERATION - LOW_FRICTION_ACC;
		}
		if ((roadtype & INCLINE_BACKWARD) || (roadtype & INCLINE_FORWARD)) {
			motion.accLinear -= INCLINE_ACC * currentPos->getSlope() / SEGMENT_LENGTH;
		}
	}
	//backward
	else if (type == 2) {
		if ((roadtype & NORMAL) || outOfRoad) {
			motion.accLinear = -ACCELERATION + FRICTION_ACC;
		}
		else if (roadtype & HIGH_FRICTION) {
			motion.accLinear = -ACCELERATION + HIGH_FRICTION_ACC;
		}
		else if (roadtype & LOW_FRICTION) {
			motion.accLinear = -ACCELERATION + LOW_FRICTION_ACC;
		}
		if ((roadtype & INCLINE_BACKWARD) || (roadtype & INCLINE_FORWARD)) {
			motion.accLinear -= INCLINE_ACC * currentPos->getSlope() / SEGMENT_LENGTH;
		}
	}

	//cout << motion.accLinear << ',' << motion.velLinear << endl;
}

Uint32 RacingCar::charge(Uint32 interval, void* para) {

	RacingCar* car = (RacingCar*)para;

	if (car->fullEnergy){
		return interval;
	}

	car->energy += ENERGY_RECOVER;
	if (car->energy > 100.0) {
		car->energy = 100.0;
		car->fullEnergy = true;
	}

	return interval;
}

void RacingCar::rush(RushType r) 
{
	switch (r)
	{
		case NONE:
			isRushing = r;
			motion.camDepth = DEFAULT_CAMERA_DEPTH;
			break;
		case ENERGY:
			if (fullEnergy) {
				motion.velLinear = ENERGY_RUSHBEGIN_SPEED;
				motion.camDepth = BEGINRUSH_CAMDEPTH;
				fullEnergy = false;
				energy = 0;
				isRushing = r;
				std::cout << "[Map] rush start" << endl;
			}
			else {
				std::cout << "[Map] not enough energy :" << energy << endl;
			}
			break;

		case ACCROAD:
			isRushing = r;
			motion.velLinear = ACCROAD_RUSHBEGIN_SPEED;
			motion.camDepth = BEGINRUSH_CAMDEPTH;
			//car.rush(ACCROAD);
			std::cout << "[Map] rush start" << endl;
			break;
		case TOOL:
			isRushing = r;
			motion.velLinear = ACCROAD_RUSHBEGIN_SPEED;
			motion.camDepth = BEGINRUSH_CAMDEPTH;
			std::cout << "[Map] rush start" << endl;
			break;
		default:
			break;
	}
	/*
	isRushing = r;
	if (r == ENERGY) {
		fullEnergy = false;
		energy = 0;
	}
	*/
}

void RacingCar::usetool(ToolType type)
{
	switch (tools.usetool(type))
	{
	case SPEEDUP:
		rush(TOOL);
		break;
	case INVINCIBLE:
		printf("INVINCIBLE NOW\n");
		invincible = true;
		break;
	}
}

void RacingCar::touchobstacle()
{
	if (!rock.istouching())
	{
		if (!invincible)
			healthPoint -= motion.velLinear > 0 ? motion.velLinear / 100 : motion.velLinear / -100;
		//cout << healthPoint << endl;
		motion.velLinear = -motion.velLinear;
		if (isRushing) {
			isRushing = NONE;
			motion.velLinear = -MAX_BACKWARD_SPEED;
			motion.camDepth = DEFAULT_CAMERA_DEPTH;
		}
	}
}
//previous code
/*
void RacingCar::init() {
	state = 0;
	//acc = 0;
	direct = FRONT;
	healthPoint = MAX_HP;
	fullyDamaged = false;
	energyPoint = MAX_ENERGY;
	fullyCharged = true;
	rechargeInterval = 0;
	for (int i = 0; i < NUM_CARIMG; ++i) {
		setImage(RACINGCAR_PATH, NUM_CARIMG, renderer,0);
	}

	map = &m;
	map->setCar(this);
	cout << "[RacingCar] Car initialized" << endl;
}


		switch (roadtype) {
			case NORMAL:
				motion.accLinear = sign * FRICTION_ACC;
				break;
			case HIGH_FRICTION:
				motion.accLinear = sign * HIGH_FRICTION_ACC;
				break;
			case LOW_FRICTION:
				motion.accLinear = sign * LOW_FRICTION_ACC;
				break;
		}
		switch (roadtype) {
			case NORMAL:
				motion.accLinear = ACCELERATION - FRICTION_ACC;
				break;
			case HIGH_FRICTION:
				motion.accLinear = ACCELERATION - HIGH_FRICTION_ACC;
				break;
			case LOW_FRICTION:
				motion.accLinear = ACCELERATION - LOW_FRICTION_ACC;
				break;
		}
		switch (roadtype) {
			case NORMAL:
				motion.accLinear = -ACCELERATION + FRICTION_ACC;
				break;
			case HIGH_FRICTION:
				motion.accLinear = -ACCELERATION + HIGH_FRICTION_ACC;
				break;
			case LOW_FRICTION:
				motion.accLinear = -ACCELERATION + LOW_FRICTION_ACC;
				break;
		}
*/


/*
void RacingCar::setPosition(int xx, int yy)
{
	x = xx;
	y = yy;
}


int RacingCar::getWidth()
{
	return image[frame].getWidth();
}

int RacingCar::getHeight()
{
	return image[frame].getHeight();
}
*/
/*
	num = n;
	try {
		image = new Image[num];
	}
	catch (bad_alloc& b) {
		cerr << "[RacingCar] new image error: " << b.what() << endl;
		exit(1);
	}

	for (int i = 0; i < num; i++)
	{
		char file[100];
		sprintf_s(file, 100, "%s%02d.png", path, i + 1);

		image[i].set(file, renderer);
	}

	frame = n / 2 - 1;
	*/
	/*
				if ((sign && ((slope < 0 && motion.accLinear + slope < 0) || (slope > 0 && motion.accLinear + slope > 0))) || (sign == 0 && (slope > FRICTION_ACC || slope < -FRICTION_ACC)))
					motion.accLinear += slope;
				else {
					motion.accLinear = 0;
					if(motion.velLinear)
					motion.velLinear = 0;
				}*/
				/*
					if (p->time != 0)
					{
						if (p->direct == 0)
						{
							if (p->frame > p->num / 2)
								p->frame--;
							else if (p->frame < p->num / 2)
								p->frame++;
						}
						else if (p->direct == -1)
						{
							if (p->frame > 0)
								p->frame--;
						}
						else if (p->direct == 1)
						{
							if (p->frame < p->num - 1)
								p->frame++;
						}
						return interval;
					}*/
					/*
						SDL_Rect d;
						d.x = x;
						d.y = y;
						d.w = image[frame].getWidth();
						d.h = image[frame].getHeight();*

						image[frame].draw(renderer, { NULL }, &d);*/