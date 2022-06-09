#include "PhysicalItem.h"


PhysicalItem::PhysicalItem(const char* objfile, const char* texfile, vector<Line>* l, double scale) : obj3D(objfile, texfile, scale),
	moveDegree(0), moveVel(0), isMoving(false), lines(l)
{
}
PhysicalItem::~PhysicalItem() {
}
void PhysicalItem::drawObj3D(Point3D pos, double camDeg, double camDepth, Engine* engine, bool& clean, double maxy) {
	obj3D.draw(pos, obj3D.getRotation(), camDeg, camDepth, engine, clean, maxy);
	clean = false;
}
void PhysicalItem::setMoving(bool m) {
	isMoving = m;
}
void PhysicalItem::logic()
{
	if (!isMoving)
		return;

	//position
	Point3D pos = obj3D.getPosition();
	pos.x += moveVel * sin(moveDegree);
	pos.z += moveVel * cos(moveDegree);
	pos.y = lines->at((int)(pos.z / SEGMENT_LENGTH)).gety() + CUBE_SIZE;
	obj3D.setPos(pos);

	//velocity
	moveVel -= ITEM_FRICTION;
	if (moveVel < 0) {
		moveVel = 0;
		moveDegree = 0;
		isMoving = false;
	}

	//rotate
	double d = obj3D.getRotY();
	d += 0.04;
	if (d > 2 * PI)
		d -= 2 * PI;
	obj3D.setRotY(d);
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
				//collided
				isMoving = true;
				moveDegree = car->getAxleDegree();
				moveVel = car->getMotion().velLinear * 1.2;
				break;
			}
		}
	}
}