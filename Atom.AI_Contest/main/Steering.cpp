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
	m_startSteeringTime = millis();
	SteeringAngle_Target = SERVO_FRONT_ANGLE;
	SteeringAngle_Current = SERVO_FRONT_ANGLE;
}

void steering::init(){
	/* code */
	SteeringServo.write(SERVO_FRONT_ANGLE);

#if BALANCE_STEERING
	m_isBalancing = true;
	m_last_balancing = 0;
#endif
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
	if(STEERING_OFF || (Last_write_servo > (millis() - SERVO_UPDATE_DELAY)))
	{
		//off steering
		return false;
	}

	if(SteeringAngle_Current == angle)
	{
		return true;
	}
	DBG("setServoTurn: ");
	DBG_LN(angle);
	//check range first 0-180
	if(angle > SERVO_LEFT_MAX && angle < SERVO_RIGHT_MAX)
	{
		//if(!g_isMinFront) // phphat : Should not add condition here. It can make us out of control the flow. Should add condition outside this function .  !!!
		{
			if(angle > (SERVO_FRONT_ANGLE + STEERING_BALANCE_ANGLE))
			{
				isSteeringLeft = false;
				isSteeringRight = true;
				m_startSteeringTime = millis();
			}
			else if(angle < (SERVO_FRONT_ANGLE - STEERING_BALANCE_ANGLE))
			{
				isSteeringLeft = true;
				isSteeringRight = false;
				m_startSteeringTime = millis();

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
		//DBG("Steering to ");
		//DBG_LN(SteeringAngle_Current);
		return true;
	}

	isTurning = false;
	SteeringDirection_Current = IDLE;
	return false;
}

void steering::updateSteeringServo()
{
	if(!MOTOR->m_isBackward && (isSteeringLeft || isSteeringRight))
	{
		if(m_startSteeringTime < millis() - STEERING_TIME)
		{
			isSteeringLeft = false;
			isSteeringRight = false;
			//DBG("steering time out!");
			setServoTurn(SERVO_FRONT_ANGLE);
		}
	}
	if(!DISTANCE->m_isMinFront)
	{
		switch (DIRECTION->g_current_condition) {
			case NO_WALL: // - - - 0
			{

				//DEBUG_PRINT(g_current_condition);
				//DEBUG_PRINTLN(" _ 0 _");
				STEERING->setServoTurn(SERVO_FRONT_ANGLE);
				break;
			}

			case LEFT_WALL: // + - - 1
				if(DIRECTION->isFollowLeft && DISTANCE->d_left > DISTANCE_NEED_SIDE_STEERING)
				{
					//90 degree, forward
					//DEBUG_PRINTLN("+ - - 90 degree, forward");
					STEERING->setServoTurn(SERVO_FRONT_ANGLE);
				} else
				{
					//turn right at 110 degree
					//DEBUG_PRINTLN("+ - - turn right at 110 degree");
					STEERING->setServoTurn(STEERING_LIGHT_RIGHT);
				}
				break;
			case FRONT_WALL: // - + - 2
				if(DIRECTION->isFollowLeft && DISTANCE->d_right > DISTANCE_NEED_SIDE_STEERING)
				{
					//turn left at 45 degree
					//DEBUG_PRINTLN("- + - turn left at 45 degree");
					STEERING->setServoTurn(STEERING_HARD_LEFT);
				} else
				{
					//turn right at 135 degree
					//DEBUG_PRINTLN("- + - turn right at 135 degree");
					STEERING->setServoTurn(STEERING_HARD_RIGHT);
				}
				break;
			case LEFT_FRONT_WALL: // + + - 3
				//turn right at STEERING_HARD_RIGHT
				STEERING->setServoTurn(STEERING_HARD_RIGHT);
				break;
			case RIGHT_WALL: // - - + 4
				if(DIRECTION->isFollowLeft)
				{
					//turn left at 70 degree
					//DEBUG_PRINTLN("- - + turn left at 70 degree");
					STEERING->setServoTurn(STEERING_LIGHT_LEFT);
				} else
				{
					//90 degree, forward
					//DEBUG_PRINTLN("- - + 90 degree, forward");
					STEERING->setServoTurn(SERVO_FRONT_ANGLE);
				}
				break;
			case LEFT_RIGHT_WALL: // + - + 5
				//90 degree, forward
				//DEBUG_PRINTLN("+ - + 90 degree, forward");
				STEERING->setServoTurn(SERVO_FRONT_ANGLE);
				break;
			case FRONT_RIGHT_WALL: // - + + 6
				//turn left at 45 degree
				//DEBUG_PRINTLN("- + + turn left at 45 degree");
				STEERING->setServoTurn(STEERING_HARD_LEFT);
				break;
			case ALL_WALL: // + + + 7
				//90 degree, backward
				//DEBUG_PRINTLN("+ + + 90 degree, slop detected, forward");
				//turnSteeringServo(STRAIGHT, A_90);
				STEERING->setServoTurn(SERVO_FRONT_ANGLE);
				DISTANCE->m_isMinFront = true;
				break;

		}
	}
	//khai end
//Huy.LH + Balancing
#if BALANCE_STEERING
	if(DISTANCE->d_front > MAX_DISTANCE_FRONT_WALL
		&& !(isSteeringLeft || isSteeringRight)
		&& !MOTOR->m_isBackward
		&& m_last_balancing < millis() - BALANCING_TIME
		&& ((DISTANCE->d_left + DISTANCE->d_right) < 100 //Huy.LH add
			|| DISTANCE->d_left < 20
			|| DISTANCE->d_right < 20
		)
	)
	{
		int deltaDistance = fabs(DISTANCE->d_left - DISTANCE->d_right);
		//delta left right bigger than 1/3 of distance 2 side
		if(deltaDistance > ((DISTANCE->d_left + DISTANCE->d_right)/4) && !m_isBalancing)
		{
			//DBG("Balancing");
			if(DISTANCE->d_left > DISTANCE->d_right)
			{
				//DBG_LN(" left");
				setServoTurn(SERVO_FRONT_ANGLE + STEERING_BALANCE_ANGLE);
			}
			else
			{
				//DBG_LN(" right");
				setServoTurn(SERVO_FRONT_ANGLE - STEERING_BALANCE_ANGLE);
			}
			m_isBalancing = true;
		}
		else if(m_isBalancing)
		{
			//DBG_LN("Balancing off");
			m_isBalancing = false;
			setServoTurn(SERVO_FRONT_ANGLE);
		}
		else
		{

		}

		m_last_balancing = millis();
	}
#endif
//Huy.LH -
}

void steering::turnSteeringServo(TurnDirection direction, const uint8_t& angle)
{
}
