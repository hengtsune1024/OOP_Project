#include "RacingCar.h"

RacingCar::RacingCar() :
	isRushing(NONE), fullEnergy(true), energy(100.0), healthPoint(100.0),
	motion(MOTION_INIT), roadtype(NORMAL), outOfRoad(false), inAir(false), BlenderObject("../images/car/car.txt", "../images/car/car", 500, 1, 2)
{}

RacingCar::~RacingCar()
{}

RacingCar::RacingCar(const char* obfpath, const char* imgpath, SDL_Renderer* renderer, Line* initpos) :
	isRushing(NONE), fullEnergy(true), energy(100.0), healthPoint(100.0), motion(MOTION_INIT), accState(0), roadtype(NORMAL), currentPos(initpos),
	theOtherCar(NULL), starttime(SDL_GetTicks64() + 3000), timing("00:00:000"), arrive(false), totaltime(0), invincible(0),
	BlenderObject(obfpath, imgpath, 1000, 1, 3),
	timetext(timing, "../fonts/akabara-cinderella.ttf", 20, 0x02, { 255, 255, 255 }, SHADED, { 0, 0, 0 }, renderer, { 200, 10 }, { 10, 10 }, NULL, SDL_FLIP_NONE, 255),
	dizzy(0), lost(0), slow(0), slowimg("../images/slow.png", renderer),
	dizzyimg("../images/dizzy.png", renderer), lostimg("../images/banana.png", renderer)
{
	lnnum = 6;
	lnimg = new Image[lnnum];
	for (int i = 0; i < lnnum; ++i) {
		char _path[100];
		sprintf_s(_path, "%s%02d.png", "../images/lightning/", i + 1);
		lnimg[i].set(_path, renderer);
	}
	lnstate = 0;
}

void RacingCar::quit()
{
	// Remove timer in case the call back was not called	
	SDL_RemoveTimer(cartimer);
	BlenderObject::close();
	timetext.close();
}

void RacingCar::operator-=(double d) {
	healthPoint -= d;
}
void RacingCar::changeSelect() {

}

bool RacingCar::collided() 
{
	if (theOtherCar == NULL)
		return false;
	//collision
	double height = (inAir ? motion.camHeight + motion.baseHeight : motion.camHeight + currentPos->gety()) 
			- (theOtherCar->inAir ? theOtherCar->motion.camHeight + theOtherCar->motion.baseHeight : theOtherCar->motion.camHeight + theOtherCar->currentPos->gety());
	if (height > 1277 || height < -1277)
		return false;
	double dx = (motion.posY + CAMERA_CARMIDPOINT_DIST * sin(motion.axleDegree)) - (theOtherCar->motion.posY + CAMERA_CARMIDPOINT_DIST * sin(theOtherCar->motion.axleDegree));
	double dz = (motion.posX + CAMERA_CARMIDPOINT_DIST * cos(motion.axleDegree)) - (theOtherCar->motion.posX + CAMERA_CARMIDPOINT_DIST * cos(theOtherCar->motion.axleDegree));

	if (dx * dx + dz * dz < 4.0 * (CAR_HALF_LENGTH * CAR_HALF_LENGTH + CAR_HALF_WIDTH * CAR_HALF_WIDTH) * 0.9)
	{
		double rd = theOtherCar->motion.axleDegree - motion.axleDegree;
		double cos_ = cos(rd), sin_ = sin(rd);//CAR_HALF_LENGTHcos_  CAR_HALF_WIDTH
		{
			double rz[5] = { CAR_HALF_LENGTH * cos_ - CAR_HALF_WIDTH * sin_ - dz,CAR_HALF_LENGTH * cos_ + CAR_HALF_WIDTH * sin_ - dz ,
							-CAR_HALF_LENGTH * cos_ - CAR_HALF_WIDTH * sin_ - dz ,-CAR_HALF_LENGTH * cos_ + CAR_HALF_WIDTH * sin_ - dz, CAR_HALF_LENGTH * cos_ - dz };
			double rx[5] = { CAR_HALF_LENGTH * sin_ + CAR_HALF_WIDTH * cos_ - dx,CAR_HALF_LENGTH * sin_ - CAR_HALF_WIDTH * cos_ - dx,
							-CAR_HALF_LENGTH * sin_ + CAR_HALF_WIDTH * cos_ - dx,-CAR_HALF_LENGTH * sin_ - CAR_HALF_WIDTH * cos_ - dx,  CAR_HALF_LENGTH* sin_ - dx };
			for (int i = 0; i < 5; ++i) {
				if (rz[i] < CAR_HALF_LENGTH && rz[i] > -CAR_HALF_LENGTH && rx[i] < CAR_HALF_WIDTH && rx[i] > -CAR_HALF_WIDTH) {
					//collided
					return true;
				}
			}
		}
		rd = -rd;
		sin_ = -sin_;
		double rz[5] = { CAR_HALF_LENGTH * cos_ - CAR_HALF_WIDTH * sin_ - dz,CAR_HALF_LENGTH * cos_ + CAR_HALF_WIDTH * sin_ - dz ,
						-CAR_HALF_LENGTH * cos_ - CAR_HALF_WIDTH * sin_ - dz ,-CAR_HALF_LENGTH * cos_ + CAR_HALF_WIDTH * sin_ - dz, CAR_HALF_LENGTH * cos_ - dz };
		double rx[5] = { CAR_HALF_LENGTH * sin_ + CAR_HALF_WIDTH * cos_ - dx,CAR_HALF_LENGTH * sin_ - CAR_HALF_WIDTH * cos_ - dx,
						-CAR_HALF_LENGTH * sin_ + CAR_HALF_WIDTH * cos_ - dx,-CAR_HALF_LENGTH * sin_ - CAR_HALF_WIDTH * cos_ - dx , CAR_HALF_LENGTH* sin_ - dx };

		for (int i = 0; i < 5; ++i) {
			if (rz[i] < CAR_HALF_LENGTH && rz[i] > -CAR_HALF_LENGTH && rx[i] < CAR_HALF_WIDTH && rx[i] > -CAR_HALF_WIDTH) {
				//collided, 
				return true;
			}
		}
	}
	return false;
}

void RacingCar::draw3D(Point3D campos, double camDeg, double camDepth, Engine* engine, bool& clean, int ind, double maxy) 
{
	BlenderObject_draw(campos, { -motion.Xangle,objectList[0].rotation.y,0 }, 0, motion.camDepth, engine, clean, maxy, 0);
	clean = false;
}

void RacingCar::draw(SDL_Renderer* renderer, Engine* engine, bool& clean)
{
	//car image

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
	/*
	//tool column
	roundedRectangleRGBA(renderer, 350, 10, 385, 45, 1, 255, 0, 255, 255);
	roundedRectangleRGBA(renderer, 390, 10, 425, 45, 1, 255, 0, 255, 255);
	roundedRectangleRGBA(renderer, 430, 10, 465, 45, 1, 255, 0, 255, 255);
	roundedRectangleRGBA(renderer, 470, 10, 505, 45, 1, 255, 0, 255, 255);
	roundedRectangleRGBA(renderer, 510, 10, 545, 45, 1, 255, 0, 255, 255);
	roundedRectangleRGBA(renderer, 550, 10, 585, 45, 1, 255, 0, 255, 255);
	*/

	//state
	SDL_Rect stateloc{ 20, 100, 75, 60 };
	if (dizzy)
		dizzyimg.draw(renderer, NULL, &stateloc);
	if (lost)
		lostimg.draw(renderer, NULL, &stateloc);
	if (slow)
		slowimg.draw(renderer, NULL, &stateloc);

	//lightning
	if (drawln)
		lnimg[lnstate].draw(renderer, NULL, NULL);

	//timing
	timetext.close();
	timetext.setString(timing);
	timetext.generateTexture();
	timetext.draw();
}

void RacingCar::drawOtherCar(SDL_Renderer* renderer, Engine* engine, bool& clean, double maxy, double camH) {
	/*
	car3D.draw(
		{ motion.posY - theOtherCar->getPosY() ,camH - theOtherCar->getCamHeight() - theOtherCar->getCurrentPos()->gety(),motion.posX - theOtherCar->getPosX()},
		{ 0,(theOtherCar->motion.axleDegree - motion.axleDegree),0},
		motion.camDegree, motion.camDepth, engine, clean, maxy);*/

	double height = (inAir ? motion.camHeight + motion.baseHeight - CAMERA_HEIGHT : currentPos->gety()) 
		- (theOtherCar->inAir ? theOtherCar->motion.camHeight + theOtherCar->motion.baseHeight - CAMERA_HEIGHT : theOtherCar->currentPos->gety());

	theOtherCar->BlenderObject_draw( { motion.posY - theOtherCar->getPosY() , height, motion.posX - theOtherCar->getPosX() }, 
		{ -theOtherCar->motion.Xangle,theOtherCar->motion.axleDegree,0 }, motion.camDegree, motion.camDepth, engine, clean, maxy, 0);

	clean = false;
}

Uint32 RacingCar::changeData(Uint32 interval, void* param)
{
	RacingCar* car = (RacingCar*)param;

	//time counting
	if (!car->arrive)
	{
		int totaltime = SDL_GetTicks64() - car->starttime;
		totaltime < 0 ? totaltime = 0 : 1;
		int ms = totaltime % 1000;
		totaltime /= 1000;
		int sec = totaltime % 60;
		totaltime -= sec;
		int min = totaltime / 60;
		sprintf_s(car->timing, "%02d:%02d:%03d", min, sec, ms);
	}

	Uint64 t = SDL_GetTicks64();
	//invincible tool
	if (car->invincible && t - car->invincible >= INVINCIBLE_INTERVAL){
		car->invincible = 0;
		car->objectList[0].texindex = 0;
	}

	//ghost tool
	if (car->ghost && t - car->ghost >= GHOST_INTERVAL){
		car->ghost = 0;
		car->objectList[0].texindex = 0;
	}

	//dizzy
	if (car->dizzy && t - car->dizzy >= DIZZY_INTERVAL)
		car->dizzy = 0;

	if (car->dizzy && car->isRushing)
		car->rush(NONE);

	//lost
	if (car->lost && t - car->lost >= LOST_INTERVAL){
		car->lost = 0;
		car->motion.velAngular = 0;
	}

	//speeddown
	if (car->slow && t - car->slow >= SLOW_INTERVAL)
		car->slow = 0;

	//if (car->slow){
	//	car->motion.velLinear = car->motion.velLinear * 0.7;
	//}

	//slow down with losing HP
	//if (!car->isRushing)
	//	car->motion.velLinear = car->motion.velLinear * (car->healthPoint / 5 + 80) / 100;

	//lightning
	if (car->drawln)
		car->lnstate = (car->lnstate + 1) % car->lnnum;
	if (SDL_GetTicks64() - car->drawln >= 1000)
		car->drawln = 0;

	//car rotation
	double dif = car->motion.axleDegree - car->motion.camDegree;

	if (dif < -1e-6) {
		car->objectList[0].rotation.y -= 0.06;
		if (car->objectList[0].rotation.y < dif)
			car->objectList[0].rotation.y = dif;
	}
	else if (dif > 1e-6) {
		car->objectList[0].rotation.y += 0.06;
		if (car->objectList[0].rotation.y > dif)
			car->objectList[0].rotation.y = dif;
	}

	//charge
	if (!car->fullEnergy) {
		car->energy += ENERGY_RECOVER;
		if (car->energy > 100.0) {
			car->energy = 100.0;
			car->fullEnergy = true;
		}
	}
	return interval;
}

void RacingCar::startTimer(Uint32 t)
{
	cartimer = SDL_AddTimer(50, changeData, this); // Set Timer callback
}

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
				slow = 0;
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
			slow = 0;
			isRushing = r;
			motion.velLinear = ACCROAD_RUSHBEGIN_SPEED;
			motion.camDepth = BEGINRUSH_CAMDEPTH;
			//car.rush(ACCROAD);
			std::cout << "[Map] rush start" << endl;
			break;
		case TOOL:
			slow = 0;
			isRushing = r;
			motion.velLinear = ACCROAD_RUSHBEGIN_SPEED;
			motion.camDepth = BEGINRUSH_CAMDEPTH;
			std::cout << "[Map] rush start" << endl;
			break;
		default:
			break;
	}
}

int RacingCar::usetool(ToolType type, Tool* tools, bool car)
{
	switch (tools->usetool(type, car))
	{
		case SPEEDUP:
			rush(TOOL);
			break;
		case INVINCIBLE:
			invincible = SDL_GetTicks64();
			objectList[0].texindex = 1;
			break;
		case HEALING:
			healthPoint += 30;
			if (healthPoint >= 100)
				healthPoint = 100;
			break;
		case GHOST:
			ghost = SDL_GetTicks64();
			objectList[0].texindex = 2;
			break;
		case LIGHTNING:
			return 1;
			break;
		case SWITCH:
			//tools->getalltools(car);
			return 1;
			break;
	}
	return 0;
}

void RacingCar::gettrap(int type)
{
	switch (type)
	{
		case STAIN:

			break;
		case BANANA:
			lost = SDL_GetTicks64();
			break;
		case SPEEDDOWN:
			slow = SDL_GetTicks64();
			break;
		case BOMB:
			if (!invincible)
			{
				healthPoint -= 25;
				motion.velLinear *= 0.3;
				motion.velAngular = 0;
				brake(0);
				dizzy = SDL_GetTicks64();
				if (isRushing) {
					rush(NONE);
				}

			}
			break;
		case -1:
			return;
		default:;
	}
}
void RacingCar::touchobstacle(Obstacle& rock, int ind, vector<Line>& lines)
{
	if (!rock.istouching(ind))
	{
		//cout << motion.velLinear << endl;

		if (!invincible)
			healthPoint -= motion.velLinear * motion.velLinear / 2 / 100000;
		else if (isRushing) {
			rock.broken(rock.getNearestObstacle(motion.posX / SEGMENT_LENGTH));
			return;
		}
		/*
		if (motion.velLinear > 0) {
			motion.posX -= ROCK_SIZE / 2 * cos(motion.axleDegree);
			motion.posY -= ROCK_SIZE / 2 * sin(motion.axleDegree);
			if (collided()) {
				theOtherCar->motion.posX -= ROCK_SIZE / 2 * cos(motion.axleDegree);
				theOtherCar->motion.posY -= ROCK_SIZE / 2 * sin(motion.axleDegree);
			}
		}
		else if (motion.velLinear < 0) {
			motion.posX += ROCK_SIZE / 2 * cos(motion.axleDegree);
			motion.posY += ROCK_SIZE / 2 * sin(motion.axleDegree);
			if (collided()) {
				theOtherCar->motion.posX += ROCK_SIZE / 2 * cos(motion.axleDegree);
				theOtherCar->motion.posY += ROCK_SIZE / 2 * sin(motion.axleDegree);
			}
		}*/
		/*
		motion.posX -= motion.velLinear * cos(motion.axleDegree) * motion.velM;
		motion.posY -= motion.velLinear * sin(motion.axleDegree) * motion.velM;

		if (collided()) {
			theOtherCar->motion.posX -= motion.velLinear * cos(motion.axleDegree);
			theOtherCar->motion.posY -= motion.velLinear * sin(motion.axleDegree);
		}*/

		motion.velLinear = -motion.velLinear * 0.5;

		if (isRushing) {
			isRushing = NONE;
			motion.velLinear = -MAX_BACKWARD_SPEED * 0.5;
			motion.camDepth = DEFAULT_CAMERA_DEPTH;
		}
		//cout << motion.velLinear << endl;

	}
}

void RacingCar::isarrive() 
{
	if (!arrive)
	{
		arrive = true;
		totaltime= SDL_GetTicks64() - starttime;
		//printf("%d\n", arrivetime);
	}

}

void RacingCar::setInAir(bool ia, double baseheight) {
	inAir = ia;
	if (inAir)
		motion.baseHeight = baseheight;
}

void RacingCar::beattacked()
{
	drawln = SDL_GetTicks64();
	if (!invincible)
	{
		healthPoint -= 20;
		motion.velLinear *= 0.3;
		motion.velAngular = 0;
		brake(0);
		dizzy = SDL_GetTicks64();
		if (isRushing) {
			rush(NONE);
		}
	}

}

//previous code

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