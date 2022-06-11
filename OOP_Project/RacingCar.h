#pragma once
#include <stdlib.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "Tool.h"
#include "constants.h"
#include "Line.h"
#include "Trap.h"
#include "Obstacle.h"
#include "BlenderObject.h"
#include "Text.h"

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
	double Xangle;			// rotation about x-axis(Y-axis)

	double velM;			// velocity modification

	double camDepth;		// camera depth
	double roadMod;			// road modification (for friction to change gradually between different roadTypes)
	double camHeight;		// camera height from the road
	double baseHeight;		//only used when in air
	
};

class RacingCar: public BlenderObject
{
	Motion motion;
	double healthPoint;
	double energy;
	RacingCar* theOtherCar;

	//timer
	SDL_TimerID cartimer;
	static Uint32 changeData(Uint32 interval, void* param); // Timer callback 

	//state
	RushType isRushing;
	bool fullEnergy;
	bool outOfRoad;
	int invincible;
	int accState;
	bool inAir;

	//map object

	//road type
	unsigned long long roadtype;
	Line* currentPos;

	//timing
	bool arrive;
	Uint64 starttime;
	Uint64 totaltime;
	char timing[100];
	Text timetext;

public:

	RacingCar();
	RacingCar(const char* objpath, const char* imgpath,SDL_Renderer* renderer, Line*);
	~RacingCar();

	void quit();

	//draw
	void draw3D(Point3D campos, double camDeg, double camDepth, Engine* engine, bool& clean, int ind, double maxy = HEIGHT) override;
	void draw(SDL_Renderer* renderer, Engine* engine, bool& clean);
	void drawOtherCar(SDL_Renderer* renderer, Engine* engine, bool& clean, double maxy, double camH);

	//timer
	void startTimer(Uint32 t);
	bool collided();
	void usetool(ToolType type, Tool* tools, bool car);

	//getter
	RacingCar* getOtherCar() { return theOtherCar; }
	RushType getRushing() { return isRushing; }
	bool getFullEnergy() { return fullEnergy; }
	double getEnergy() { return energy; }
	double getHP() { return healthPoint; }
	double getAxleDegree() { return motion.axleDegree; }
	double getPosX() { return motion.posX; }
	double getPosY() { return motion.posY; }
	double getVelLinear() { return motion.velLinear; }
	double getVelAngular() { return motion.velAngular; }
	double getAccLinear() { return motion.accLinear; }
	double getRoadMod() { return motion.roadMod; }
	bool isOutofRoad() { return outOfRoad; }
	bool isInAir() { return inAir; }
	Line* getCurrentPos() { return currentPos; }
	double getCamHeight() { return motion.camHeight; }
	double getVelPerpen() { return motion.velPerpen; }
	const Motion& getMotion() { return motion; }
	Uint64 gettotaltime() { return totaltime; }

	//setter
	void setOtherCar(RacingCar* c) { theOtherCar = c; }
	void setXangle(double xd) { motion.Xangle = xd; }
	void setCurrentPos(Line* l) { currentPos = l; }
	void setAxleDegree(double ad) { motion.axleDegree = ad; }
	void setCamHeight(double ch) { motion.camHeight = ch; }
	void setVelPerpen(double vp) { motion.velPerpen = vp; }
	void setInAir(bool ia, double baseheight = 0);
	void setFrictionType(unsigned long long);
	void setOutofRoad(bool o) { outOfRoad = o; }
	void setPosX(double x) { motion.posX = x; }
	void setPosY(double y) { motion.posY = y; }
	void setVelLinear(double v) { motion.velLinear = v; }
	void setVelAngular(double v) { motion.velAngular = v; }
	void setAccLinear(double a) { motion.accLinear = a; }
	void setCamDegree(double cd) { motion.camDegree = cd; }  //camera degree
	void setCamDepth(double cdp) { motion.camDepth = cdp; }  //camera depth
	void setRoadDegree(double rd) { motion.roadDegree = rd; }
	void setVelM(double vm) { motion.velM = vm; }
	void setRoadMod(double rm) { motion.roadMod = rm; }


	void rush(RushType rushtype);
	void brake(int type = -1);		//car accelerating
	void touchobstacle(Obstacle& rock);
	void isarrive();
};
