#pragma once
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "Image.h"
#include "constants.h"

class Map;

class RacingCar
{
	char path[100];
	int num;  // New in this example
	Image* image;
	int frame;
	int x;
	int y;
	SDL_TimerID timerID;
	Uint32 time;
	static Uint32 changeData(Uint32 interval, void* param); // Timer callback 
	int direct;
	
	//timer
	SDL_TimerID chargeTimer;
	static Uint32 charge(Uint32, void*);

	double energy;
	//state
	bool isRushing;
	bool fullEnergy;

public:
	RacingCar();
	RacingCar(const char* path, int n, SDL_Renderer* renderer);
	void quit();
	void setPosition(int xx, int yy);
	int getWidth();
	int getHeight();
	void draw(SDL_Renderer* renderer);
	void startTimer(Uint32 t);
	void stopTimer();
	void turn(int); // move the object

	void rush();
	bool getRushing() { return isRushing; }
	bool getFullEnergy() { return fullEnergy; }
	double getEnergy() { return energy; }
	void setRushing(bool r) { isRushing = r; }

};



//previous code
/*class RacingCar :public Entity
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
};*/
