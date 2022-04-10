#include "RacingCar.h"
RacingCar::RacingCar() {

}
RacingCar::RacingCar(const char* path, int n, SDL_Renderer* renderer)
{
	num = n;
	image = new Image[num];

	for (int i = 0; i < num; i++)
	{
		char file[100];
		sprintf_s(file, 100, "%s%02d.png", path, i + 1);

		image[i].set(file, renderer);
	}

	frame = 0;
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
	SDL_Rect d, s;
	d.x = x;
	d.y = y;
	d.w = image[frame].getWidth();
	d.h = image[frame].getHeight();

	image[frame].draw(renderer, { NULL }, &d);
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
}

void RacingCar::stopTimer()
{
	time = 0;
}
void RacingCar::turn(int d)
{
	direct = d;
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
