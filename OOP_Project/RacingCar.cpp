#include "RacingCar.h"

RacingCar::RacingCar() {}

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
/*
void RacingCar::HitObstacle() {
	acc = 0;
	velX = BOUNCE_SPEED;
	// HP damage 

}*/

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
*/
