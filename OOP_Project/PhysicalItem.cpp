#include "PhysicalItem.h"


PhysicalItem::PhysicalItem(const char* objfile, const char* texfile, vector<Line>* l, double scale) : obj3D(objfile, texfile, scale),
	moveDegree(0), moveVel(0), isMoving(false), lines(l)
{
}
PhysicalItem::~PhysicalItem() {
	SDL_RemoveTimer(timer);
}
void PhysicalItem::drawObj3D(Point3D pos, double camDeg, double camDepth, Engine* engine, bool clean, double maxy) {
	obj3D.draw(pos, obj3D.getRotation(), camDeg, camDepth, engine, clean, maxy);
}
void PhysicalItem::startTimer() {
	timer = SDL_AddTimer(50, move, this);
}
void PhysicalItem::setMoving(bool m) {
	if (!isMoving && m) {
		startTimer();
	}
	isMoving = m;
}
Uint32 PhysicalItem::move(Uint32 interval, void* para)
{
	PhysicalItem* it = (PhysicalItem*)para;
	//position
	Point3D pos = it->obj3D.getPosition();
	pos.x += it->moveVel * sin(it->moveDegree);
	pos.z += it->moveVel * cos(it->moveDegree);
	pos.y = it->lines->at((int)(pos.z / SEGMENT_LENGTH)).gety() + CUBE_SIZE;
	it->obj3D.setPos(pos);

	//velocity
	it->moveVel -= ITEM_FRICTION;
	if (it->moveVel < 0) {
		it->moveVel = 0;
		it->moveDegree = 0;
		it->isMoving = false;
		return 0;
	}

	//rotate
	double d = it->obj3D.getRotY();
	d += 0.02;
	if (d > 6.2832)
		d -= 6.2832;
	it->obj3D.setRotY(d);

	return interval;
}

void PhysicalItem::collide(RacingCar* car) 
{
	Point3D pos = obj3D.getPosition();
	Point3D rot = obj3D.getRotation();

	double dx = car->getPosY() + CAMERA_CARMIDPOINT_DIST * sin(car->getAxleDegree()) - pos.x;
	double dz = car->getPosX() + CAMERA_CARMIDPOINT_DIST * cos(car->getAxleDegree()) - pos.z;
	
	if (dx * dx + dz * dz < ((CUBE_SIZE + CAR_HALF_LENGTH) * (CUBE_SIZE + CAR_HALF_LENGTH) + (CUBE_SIZE + CAR_HALF_WIDTH) * (CUBE_SIZE + CAR_HALF_WIDTH)) * 0.9) {
		double rd = car->getAxleDegree() - rot.y;
		double cos_ = cos(rd), sin_ = sin(rd);
		double rz[4] = { CAR_HALF_LENGTH * cos_ - CAR_HALF_WIDTH * sin_ - dz,CAR_HALF_LENGTH * cos_ + CAR_HALF_WIDTH * sin_ - dz ,
						-CAR_HALF_LENGTH * cos_ - CAR_HALF_WIDTH * sin_ - dz ,-CAR_HALF_LENGTH * cos_ + CAR_HALF_WIDTH * sin_ - dz };
		double rx[4] = { CAR_HALF_LENGTH * sin_ + CAR_HALF_WIDTH * cos_ - dx,CAR_HALF_LENGTH * sin_ - CAR_HALF_WIDTH * cos_ - dx,
						-CAR_HALF_LENGTH * sin_ + CAR_HALF_WIDTH * cos_ - dx,-CAR_HALF_LENGTH * sin_ - CAR_HALF_WIDTH * cos_ - dx };
		for (int i = 0; i < 4; ++i) {
			if (rz[i] < CUBE_SIZE && rz[i] > -CUBE_SIZE && rx[i] < CUBE_SIZE && rx[i] > -CUBE_SIZE) {
				//collided, 
				double e = 0.6;
				isMoving = true;
				moveDegree = car->getAxleDegree();
				moveVel = car->getMotion().velLinear * 1.2;
				startTimer();
				break;
			}
		}
	}
}