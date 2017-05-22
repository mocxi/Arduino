#include "Direction.h"
#include "DebugUtils.h"
#include "Steering.h"
#include "Distance.h"

#if USE_IR
	#include "IR_Distance.h"
#endif


//Init
direction* direction::GetInstance()
{
	static direction* _direction = NULL;
	if(!_direction)
	{
		_direction = new direction();
	}
	return _direction;
}

direction::direction():
isFollowLeft(false),
isWallOnLeft(false),
isWallOnRight(false),
isWallInFront(false)
{
	g_current_condition = 0;
#if DEBUG_LEVEL_INFO
	last_print_log = 0;
#endif
}

void direction::init()
{

}


void direction::updateDirection(const uint8_t d_left, const uint8_t d_front, const uint8_t d_right)
{
#if USE_IR
//Huy.LH + IR_2 and IR_3 next to the front
	isWallInFront = ((d_front < MAX_DISTANCE_FRONT_WALL) ||
				IR_DISTANCE->getIR_Status(ir_distance::IR_2) ||
				IR_DISTANCE->getIR_Status(ir_distance::IR_3));

//IR_LEFT is IR_1 and IR_2
	isWallOnLeft = ((d_left < MAX_DISTANCE_FROM_WALL) ||
				IR_DISTANCE->getIR_Status(ir_distance::IR_LEFT));

//IR_RIGHT is IR_3 and IR_4
	isWallOnRight = ((d_right < MAX_DISTANCE_FROM_WALL) ||
				IR_DISTANCE->getIR_Status(ir_distance::IR_RIGHT));
//Need detected stuckk after 2second here
//Huy.LH -
#else
	isWallInFront = (d_front < MAX_DISTANCE_FRONT_WALL);
	isWallOnLeft = (d_left < MAX_DISTANCE_FROM_WALL);
	isWallOnRight = (d_right < MAX_DISTANCE_FROM_WALL);
#endif



	isSlowSpeed = (d_front < SLOW_SPEED_DISTANCE_FRONT_WALL);
	if((d_front < MIN_FRONT_DISTANCE)
		|| (d_right < MIN_SIDE_DISTANCE)
		|| (d_left < MIN_SIDE_DISTANCE)
		|| IR_DISTANCE->IsThereAnyStuck()
	)
	{
		DISTANCE->m_isMinFront = true;
	}
	else{
		DISTANCE->m_isMinFront = false;
	}


	g_current_condition = ( isWallOnLeft ? ( 1<<0 ) : 0 ) +
				( isWallInFront  ? ( 1<<1 ) : 0 ) +
				( isWallOnRight ? ( 1<<2 ) : 0 );

	//g_current_condition if there is wall on left, in front or on right side of the car
	//...<left><front><right>
	//+: there is a wall on that side
	//-: there is no wall on that side
	// DEBUG_PRINT("<left><front><right>:");
#if DEBUG_LEVEL_INFO
	if(last_print_log < millis() - PRINT_LOG_DELAY)
	{
		DBG("Left: ");DBG(d_left);DBG(", Front: ");DBG(d_front);DBG(", Right: ");DBG(d_right);DBG("->");DBG_LN(g_current_condition);
		last_print_log = millis();
	}
#endif
/* phphat wtf here ??  lost info of d_front ,... when try print on lcd
	if(isFollowLeft)
	  {
		DEBUG_PRINTLN("I am following LLLLLLLLLLLEFT");
	  } else
	  {
		DEBUG_PRINTLN("I am following RRRRRRRRRRIGHT");
	  }
	//motor_output(MOTOR1_A, HIGH, 255); */
  g_needUpdateMotor = true;

}
