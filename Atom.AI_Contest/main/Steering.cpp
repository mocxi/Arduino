#include "Steering.h"
#include "Distance.h"
#include "Direction.h"
#include "Motor.h"

steering::steering():
isSteeringLeft(false),
isSteeringRight(false),
isTurning(false),
SteeringDirection_Current(STRAIGHT)

{
	SteeringServo.attach(SERVO1_PWM);
	Last_write_servo = millis();
	SteeringAngle_Target = SERVO_FRONT_ANGLE;
	SteeringAngle_Current = SERVO_FRONT_ANGLE;
}

void steering::init(){
	/* code */
	SteeringServo.write(SERVO_FRONT_ANGLE);
	m_isBalancing = true;
	m_last_balancing = 0;
}
steering* steering::GetInstance()
{
	static steering* _steering = NULL;
	if(!_steering)
	{
		_steering = new steering();
	}

	return _steering;
}

bool steering::setServoTurn(uint8_t angle)
{
	//SteeringServo.write(angle);
	//return;
	if(STEERING_OFF || Last_write_servo > (millis() - SERVO_UPDATE_DELAY))
	{
		//off steering
		return false;
	}

	if(SteeringAngle_Current == angle)
	{
		return true;
	}
	//check range first 0-180
	if(angle > SERVO_LEFT_MAX && angle < SERVO_RIGHT_MAX)
	{
		//if(!g_isMinFront) // phphat : Should not add condition here. It can make us out of control the flow. Should add condition outside this function .  !!!
		{
			if(angle >= (SERVO_FRONT_ANGLE + STEERING_ANGLE))
			{
				isSteeringLeft = false;
				isSteeringRight = true;
			}
			else if(angle <= (SERVO_FRONT_ANGLE - STEERING_ANGLE))
			{
				isSteeringLeft = true;
				isSteeringRight = false;
			}
			else
			{
				isSteeringLeft = false;
				isSteeringRight = false;
			}
		}
		DEBUG_PRINT("Huy steering to: ");
		DEBUG_PRINTLN(angle - A_0); // just for "reading"

		SteeringServo.write(angle);
		SteeringAngle_Current = angle;
		Last_write_servo = millis();
		DBG("Steering to ");
		DBG_LN(SteeringAngle_Current);
		return true;
	}

	isTurning = false;
	SteeringDirection_Current = IDLE;
	return false;
}

void steering::updateSteeringServo()
{
	if(!DISTANCE->m_isMinFront)
	{
		switch (DIRECTION->g_current_condition) {
			case NO_WALL: // - - -
			{

				//DEBUG_PRINT(g_current_condition);
				//DEBUG_PRINTLN(" _ 0 _");
				STEERING->setServoTurn(SERVO_FRONT_ANGLE);
				break;
			}

			case LEFT_WALL: // + - -
				if(DIRECTION->isFollowLeft)
				{
					//90 degree, forward
					//DEBUG_PRINTLN("+ - - 90 degree, forward");
					//STEERING->setServoTurn(SERVO_FRONT_ANGLE);
				} else
				{
					//turn right at 110 degree
					//DEBUG_PRINTLN("+ - - turn right at 110 degree");
					//STEERING->setServoTurn(SERVO_FRONT_ANGLE - STEERING_ANGLE);
				}
				break;
			case FRONT_WALL: // - + -
				if(DIRECTION->isFollowLeft)
				{
					//turn left at 45 degree
					//DEBUG_PRINTLN("- + - turn left at 45 degree");
					STEERING->setServoTurn(SERVO_FRONT_ANGLE + HARDTURN_ANGLE);
				} else
				{
					//turn right at 135 degree
					//DEBUG_PRINTLN("- + - turn right at 135 degree");
					STEERING->setServoTurn(SERVO_FRONT_ANGLE - HARDTURN_ANGLE);
				}
				break;
			case LEFT_FRONT_WALL: // + + -
				//turn right at 135 degree
				//DEBUG_PRINTLN("+ + - turn right at 135 degree");
				STEERING->setServoTurn(SERVO_FRONT_ANGLE - HARDTURN_ANGLE);
				break;
			case RIGHT_WALL: // - - +
				if(DIRECTION->isFollowLeft)
				{
					//turn left at 70 degree
					//DEBUG_PRINTLN("- - + turn left at 70 degree");
					//STEERING->setServoTurn(SERVO_FRONT_ANGLE + STEERING_ANGLE);
				} else
				{
					//90 degree, forward
					//DEBUG_PRINTLN("- - + 90 degree, forward");
					//STEERING->setServoTurn(SERVO_FRONT_ANGLE);
				}
				break;
			case LEFT_RIGHT_WALL: // + - +
				//90 degree, forward
				//DEBUG_PRINTLN("+ - + 90 degree, forward");
				STEERING->setServoTurn(SERVO_FRONT_ANGLE);
				break;
			case FRONT_RIGHT_WALL: // - + +
				//turn left at 45 degree
				//DEBUG_PRINTLN("- + + turn left at 45 degree");
				STEERING->setServoTurn(SERVO_FRONT_ANGLE + HARDTURN_ANGLE);
				break;
			case ALL_WALL: // + + +
				//90 degree, backward
				//DEBUG_PRINTLN("+ + + 90 degree, slop detected, forward");
				//turnSteeringServo(STRAIGHT, A_90);
				STEERING->setServoTurn(SERVO_FRONT_ANGLE);
				break;

		}
	}
	//khai end
	//Balance left right
	if(DISTANCE->d_front > MAX_DISTANCE_FRONT_WALL
		&& !(isSteeringLeft || isSteeringRight)
		&& !MOTOR->m_isBackward
		&& m_last_balancing < millis() - BALANCING_TIME
	)
	{
		int deltaDistance = fabs(DISTANCE->d_left - DISTANCE->d_right);
		//delta left right bigger than 1/3 of distance 2 side
		if(deltaDistance > ((DISTANCE->d_left + DISTANCE->d_right)/4) && !m_isBalancing)
		{
			DBG("Balancing");
			if(DISTANCE->d_left > DISTANCE->d_right)
			{
				DBG_LN(" left");
				setServoTurn(SERVO_FRONT_ANGLE + STEERING_ANGLE);
			}
			else
			{
				DBG_LN(" right");
				setServoTurn(SERVO_FRONT_ANGLE - STEERING_ANGLE);
			}
			m_isBalancing = true;
		}
		else if(m_isBalancing)
		{
			DBG_LN("Balancing off");
			m_isBalancing = false;
			setServoTurn(SERVO_FRONT_ANGLE);
			m_isBalancing = false;
		}
		else
		{

		}

		m_last_balancing = millis();
	}
}

void steering::turnSteeringServo(TurnDirection direction, const uint8_t& angle)
{
}
