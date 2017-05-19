#ifndef __STEERING_H__
#define __STEERING_H__

#include "Define.h"
#include "Direction.h"
// Declare classes for Servo connectors of the MotorShield.
#include <Servo.h>
#include <DebugUtils.h>
//Huy.LH + new steering controller
//#define MIN_DELAY_SONAR_TIME 40 //milliseconds
#define LIMIT_STEERING_TURNING_ANGLE 10


//Huy.LH backward for a time





//Cotrol the front servo
class steering{
public:
  steering();
  void init(/* arguments */);

  unsigned long m_last_steering_update;
  unsigned long m_startSteeringTime;
  #if BALANCE_STEERING
  bool m_isBalancing;
  unsigned long m_last_balancing;
  #endif

  //Use uint8_t (1 byte = 255) for angle because the angle limit is between 0-180
  uint8_t SteeringAngle_Target, SteeringAngle_Current;

  bool isSteeringLeft, isSteeringRight, isTurning;

  TurnDirection SteeringDirection_Current;
  uint16_t Last_write_servo;
  Servo SteeringServo, EagleServo;


  static steering* GetInstance();
  void turnSteeringServo(TurnDirection direction, const uint8_t& angle);

  void updateSteeringServo();

  bool setServoTurn(uint8_t angle);
};

//Huy.LH -
#endif
