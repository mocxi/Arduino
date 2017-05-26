#include "Motor.h"
#include "Distance.h"
#include "Steering.h"
#include "Define.h"
#include <DebugUtils.h>

#if USE_IR
	#include "IR_Distance.h"
#endif

#ifndef DEBUG_PRINT
#define DEBUG_PRINT(str) Serial.print(str)
#define DEBUG_PRINTLN(str) Serial.println(str)
#endif

bool is_go_FORWARD = true;

MotorController::MotorController()
{
	last_trigger_backward = millis();
}

MotorController* MotorController::GetInstance()
{
	static MotorController* _motorCtr = NULL;
	if(!_motorCtr)
	{
		_motorCtr = new MotorController();
	}
	return _motorCtr;
}


void MotorController::updateMotor(uint8_t g_current_condition,bool isFollowLeft)
{
	//phphat test
	//if(g_isBoosting && (g_startBoostTime > (millis() - BOOST_MOTOR_TIME)))
	// {
		//motor(MAIN_MOTOR, BACKWARD, MOTOR_BACKWARD_SPEED);
		//return;
	// }
	//else if(g_isBoosting)
	// {

		//g_isBoosting = false;
		//motor(MAIN_MOTOR, g_currentCommand, g_currentSpeed, false);
	// }

	// int detalDistance = fabs(DISTANCE->getAvrTotal() - (DISTANCE->d_front + DISTANCE->d_right + DISTANCE->d_left)) ;

	//if( detalDistance < 20)
	// if(0)
	// {
	// 	DEBUG_PRINTLN("updateMotor  STUCK <<<<<<<<<<<<<<<<<");
	// 	if (is_go_FORWARD) // go forward and stuck => go back
	// 	{
	// 		DEBUG_PRINTLN(" GO BACK __________ <<<<<<<<<<<<<<<<<");
	// 		is_go_FORWARD = false;
	// 		motor(MAIN_MOTOR, BACKWARD, MOTOR_BACKWARD_SPEED);
	// 	}
	// 	else  // go back and stuck => go forward
	// 	{
	// 		DEBUG_PRINTLN(" GO FORWARD __________ <<<<<<<<<<<<<<<<<");
	// 		is_go_FORWARD = true;
	// 		motor(MAIN_MOTOR, FORWARD, MOTOR_NORMAL_SPEED);
	// 	}
	// 	return;
	// }


	//___________________
	//if ((g_current_condition == 7) || g_isMinFront) //+++ //khai rem
	//khai modified, there are no dead end in contest
	//+++ now means slide, slop, forward
	if (DISTANCE->m_isMinFront && !m_isBackward) //+++
	{
		//DBG("updateMotor Go back");
		bool steeringResult = false;
		if(STEERING->isSteeringRight)
		{
			steeringResult = STEERING->setServoTurn(SERVO_FRONT_ANGLE + STEERING_BACK_ANGLE);
		}
		else if(STEERING->isSteeringLeft)
		{
			steeringResult = STEERING->setServoTurn(SERVO_FRONT_ANGLE - STEERING_BACK_ANGLE);
		}
		else{
		#if USE_IR
			if(IR_DISTANCE->getIR_Status(ir_distance::IR_LEFT))
			{
				steeringResult = STEERING->setServoTurn(SERVO_FRONT_ANGLE + STEERING_BACK_ANGLE);
			}
			else if(IR_DISTANCE->getIR_Status(ir_distance::IR_RIGHT))
			{
				steeringResult = STEERING->setServoTurn(SERVO_FRONT_ANGLE - STEERING_BACK_ANGLE);
			}
			else
			{
				steeringResult = STEERING->setServoTurn(SERVO_FRONT_ANGLE + (DIRECTION->isFollowLeft? -STEERING_BACK_ANGLE : STEERING_BACK_ANGLE));
			}
		#else
			if(DISTANCE->d_left > DISTANCE->d_right * 1.5)
			{
				steeringResult = STEERING->setServoTurn(SERVO_FRONT_ANGLE - STEERING_BACK_ANGLE);
			}
			else
			{
				steeringResult = STEERING->setServoTurn(SERVO_FRONT_ANGLE + STEERING_BACK_ANGLE);
			}
		#endif
		}

		if(steeringResult)
		{
			is_go_FORWARD = false;
			motor(MAIN_MOTOR, BACKWARD, MOTOR_BACKWARD_SPEED);
			m_isBackward = true;
			last_trigger_backward = millis();
		}
		else
		{
			DBG("Motor do not thing!");
			//motor(MAIN_MOTOR, BACKWARD, 0);
		}
	}
	//else if(DISTANCE->isSlowSpeed)
	//{
		// go head
		//DEBUG_PRINTLN("updateMotor Go FORWARD SLOW");
		//is_go_FORWARD = true;
		//motor(MAIN_MOTOR, FORWARD, MOTOR_LOW_SPEED);
	//}
	else if(STEERING->isSteeringRight || STEERING->isSteeringLeft)
	{
		DBG("updateMotor Go FORWARD !!");
		is_go_FORWARD = true;

		if(g_currentSpeed == MOTOR_MAX_SPEED)
		{
			//motor(MAIN_MOTOR, FORWARD, 0);
			delay(500);
		}
		motor(MAIN_MOTOR, FORWARD, MOTOR_NORMAL_SPEED);

	}
	else
	{
		motor(MAIN_MOTOR, FORWARD, MOTOR_MAX_SPEED);
	}
	g_needUpdateMotor = false;
}

// --------------------------------------------------------------------

void MotorController::motor(int nMotor, int command, int speed, bool needCheckBoost)
{

	if(g_currentSpeed == speed)
	{
		return;
	}
	DBG("command: ");
	DBG(command);
	DBG(", speed: ");
	DBG_LN(speed);
	g_currentCommand = command;
	g_currentSpeed = speed;
	// if(0)
	// //if(needCheckBoost && speed > 0)
	// {
	// 	if(g_currentCommand != command)
	// 	{
	// 		DEBUG_PRINT("Boost ");DEBUG_PRINT(command);DEBUG_PRINT(", speed: ");DEBUG_PRINTLN(speed);
	//
	//
	// 		g_currentSpeed = speed;
	// 		g_startBoostTime = millis();
	// 		g_isBoosting = true;
	// 		motor(nMotor, command, MOTOR_BOOST_SPEED, false);
	// 		return;
	// 	}
	// }
  int motorA, motorB;

  if (nMotor >= 1 && nMotor <= 4)
  {
    switch (nMotor)
    {
    case 1:
      motorA   = MOTOR1_A;
      motorB   = MOTOR1_B;
      break;
    case 2:
      motorA   = MOTOR2_A;
      motorB   = MOTOR2_B;
      break;
    case 3:
      motorA   = MOTOR3_A;
      motorB   = MOTOR3_B;
      break;
    case 4:
      motorA   = MOTOR4_A;
      motorB   = MOTOR4_B;
      break;
    default:
      break;
    }

    switch (command)
    {
    case FORWARD:
      motor_output (motorA, HIGH, speed);
      motor_output (motorB, LOW, -1);     // -1: no PWM set
      break;
    case BACKWARD:
      motor_output (motorA, LOW, speed);
      motor_output (motorB, HIGH, -1);    // -1: no PWM set
      break;
    case BRAKE:
      // The AdaFruit library didn't implement a brake.
      // The L293D motor driver ic doesn't have a good
      // brake anyway.
      // It uses transistors inside, and not mosfets.
      // Some use a software break, by using a short
      // reverse voltage.
      // This brake will try to brake, by enabling
      // the output and by pulling both outputs to ground.
      // But it isn't a good break.
      motor_output (motorA, LOW, 255); // 255: fully on.
      motor_output (motorB, LOW, -1);  // -1: no PWM set
      break;
    case RELEASE:
      motor_output (motorA, LOW, 0);  // 0: output floating.
      motor_output (motorB, LOW, -1); // -1: no PWM set
      break;
    default:
      break;
    }
  }
}


// ---------------------------------
// motor_output
//
// The function motor_ouput uses the motor driver to
// drive normal outputs like lights, relays, solenoids,
// DC motors (but not in reverse).
//
// It is also used as an internal helper function
// for the motor() function.
//
// The high_low variable should be set 'HIGH'
// to drive lights, etc.
// It can be set 'LOW', to switch it off,
// but also a 'speed' of 0 will switch it off.
//
// The 'speed' sets the PWM for 0...255, and is for
// both pins of the motor output.
//   For example, if motor 3 side 'A' is used to for a
//   dimmed light at 50% (speed is 128), also the
//   motor 3 side 'B' output will be dimmed for 50%.
// Set to 0 for completelty off (high impedance).
// Set to 255 for fully on.
// Special settings for the PWM speed:
//    Set to -1 for not setting the PWM at all.
//
void MotorController::motor_output (int output, int high_low, int speed)
{
  if(MOTOR_OFF)
  {
    //DEBUG_PRINT("phphat motor_output off");
    return;
  }
  int motorPWM;

  switch (output)
  {
  case MOTOR1_A:
  case MOTOR1_B:
    motorPWM = MOTOR1_PWM;
    break;
  case MOTOR2_A:
  case MOTOR2_B:
    motorPWM = MOTOR2_PWM;
    break;
  case MOTOR3_A:
  case MOTOR3_B:
    motorPWM = MOTOR3_PWM;
    break;
  case MOTOR4_A:
  case MOTOR4_B:
    motorPWM = MOTOR4_PWM;
    break;
  default:
    // Use speed as error flag, -3333 = invalid output.
    speed = -3333;
    break;
  }

  if (speed != -3333)
  {
    // Set the direction with the shift register
    // on the MotorShield, even if the speed = -1.
    // In that case the direction will be set, but
    // not the PWM.
    shiftWrite(output, high_low);

    // set PWM only if it is valid
    if (speed >= 0 && speed <= 255)
    {
      analogWrite(motorPWM, speed);
    }
  }
}


// ---------------------------------
// shiftWrite
//
// The parameters are just like digitalWrite().
//
// The output is the pin 0...7 (the pin behind
// the shift register).
// The second parameter is HIGH or LOW.
//
// There is no initialization function.
// Initialization is automatically done at the first
// time it is used.
//
void MotorController::shiftWrite(int output, int high_low)
{
  static int latch_copy;
  static int shift_register_initialized = false;

  // Do the initialization on the fly,
  // at the first time it is used.
  if (!shift_register_initialized)
  {
    // Set pins for shift register to output
    pinMode(MOTORLATCH, OUTPUT);
    pinMode(MOTORENABLE, OUTPUT);
    pinMode(MOTORDATA, OUTPUT);
    pinMode(MOTORCLK, OUTPUT);

    // Set pins for shift register to default value (low);
    digitalWrite(MOTORDATA, LOW);
    digitalWrite(MOTORLATCH, LOW);
    digitalWrite(MOTORCLK, LOW);
    // Enable the shift register, set Enable pin Low.
    digitalWrite(MOTORENABLE, LOW);

    // start with all outputs (of the shift register) low
    latch_copy = 0;

    shift_register_initialized = true;
  }

  // The defines HIGH and LOW are 1 and 0.
  // So this is valid.
  bitWrite(latch_copy, output, high_low);

  // Use the default Arduino 'shiftOut()' function to
  // shift the bits with the MOTORCLK as clock pulse.
  // The 74HC595 shiftregister wants the MSB first.
  // After that, generate a latch pulse with MOTORLATCH.
  shiftOut(MOTORDATA, MOTORCLK, MSBFIRST, latch_copy);
  delayMicroseconds(5);    // For safety, not really needed.
  digitalWrite(MOTORLATCH, HIGH);
  delayMicroseconds(5);    // For safety, not really needed.
  digitalWrite(MOTORLATCH, LOW);
}
