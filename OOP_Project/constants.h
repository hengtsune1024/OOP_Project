#pragma once

//window
#define WIDTH 800				//window width and height
#define HEIGHT 600
#define TITLE "OOP Project"		//window title

//image
#define ALL_REGION -1	//when drawing image, pass {ALL_REGION} into SDL_Rect src to draw the whole image 
						//or into SDL_Rect dst to fill image into the whole window 

//racing car
#define MOTION_INIT {INITIAL_POS* SEGMENT_LENGTH,0,0,0,0,0,0,DEFAULT_CAMERA_DEPTH,1,CAMERA_HEIGHT}

//map
#define NUM_LINE 5000		//
#define INITIAL_POS 30
#define FINAL_POS 3000

//camera
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

//speed
#define MAX_FORWARD_SPEED 1800		//max car forward moving speed
#define MAX_BACKWARD_SPEED 1000		//max car backward moving speed
#define AFTERRUSH_SPEED_DECREASE 3

//rush
#define ENERGY_RUSHBEGIN_SPEED 2200
#define ACCROAD_RUSHBEGIN_SPEED 2000

//acceleration
#define ACCELERATION 150		//
#define FRICTION_ACC 20

//rotation
#define ROTATE 0.04			//car camera rotation
#define MAX_ROTATE_DEGREE 0.52  // 30 degree

//road
#define ROAD_WIDTH 4000
#define SEGMENT_LENGTH 400
#define ACCROAD_LENGHT 20
#define ROAD_BORDER (2.5 * ROAD_WIDTH)
#define LANELINE_WIDTH 150
#define TRAP_WIDTH 1000

//energy
#define ENERGY_RECOVER 0.2

//road type (used in Line class)
enum RoadType {
	NORMAL, ENDPOINT, TOOLAREA, ACCELERATE_RIGHT, ACCELERATE_LEFT, TRAPAREA
};
enum RushType {
	NONE, ENERGY, ACCROAD, TOOL
};
enum TrapType {
	STAIN, DIZZY, LOST
};
enum ToolType {
	ACCELERATE_SHORT, ACCELERATE_LONG, INVINCIBLE
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