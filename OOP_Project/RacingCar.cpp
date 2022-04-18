#include "RacingCar.h"
RacingCar::RacingCar() {

}
RacingCar::RacingCar(const char* path, int n, SDL_Renderer* renderer): 
	isRushing(false), fullEnergy(true), energy(100.0)
{
	num = n;
	image = new Image[num];

	for (int i = 0; i < num; i++)
	{
		char file[100];
		sprintf_s(file, 100, "%s%02d.png", path, i + 1);

		image[i].set(file, renderer);
	}

	frame = n / 2 - 1;
}

void RacingCar::quit()
{
	// Free loaded image	
	for (int i = 0; i < num; i++)
	{
		image[i].close();
	}

	// Remove timer in case the call back was not called	
	SDL_RemoveTimer(timerID);
	SDL_RemoveTimer(chargeTimer);
}

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

void RacingCar::draw(SDL_Renderer* renderer)
{
	//car image
	SDL_Rect d;
	d.x = x;
	d.y = y;
	d.w = image[frame].getWidth();
	d.h = image[frame].getHeight();

	image[frame].draw(renderer, { NULL }, &d);

	//energy bottle
	roundedBoxColor(renderer, 10, 10, 10 + WIDTH / 4, 30, 2, 0xff828282);
	if(energy == 100)
		roundedBoxColor(renderer, 13, 13, 13 + (WIDTH / 4 - 6) * (energy / 100.0), 27, 2, 0xff00ff00);
	else if (energy > 2.5)
		roundedBoxColor(renderer, 13, 13, 13 + (WIDTH / 4 - 6) * (energy / 100.0), 27, 2, 0xff00ffff);
	else
		roundedBoxColor(renderer, 13, 13, 13 + (WIDTH / 4 - 6) * 0.02, 27, 1, 0xff0000ff);
}

Uint32 RacingCar::changeData(Uint32 interval, void* param)
{
	RacingCar* p = (RacingCar*)param;
	if (p->time != 0)
	{
		if (p->direct == 0)
		{
			if (p->frame > 10)
				p->frame--;
			else if (p->frame < 10)
				p->frame++;
		}
		else if (p->direct == -1)
		{
			if (p->frame > 1)
				p->frame--;
		}
		else if (p->direct == 1)
		{
			if (p->frame < 21)
				p->frame++;
		}
		return interval;
	}
	else
	{
		return 0;
	}
}
void RacingCar::startTimer(Uint32 t)
{
	time = t;
	timerID = SDL_AddTimer(time, changeData, this); // Set Timer callback

	chargeTimer = SDL_AddTimer(CHARGE_INTERVAL, charge, this);
}

void RacingCar::stopTimer()
{
	time = 0;
}
void RacingCar::turn(int d)
{
	direct = d;
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

void RacingCar::rush(bool r) {
	if (r) {
		isRushing = true;
		fullEnergy = false;
		energy = 0;
	}
	else {
		isRushing = false;
	}
}




//previous code
/*RacingCar::RacingCar() {}

RacingCar::RacingCar(Map* _map, SDL_Renderer* renderer):state(0),direct(FRONT),healthPoint(MAX_HP),
      fullyDamaged(false),energyPoint(MAX_ENERGY),fullyCharged(true),rechargeInterval(0),map(_map)
{
	
	//for (int i = 0; i < NUM_CARIMG; ++i) {
		setImage(RACINGCAR_PATH, NUM_CARIMG, renderer, 0);
	//}
	cout << "[RacingCar] Car initialized" << endl;

}

RacingCar::~RacingCar() {}


void RacingCar::quit() {
	removeTimer();
	closeImg();
	cout << "[RacingCar] Car closed" << endl;
}

bool RacingCar::isHit() {
	//need map
	return 1;
}

void RacingCar::HitObstacle() {
	acc = 0;
	velX = BOUNCE_SPEED;
	// HP damage 

}

void RacingCar::stopTimer() {

}

void RacingCar::turn(int d) {
	if (direct == FRONT) {
		if (d == 1)
			direct = RIGHT_FRONT, state = 1;
		else if (d == -1)
			direct = LEFT_FRONT, state = 2;
	}
	else if (direct == BACK) {
		if (d == 1)
			direct = RIGHT_BACK, state = 1;
		else if (d == -1)
			direct = LEFT_BACK, state = 2;
	}
	else if (d == 0) {
		direct = FRONT, state = 0;
	}
}

void RacingCar::startTimer(Uint32 interval) {
	rechargeTimer = SDL_AddTimer(rechargeInterval, recharge, this);
}

void RacingCar::removeTimer() {
	SDL_RemoveTimer(rechargeTimer);
}

void RacingCar::accelerate() {

}

void RacingCar::draw(SDL_Renderer* renderer) {
	drawImg(renderer, { ALL_REGION },
		{WIDTH/2- getCurrentState().getWidth() / 2,HEIGHT - getCurrentState().getHeight(),getCurrentState().getWidth() ,getCurrentState().getHeight() });
}

Uint32 RacingCar::recharge(Uint32 interval, void* para) {
	if (interval == 0)
		return 0;
	RacingCar* rc = (RacingCar*)para;
	if (rc->energyPoint == MAX_ENERGY) {
		rc->rechargeInterval = 0;
		rc->fullyCharged = true;
		return rc->rechargeInterval;
	}
	rc->energyPoint += RATE_RECHARGE;
	return rc->rechargeInterval;
}



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
*/
