#pragma once
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "Image.h"
#include "constants.h"
#include "Line.h"
#include "Trap.h"

class Map;

struct Motion {
	double posX;			// x position in 3D world (moving forward and backword)
	double posY;			// y position in 3D world (moving right and left)
	double velLinear;		// linear velocity 
	double velAngular;		// angular velocity 
	double accLinear;		// linear acceleration 
	double camDegree;		// camera degree (in radius, x-axis is 0)
	double roadDegree;		// the degree from x-axis to the road vector (from current segment to the next segment) 
	double camDepth;
	double velM;
	double roadMod;
	double camHeight;
};

class RacingCar
{
	Motion motion;

	char path[100];
	int num;  // New in this example
	Image* image;
	int frame;
	int x;
	int y;
	SDL_TimerID cartimer;
	Uint32 time;
	static Uint32 changeData(Uint32 interval, void* param); // Timer callback 
	int direct;

	//timer
	SDL_TimerID chargeTimer;
	static Uint32 charge(Uint32, void*);

	double healthPoint;
	double energy;
	//state
	RushType isRushing;
	bool fullEnergy;
	bool outOfRoad;

	//trap
	Trap virus;

	//road type
	RoadType roadtype;

public:
	RacingCar();
	RacingCar(const char* path, int n, SDL_Renderer* renderer);
	~RacingCar();
	void quit();
	void setPosition(int xx, int yy);
	int getWidth();
	int getHeight();
	void draw(SDL_Renderer* renderer);
	void startTimer(Uint32 t);
	void stopTimer();
	void turn(int); // move the object

	void rush(RushType);
	RushType getRushing() { return isRushing; }
	bool getFullEnergy() { return fullEnergy; }
	double getEnergy() { return energy; }


	double getPosX() { return motion.posX; }
	double getPosY() { return motion.posY; }
	double getVelLinear() { return motion.velLinear; }
	double getVelAngular() { return motion.velAngular; }
	double getAccLinear() { return motion.accLinear; }
	double getRoadMod() { return motion.roadMod; }
	Trap* getTrap() { return &virus; }
	bool isOutofRoad() { return outOfRoad; }

	const Motion& getMotioin() { return motion; }

	void brake(int = -1);
	void setRoadType(RoadType);

	void setTrap(Line* l) { virus.setTrap(l); }
	void setOutofRoad(bool o) { outOfRoad = o; }

	void setPosX(double x) { motion.posX = x; }
	void setPosY(double y) { motion.posY = y; }
	void setVelLinear(double v) { motion.velLinear = v; }
	void setVelAngular(double v) { motion.velAngular = v; }
	//void setAccLinear(double a) { motion.accLinear = a; }
	void setCamDegree(double cd) { motion.camDegree = cd;  }  //camera degree
	void setCamDepth(double cdp) { motion.camDepth = cdp; }  //camera depth
	void setRoadDegree(double rd) { motion.roadDegree = rd; }
	void setVelM(double vm) { motion.velM = vm; }
	void setRoadMod(double rm) { motion.roadMod = rm; }

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
