#include "PhysicalItem.h"


PhysicalItem::PhysicalItem(const char* objfile, const char* texfile, vector<Line>* l, double scale) : BlenderObject(objfile, texfile, scale, NUM_PHYSICALITEM),
	move(NUM_PHYSICALITEM, {0,0,false}), lines(l)
{}

PhysicalItem::~PhysicalItem() 
{
	lines = NULL;
}

void PhysicalItem::close() {
	BlenderObject::close();
}

void PhysicalItem::draw3D(Point3D pos, double camDeg, double camDepth, Engine* engine, bool& clean, int ind, double maxy) {
	BlenderObject_draw(pos, objectList[ind].rotation, camDeg, camDepth, engine, clean, maxy, ind);
	clean = false;
}

void PhysicalItem::setItem(Line* line, int lineindex, int ind) 
{
	double shift = ROAD_WIDTH * 1.5 * 2 * (rand() / (RAND_MAX + 1.0)) - ROAD_WIDTH * 1.5;
	objectList[ind].position = { line->getx() + shift , line->gety() + CUBE_SIZE ,line->getz(),0,0,0 };
	objectList[ind].index = lineindex;
}

void PhysicalItem::logic()
{
	for (int i = 0; i < NUM_PHYSICALITEM; ++i) 
	{
		if (!move[i].isMoving)
			continue;

		//position
		objectList[i].position.x += move[i].moveVel * sin(move[i].moveDegree);
		objectList[i].position.z += move[i].moveVel * cos(move[i].moveDegree);
		objectList[i].position.y = lines->at((int)(objectList[i].position.z / SEGMENT_LENGTH)).gety() + CUBE_SIZE;

		//rotate
		objectList[i].rotation.y += 0.04;
		if (objectList[i].rotation.y > 2 * PI)
			objectList[i].rotation.y -= 2 * PI;

		//velocity
		move[i].moveVel -= ITEM_FRICTION;
		if (move[i].moveVel < 0) {
			move[i].moveVel = 0;
			move[i].moveDegree = 0;
			move[i].isMoving = false;
		}

		//index
		objectList[i].index = objectList[i].position.z / SEGMENT_LENGTH;
	}
}

void PhysicalItem::collide(RacingCar* car) 
{
	double dx, dz, rd, cos_, sin_;

	for (int j = 0; j < NUM_PHYSICALITEM; ++j) 
	{
		dx = car->getPosY() + CAMERA_CARMIDPOINT_DIST * sin(car->getAxleDegree()) - objectList[j].position.x;
		dz = car->getPosX() + CAMERA_CARMIDPOINT_DIST * cos(car->getAxleDegree()) - objectList[j].position.z;

		if (dx * dx + dz * dz < ((CUBE_SIZE + CAR_HALF_LENGTH) * (CUBE_SIZE + CAR_HALF_LENGTH) + (CUBE_SIZE + CAR_HALF_WIDTH) * (CUBE_SIZE + CAR_HALF_WIDTH)) * 0.9) {
			rd = car->getAxleDegree() - objectList[j].rotation.y;
			cos_ = cos(rd), sin_ = sin(rd);
			double rz[4] = { CAR_HALF_LENGTH * cos_ - CAR_HALF_WIDTH * sin_ - dz,CAR_HALF_LENGTH * cos_ + CAR_HALF_WIDTH * sin_ - dz ,
							-CAR_HALF_LENGTH * cos_ - CAR_HALF_WIDTH * sin_ - dz ,-CAR_HALF_LENGTH * cos_ + CAR_HALF_WIDTH * sin_ - dz };
			double rx[4] = { CAR_HALF_LENGTH * sin_ + CAR_HALF_WIDTH * cos_ - dx,CAR_HALF_LENGTH * sin_ - CAR_HALF_WIDTH * cos_ - dx,
							-CAR_HALF_LENGTH * sin_ + CAR_HALF_WIDTH * cos_ - dx,-CAR_HALF_LENGTH * sin_ - CAR_HALF_WIDTH * cos_ - dx };
			for (int i = 0; i < 4; ++i) {
				if (rz[i] < CUBE_SIZE && rz[i] > -CUBE_SIZE && rx[i] < CUBE_SIZE && rx[i] > -CUBE_SIZE) {
					//collided
					move[j].isMoving = true;
					move[j].moveDegree = car->getAxleDegree();
					move[j].moveVel = car->getMotion().velLinear * 1.2;
					break;
				}
			}
		}
	}
}