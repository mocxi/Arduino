#ifndef __DIRECTION_H__
#define __DIRECTION_H__

#include <NewPing.h>
#include "Define.h"
#include "Arduino.h"


enum TurnDirection{
	STRAIGHT = 0,
	TURN_LEFT,
	TURN_RIGHT,
	BACK,
	IDLE
};

class direction{
	#if DEBUG_LEVEL_INFO
	unsigned long last_print_log;
	#endif
public:
	bool isFollowLeft, isWallOnLeft, isWallOnRight, isWallInFront, isSlowSpeed;
	uint8_t g_current_condition = 0;
	direction();
	void init();
	static direction* GetInstance();
	void updateDirection(const uint8_t d_left, const uint8_t d_front, const uint8_t d_right);
};

#endif
