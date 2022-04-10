#pragma once

//window
#define WIDTH 800			//window width and height
#define HEIGHT 600
#define TITLE "OOP Project"		//window title

//image
#define ALL_REGION -1	

//racing car
#define MAX_SPEED 100  
#define BOUNCE_SPEED -5
#define NUM_CARIMG 3 //number of car images
#define MOVE_INTERVAL 100 //moving timer interval

#define MAX_HP 1000
#define RATE_HEAL 100
#define MAX_ENERGY 1000
#define RATE_RECHARGE 100

#define CAR_ACC 5
#define FRICTION_ACC 2

#define RACINGCAR_PATH "../images/RacingCar/RacingCar"

//map
#define ROAD_WIDTH 2000
#define NUM_LINE 5000
#define CAMERA_HEIGHT 1500
#define MAX_ROTATE_DEGREE 0.785  // 45 degree
#define INITIAL_POS 300

//Line
#define SEGMENT_LENGTH 400
#define CAMERA_DEPTH 0.96

#define SPEED 800
#define ROTATE 0.05


enum Direction {
	LEFT_BACK, LEFT, LEFT_FRONT, FRONT, RIGHT_FRONT, RIGHT, RIGHT_BACK, BACK
};