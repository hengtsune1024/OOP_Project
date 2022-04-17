#pragma once

//window
#define WIDTH 800				//window width and height
#define HEIGHT 600
#define TITLE "OOP Project"		//window title

//image
#define ALL_REGION -1	//when drawing image, pass {ALL_REGION} into SDL_Rect src to draw the whole image 
						//or into SDL_Rect dst to fill image into the whole window 

//racing car
#define RACINGCAR_PATH "../images/RacingCar/RacingCar"

//map
#define NUM_LINE 5000		//
#define CAMERA_HEIGHT 1500	//
#define INITIAL_POS 30

//timer interval
#define MOVE_INTERVAL 50
#define CAR_INTERVAL 50

//speed
#define MAX_FORWARD_SPEED 1200		//max car forward moving speed
#define MAX_BACKWARD_SPEED 750		//max car backward moving speed

//acceleration
#define ACCELERATION 80		//
#define FRICTION_ACC 40

//rotation
#define ROTATE 0.04			//car camera rotation
#define MAX_ROTATE_DEGREE 0.785  // 45 degree

//road
#define ROAD_WIDTH 2000
#define SEGMENT_LENGTH 400
#define DEFAULT_CAMERA_DEPTH 0.96



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