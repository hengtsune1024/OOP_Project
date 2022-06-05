#pragma once

#define CUBE_SIZE 1000
#define ROCK_SIZE 3000

//window
#define WIDTH 600				//window width and height
#define HEIGHT 600
#define TITLE "OOP Project"		//window title

//image
#define ALL_REGION -1	//when drawing image, pass {ALL_REGION} into SDL_Rect src to draw the whole image 
						//or into SDL_Rect dst to fill image into the whole window 

//map
#define NUM_LINE 5000		//
#define INITIAL_POS 30
#define FINAL_POS 4200

//camera
#define FIELD_OF_VIEW 45
#define DEFAULT_CAMERA_DEPTH 0.96
#define CAMERA_HEIGHT 2000
#define BEGINRUSH_CAMDEPTH 0.2
#define AFTERRUSH_CAMDEPTH_RECOVER 0.0075

//timer interval
#define MOVE_INTERVAL 50
#define ACCELERATE_INTERVAL 50
#define CAR_INTERVAL 50
#define CHARGE_INTERVAL 50
#define TRAP_INTERVAL 50
#define STAIN_INTERVAL 3000

//speed
#define MAX_FORWARD_SPEED 1600		//max car forward moving speed
#define MAX_BACKWARD_SPEED 1000		//max car backward moving speed
#define AFTERRUSH_SPEED_DECREASE 3

//rush
#define ENERGY_RUSHBEGIN_SPEED 2000
#define ACCROAD_RUSHBEGIN_SPEED 1800

//acceleration
#define ACCELERATION 150		//
#define FRICTION_ACC 20
#define HIGH_FRICTION_ACC 60
#define LOW_FRICTION_ACC 5
#define INCLINE_ACC 150
#define GRAVITY 50

//rotation
#define ROTATE 0.06			//car camera rotation
#define MAX_ROTATE_DEGREE 0.7854  // 30 degree

//road
#define ROAD_WIDTH 4000
#define SEGMENT_LENGTH 400
#define ACCROAD_LENGHT 20
#define ROAD_BORDER (2.5 * ROAD_WIDTH)
#define LANELINE_WIDTH 150
#define TRAP_WIDTH 1000
#define TOOL_WIDTH 2000

//racing car
#define MOTION_INIT {INITIAL_POS * SEGMENT_LENGTH,0,0,0,0,0,0,0,0,DEFAULT_CAMERA_DEPTH,1,1,CAMERA_HEIGHT}
#define CAR__HALF_LENGTH 2000
#define FRONT_REAR_DISTANCE (0.64 * ROAD_WIDTH)    //wheel width=0.2*CAR_LENGTH=0.32*ROAD_WIDTH

//energy
#define ENERGY_RECOVER 0.2

//road type (used in Line class)
enum RoadType {
	NORMAL = 1, ENDPOINT = 2, ACCELERATE_RIGHT = 4, ACCELERATE_LEFT = 8, TRAPAREA = 16, TOOLAREA = 32, HIGH_FRICTION = 64, LOW_FRICTION = 128,
	INCLINE_FORWARD = 256, INCLINE_BACKWARD = 512, OBSTACLEAREA = 1024, INCLINE_PLANE = 2048
	//INCLINE_BACKWARD: the front one is higher
};

enum RushType {
	NONE, ENERGY, ACCROAD, TOOL
};

enum TrapType {
	STAIN, DIZZY, SPEEDDOWN
};

enum ToolType {
	SPEEDUP,INVINCIBLE, ACCELERATE_SHORT, ACCELERATE_LONG
};
/*

#define BOUNCE_SPEED -5
#define MAX_HP 1000
#define RATE_HEAL 100
#define MAX_ENERGY 1000
#define RATE_RECHARGE 100


enum Direction {
	LEFT_BACK, LEFT, LEFT_FRONT, FRONT, RIGHT_FRONT, RIGHT, RIGHT_BACK, BACK
};
*/