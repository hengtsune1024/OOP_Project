#pragma once
#include <SDL.h>
#include "Image.h"
#include "Map.h"
#include "constants.h"
#include "Entity.h"
class Map;

class RacingCar :public Entity
{
	
	int state;	//0: front, 1: right_front, 2:left_front

	//int acc;	//acceleration
	int direct;

	int healthPoint;
	bool fullyDamaged;
	int energyPoint;
	bool fullyCharged;

	Map* map;

	SDL_TimerID rechargeTimer;
	Uint32 rechargeInterval;
	static Uint32 recharge(Uint32, void*);

	void stopTimer();
	void removeTimer();
	void HitObstacle();

public:
	RacingCar();
	RacingCar(Map*, SDL_Renderer*);
	~RacingCar();

	//int getAcc() { return acc; }
	bool getFullyDamage() { return fullyDamaged; }
	Map* getMap() { return map; }

	//void setAcc(int a) { acc = a; }
	void turn(int);

	//void init(Map&, SDL_Renderer*);
	void quit();
	void startTimer(Uint32);

	void accelerate();
	bool isHit();
	
	void draw(SDL_Renderer*);
};
