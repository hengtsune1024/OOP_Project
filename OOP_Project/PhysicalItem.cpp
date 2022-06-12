#include "PhysicalItem.h"


PhysicalItem::PhysicalItem(const char* objfile, const char* texfile, vector<Line>* l, double scale) : BlenderObject(objfile, texfile, scale, NUM_PHYSICALITEM, 2) ,
	move(NUM_PHYSICALITEM, {0,0,0,false}), lines(l)
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
	objectList[ind].rotation = { 0,2 * PI * rand() / (RAND_MAX + 1.0),0 };
	objectList[ind].index = lineindex;
	objectList[ind].texindex = rand() & 1;
}

void PhysicalItem::logic(void* para1, void* para2)
{
	vector<Line>* lines = (vector<Line>*)para1;
	Obstacle* obst = (Obstacle*)para2;
	double cos_, sin_, dx, dz, v1, v2, v, e = 0.6;
	for (int i = 0; i < NUM_PHYSICALITEM; ++i) 
	{
		if (!move[i].isMoving)
			continue;

		//position
		cos_ = cos(move[i].moveDegree);
		sin_ = sin(move[i].moveDegree);
		objectList[i].position.x += move[i].moveVel * sin_;
		objectList[i].position.z += move[i].moveVel * cos_;
		objectList[i].position.y = lines->at((int)(objectList[i].position.z / SEGMENT_LENGTH)).gety() + CUBE_SIZE;

		//collision with obstacle
		objectList[i].index = objectList[i].position.z / SEGMENT_LENGTH;
		if ((lines->at(objectList[i].index).getType() & OBSTACLEAREA)
			&& obst->hitObstacle(objectList[i].position.x, objectList[i].position.y + CUBE_SIZE, obst->getNearestObstacle(objectList[i].index)))
		{
			objectList[i].position.x -= move[i].moveVel * sin_;
			objectList[i].position.z -= move[i].moveVel * cos_;
			move[i].moveVel = 0;
			move[i].moveDegree = 0;
			move[i].angularVel = 0;
			move[i].isMoving = false;
		}
		else 
		{
			bool collided = false;
			for (int j = 0; j < NUM_PHYSICALITEM; ++j)
			{
				if (i == j)
					continue;
				collided = false;
				if (objectList[j].index > objectList[i].index - 15 && objectList[j].index < objectList[i].index + 15) 
				{
					cout << "  1111111\n" ;
					dx = objectList[j].position.x - objectList[i].position.x;
					dz = objectList[j].position.z - objectList[i].position.z;
					if (dx * dx + dz * dz < 8 * CUBE_SIZE * CUBE_SIZE) 
					{
						cout << "  2222222\n";
						if (dx * dx + dz * dz < 4 * CUBE_SIZE * CUBE_SIZE) {
							cout << "  444444\n";
							collided = true;
						}
						else {
							cout << "  333333\n";
							cos_ = cos(move[j].moveDegree - move[i].moveDegree);
							sin_ = sin(move[j].moveDegree - move[i].moveDegree);
							double rz[4] = { CUBE_SIZE * cos_ - CUBE_SIZE * sin_ - dz,CUBE_SIZE * cos_ + CUBE_SIZE * sin_ - dz ,
													-CUBE_SIZE * cos_ - CUBE_SIZE * sin_ - dz ,-CUBE_SIZE * cos_ + CUBE_SIZE * sin_ - dz };
							double rx[4] = { CUBE_SIZE * sin_ + CUBE_SIZE * cos_ - dx,CUBE_SIZE * sin_ - CUBE_SIZE * cos_ - dx,
											-CUBE_SIZE * sin_ + CUBE_SIZE * cos_ - dx,-CUBE_SIZE * sin_ - CUBE_SIZE * cos_ - dx };
							for (int k = 0; k < 4; ++k) 
								if (rz[k] < CUBE_SIZE && rz[k] > -CUBE_SIZE && rx[k] < CUBE_SIZE && rx[k] > -CUBE_SIZE) 
								{
									cout << "  555555\n";
									collided = true;
									break;
								}
						}

						if (collided) 
						{
							cos_ = cos(move[j].moveDegree - move[i].moveDegree);
							v1 = move[i].moveVel, v2 = move[j].moveVel * cos_;
							v = ((1 - e) * v1 + (1 + e) * v2) / 2.0;

							v1 = move[i].moveVel * cos_, v2 = move[j].moveVel;
							move[i].moveVel = v;
							move[j].moveVel = ((1 - e) * v2 + (1 + e) * v1) / 2.0;
							move[j].isMoving = true;
							move[j].moveDegree = move[i].moveDegree;
						}
					}
				}
			}
			//velocity
			move[i].moveVel -= ITEM_FRICTION;
			if (move[i].moveVel < 0) {
				move[i].moveVel = 0;
				move[i].moveDegree = 0;
				move[i].isMoving = false;
			}

			//y-rotate
			objectList[i].rotation.y += move[i].angularVel;
			if (move[i].angularVel > 1e-6) {
				move[i].angularVel -= 0.02;
				if (move[i].angularVel < 0)
					move[i].angularVel = 0;
			}
			else if (move[i].angularVel < -1e-6) {
				move[i].angularVel += 0.02;
				if (move[i].angularVel > 0)
					move[i].angularVel = 0;
			}
			if (objectList[i].rotation.y > 2 * PI)
				objectList[i].rotation.y -= 2 * PI;
			else if (objectList[i].rotation.y < -2 * PI)
				objectList[i].rotation.y += 2 * PI;
		}

		//index
		objectList[i].index = objectList[i].position.z / SEGMENT_LENGTH;
	}
}

bool PhysicalItem::collide(RacingCar* car) 
{
	double dx, dz, rd, cos_, sin_, height;
	bool collision = false;
	const Motion& motion = car->getMotion();
	for (int j = 0; j < NUM_PHYSICALITEM; ++j) 
	{
		height = car->isInAir() ? motion.camHeight + motion.baseHeight : motion.camHeight + car->getCurrentPos()->gety();
		if (height - CAMERA_HEIGHT > objectList[j].position.y + CUBE_SIZE)
			continue;

		dx = motion.posY + CAMERA_CARMIDPOINT_DIST * sin(motion.axleDegree) - objectList[j].position.x;
		dz = motion.posX + CAMERA_CARMIDPOINT_DIST * cos(motion.axleDegree) - objectList[j].position.z;

		if (dx * dx + dz * dz < ((CUBE_SIZE + CAR_HALF_LENGTH) * (CUBE_SIZE + CAR_HALF_LENGTH) + (CUBE_SIZE + CAR_HALF_WIDTH) * (CUBE_SIZE + CAR_HALF_WIDTH)) * 0.9) {
			rd = motion.axleDegree - objectList[j].rotation.y;
			cos_ = cos(rd), sin_ = sin(rd);
			double rz[5] = { CAR_HALF_LENGTH * cos_ - CAR_HALF_WIDTH * sin_ - dz,CAR_HALF_LENGTH * cos_ + CAR_HALF_WIDTH * sin_ - dz ,
							-CAR_HALF_LENGTH * cos_ - CAR_HALF_WIDTH * sin_ - dz ,-CAR_HALF_LENGTH * cos_ + CAR_HALF_WIDTH * sin_ - dz, CAR_HALF_LENGTH * cos_ - dz };
			double rx[5] = { CAR_HALF_LENGTH * sin_ + CAR_HALF_WIDTH * cos_ - dx,CAR_HALF_LENGTH * sin_ - CAR_HALF_WIDTH * cos_ - dx,
							-CAR_HALF_LENGTH * sin_ + CAR_HALF_WIDTH * cos_ - dx,-CAR_HALF_LENGTH * sin_ - CAR_HALF_WIDTH * cos_ - dx,  CAR_HALF_LENGTH  * sin_ - dx };
			for (int i = 0; i < 5; ++i) {
				if (rz[i] < CUBE_SIZE && rz[i] > -CUBE_SIZE && rx[i] < CUBE_SIZE && rx[i] > -CUBE_SIZE) {
					//collided
					collision = true;
					move[j].isMoving = true;
					move[j].moveDegree = motion.axleDegree;
					move[j].moveVel = motion.velLinear * 1.2 * motion.velM;
					move[j].angularVel = ((0.5 * move[j].moveVel / ENERGY_RUSHBEGIN_SPEED) * rand() / (RAND_MAX + 1.0)) * ((rand() & 1) ? 1 : -1);
					break;
				}
			}
		}
	}
	return collision;
}