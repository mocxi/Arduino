#ifndef __MOTOR_H__
#define __MOTOR_H__
#include "Define.h"
//#include "Distance.h"
// Arduino pins for the shift register
#define MOTORLATCH 12
#define MOTORCLK 4
#define MOTORENABLE 7
#define MOTORDATA 8

// 8-bit bus after the 74HC595 shift register
// (not Arduino pins)
// These are used to set the direction of the bridge driver.
#define MOTOR1_A 2
#define MOTOR1_B 3
#define MOTOR2_A 1
#define MOTOR2_B 4
#define MOTOR3_A 5
#define MOTOR3_B 7
#define MOTOR4_A 0
#define MOTOR4_B 6

// Arduino pins for the PWM signals.
#define MOTOR1_PWM 11
#define MOTOR2_PWM 3
#define MOTOR3_PWM 6
#define MOTOR4_PWM 5

extern bool is_go_FORWARD;
#define BOOST_MOTOR_TIME 500 //micros


class MotorController{
private:
  void shiftWrite(int output, int high_low);
public:
  MotorController();
  bool m_isBackward;
  uint16_t last_trigger_backward;

	void init();
	void motor_output (int output, int high_low, int speed);
	void motor(int nMotor, int command, int speed, bool needCheckBoost = true);
	void updateMotor(uint8_t g_current_condition, bool isFollowLeft);


	int g_currentSpeed;

	static MotorController* GetInstance();

};

#endif
