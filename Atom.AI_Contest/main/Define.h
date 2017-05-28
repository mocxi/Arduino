#ifndef __DEFINE_H__
#define __DEFINE_H__
#include "Arduino.h"
//#include <DebugUtils.h> //error comiplie multi define

#define DEBUG_BUILD
#define DEBUG_LEVEL_INFO 1
#if DEBUG_LEVEL_INFO
#define PRINT_LOG_DELAY 2000
#endif

//khai start
#define TEST_SERVO_SPIN 0
#define TEST_PING 0
//khai end
//Huy.LH encoder
#define USE_ENCODER 0
#if USE_ENCODER
#define ENCODER encoder::GetInstance()
#endif
// MOTOR ----------------------------------------------------------
#define MOTOR MotorController::GetInstance()

#define MOTOR_OFF 0
#define STEERING_OFF 0

#define BACKWARD_DELAY 500
#define BACKWARD_TIME 1000

// Digital pin 11: DC Motor #1 / Stepper #1 (activation/speed control)
// Digital pin 3: DC Motor #2 / Stepper #1 (activation/speed control)
// Digital pin 5: DC Motor #3 / Stepper #2 (activation/speed control)
// Digital pin 6: DC Motor #4 / Stepper #2 (activation/speed control)
//Digital pin 4, 7, 8 and 12 are used to drive the DC/Stepper motors via the 74HC595 serial-to-parallel latch
// Digitals pin 9: Servo #1 control
// Digital pin 10: Servo #2 control
// Digital pin 2 unused
#define MAIN_MOTOR 3 //Huy.LH new reborn
// Motor speed
// MIN_SPEED to run is 140
#define MOTOR_MAX_SPEED 255
#define MOTOR_NORMAL_SPEED 255
//#define MOTOR_LOW_SPEED 140
#define MOTOR_BACKWARD_SPEED 255 //80
#define MOTOR_BOOST_SPEED 255

enum WALL_CONDITION{
	NO_WALL = 0,
	LEFT_WALL,
	FRONT_WALL,
	LEFT_FRONT_WALL,
	RIGHT_WALL,
	LEFT_RIGHT_WALL,
	FRONT_RIGHT_WALL,
	ALL_WALL
};
// Codes for the motor function.
#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4
//Global variable

extern int g_currentCommand;

extern bool g_isBoosting;
extern float g_startBoostTime;



// SERVO ----------------------------------------------------------
#define STEERING steering::GetInstance()

#define SERVO_UPDATE_DELAY 1500
#define SERVO1_PWM 10
//#define SERVO2_PWM 9

// Servo angles in degrees , from 0 -> 180
#define A_0 0 // init value
#define A_15 A_0 + 15 // 15 degrees
#define A_30 A_0 + 30 // 15 degrees
#define A_45 A_0 + 55 // 15 degrees
#define A_75 A_0 + 75 // 15 degrees
//#define A_90 A_0 + 90 // 15 degrees
#define A_105 A_0 + 105 // 15 degrees
#define A_120 A_0 + 120 // 15 degrees
#define A_135 A_0 + 135 // 15 degrees
#define A_150 A_0 + 150 // 15 degrees
#define A_165 A_0 + 165 // 15 degrees
#define A_180 A_0 + 180 // 15 degrees

#define BALANCING_TIME 50
#define STEERING_TIME 200 //milliseconds


//khai add
#define HARDTURN_ANGLE 35
#define STEERING_LIGHT_ANGLE 20
#define STEERING_BACK_ANGLE 20
//khai end

//Huy.LH + Balancing
#define BALANCE_STEERING 1
#if BALANCE_STEERING
#define STEERING_BALANCE_ANGLE 4
#endif
//Huy.LH -

//Huy.LH + define server angle
#define SERVO_FRONT_ANGLE 90
#define SERVO_LEFT_45 45
#define SERVO_RIGHT_45 135
#define SERVO_LEFT_MAX 29
#define SERVO_RIGHT_MAX 170

#define STEERING_HARD_LEFT SERVO_FRONT_ANGLE + HARDTURN_ANGLE
#define STEERING_LIGHT_LEFT SERVO_FRONT_ANGLE + STEERING_LIGHT_ANGLE
#define STEERING_HARD_RIGHT SERVO_FRONT_ANGLE - HARDTURN_ANGLE
#define STEERING_LIGHT_RIGHT SERVO_FRONT_ANGLE - STEERING_LIGHT_ANGLE
//Huy.LH -

extern bool g_needUpdateMotor;

//IR Distance --------------------------------------------------
#define USE_IR 0
#if USE_IR
 #define IR_DISTANCE ir_distance::GetInstance()
#endif
//Distance --------------------------------------------------
#define DISTANCE distance::GetInstance()
//Direction --------------------------------------------------
#define DIRECTION direction::GetInstance()

#define PING_PIN_FRONT  A0 // Arduino pin for both trig and echo
#define PING_PIN_RIGHT  A2 // Arduino pin for both trig and echo
#define PING_PIN_LEFT   A1 // Arduino pin for both trig and echo

#define MIN_DELAY_SONAR_TIME 40 //milliseconds
#define CHECK_DISTANCE_DELAY 100
#define GET_DISTANCE_COUNTS 5

//DISTANCE FROM WALL+++++++
#define MAX_DISTANCE 100

#define MAX_FRONT_DISTANCE 100
#define MIN_FRONT_DISTANCE 15
#define MIN_SIDE_DISTANCE 9

#define MIN_DISTANCE 3
#define DELTA_DISTANCE 30
#define MAX_DISTANCE_FROM_WALL 40
#define MAX_DISTANCE_FRONT_WALL 50
#define SLOW_SPEED_DISTANCE_FRONT_WALL 95

#define DISTANCE_NEED_SIDE_STEERING 20

#define PING_MEAN_DELAY 29000 //microseconds

//distance stack +
#define DISTANCE_STACK_MAX 5
//distance stack -
extern uint8_t g_DistanceStackIndex;

#define USE_COMPASS 0 //Huy.LH + compass

#if USE_COMPASS

	#include "Compass.h"

	//extern Compass* g_compass;

  #define CHECK_POSITION_DELAY 1
  #define STUCK_DISTANCE 2



  extern uint16_t last_check_position;


  #define ACCEPTED_MIN_ACCURACY_PERCENT 0.8f
  #define ACCEPTED_MAX_ACCURACY_PERCENT 1.2f
#endif //USE_COMMPASS

#endif
