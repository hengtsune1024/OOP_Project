#pragma once
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "Tool.h"
#include "Image.h"
#include "constants.h"
#include "Line.h"
#include "Trap.h"
#include "Obstacle.h"
#include "BlenderObject.h"

class Map;

struct Motion {
	double posX;			// x position in 3D world (moving forward and backword)
	double posY;			// y position in 3D world (moving right and left)
	double velLinear;		// total xy linear velocity 
	double velPerpen;		// z-direction velocity (moving up and down)
	double velAngular;		// angular velocity on xy-plane
	double accLinear;		// linear acceleration on xy-plane
	double axleDegree;		// angle between car-direction and z-axis(X-axis)
	double camDegree;		// camera degree (in rad, z-axis(X-axis) is 0)
	double roadDegree;		// the degree from z-axis(X-axis) to the road vector (from current segment to the next segment) 
	double camDepth;		// camera depth
	double velM;			// velocity modification
	double roadMod;			// road modification (for friction to change gradually between different roadTypes)
	double camHeight;		// camera height from the road
};

class RacingCar
{
	Motion motion;
	/*
	char path[100];
	int num;  // New in this example
	Image* image;
	int frame;
	int x;
	int y;
	*/
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
	bool invincible;
	int accState;
	bool inAir;

	//trap
	Trap virus;

	//tool
	Tool tools;

	Obstacle rock;

	//road type
	unsigned long long roadtype;
	Line* currentPos;

	BlenderObject car3D;
	RacingCar* theOtherCar;

public:

	double baseHeight; //only used when in air
	RacingCar();
	RacingCar(const char* path, int n, SDL_Renderer* renderer, Line*);
	~RacingCar();
	void quit();
	//void setPosition(int xx, int yy);
	//int getWidth();
	//int getHeight();
	void draw(SDL_Renderer* renderer, Engine* engine, bool clean);
	void drawOtherCar(SDL_Renderer* renderer, Engine* engine, bool clean, double maxy, double camH);
	void startTimer(Uint32 t);
	void stopTimer();
	void turn(int); // move the object

	void usetool(ToolType type);

	void setOtherCar(RacingCar* c) { theOtherCar = c; }
	RacingCar* getOtherCar() { return theOtherCar; }

	void rush(RushType);
	RushType getRushing() { return isRushing; }
	bool getFullEnergy() { return fullEnergy; }
	double getEnergy() { return energy; }

	BlenderObject* getCar3D() { return &car3D; }
	double getAxleDegree() { return motion.axleDegree; }
	void setAxleDegree(double ad) { motion.axleDegree = ad; }

	double getPosX() { return motion.posX; }
	double getPosY() { return motion.posY; }
	double getVelLinear() { return motion.velLinear; }
	double getVelAngular() { return motion.velAngular; }
	double getAccLinear() { return motion.accLinear; }
	double getRoadMod() { return motion.roadMod; }
	Trap* getTrap() { return &virus; }
	Tool* getTools() { return &tools; }
	Obstacle* getObstacle() { return &rock; }
	bool isOutofRoad() { return outOfRoad; }
	bool isInAir() { return inAir; }

	Line* getCurrentPos() { return currentPos; }
	void setCurrentPos(Line* l) { currentPos = l; }

	double getCamHeight() { return motion.camHeight; }
	void setCamHeight(double ch) { motion.camHeight = ch; }

	double getVelPerpen() { return motion.velPerpen; }
	void setVelPerpen(double vp) { motion.velPerpen = vp; }

	void setInAir(bool ia) { inAir = ia; }

	const Motion& getMotion() { return motion; }

	void brake(int = -1);		//car accelerating
	void setRoadType(unsigned long long);

	void setTrap(Line* l) { virus.setTrap(l); }
	void setTool(Line* l) { tools.setTool(l); }
	void setObstacle(Line* l) { rock.setObstacle(l); }
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

	void touchobstacle();

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
