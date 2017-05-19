#include "Distance.h"
#include <DebugUtils.h>

//Init
int avr_front , avr_right , avr_left, avr_TOTAL = 0;

//int angle, delta_angle;





NewPing sonar_front (PING_PIN_FRONT, PING_PIN_FRONT, MAX_DISTANCE);
NewPing sonar_right (PING_PIN_RIGHT, PING_PIN_RIGHT, MAX_DISTANCE);
NewPing sonar_left (PING_PIN_LEFT, PING_PIN_LEFT, MAX_DISTANCE);

distance* distance::GetInstance()
{
	static distance* _distance = NULL;
	if(!_distance)
	{
		_distance = new distance();
	}
	return _distance;
}

distance::distance():
isFollowLeft(false),
isWallOnLeft(false),
isWallOnRight(false),
isWallInFront(false),
isSlowSpeed(false),
m_isMinFront(false)
{
	Last_active_sensor = millis();
	Last_distance_check = millis();
}

const int& distance::getAvrTotal()
{
	return avr_TOTAL;
}

void distance::init()
{
	d_front = get_distance(sonar_front, MIN_DISTANCE, MAX_DISTANCE);
	DEBUG_PRINTLN("Setup d_front!");
	d_right = get_distance(sonar_right, MIN_DISTANCE, MAX_DISTANCE);
	DEBUG_PRINTLN("Setup d_right!");
	d_left = get_distance(sonar_left, MIN_DISTANCE, MAX_DISTANCE);
	DEBUG_PRINTLN("Setup d_left!");
	for (uint8_t i = 0; i < DISTANCE_STACK_MAX; i++)
	{
		arr_front_stack[i] = d_front + 10 ;
		arr_right_stack[i] = d_right + 10;
		arr_left_stack[i] = d_left + 10;
	}

  isFollowLeft = (d_right > d_left);
}

uint8_t distance::getAvrDistance(uint8_t stack[], uint8_t max_count)
{
	int sum = 0;
	for(uint8_t i = 0; i < max_count; i++)
	{
		sum += stack[i];
	}

	return sum/max_count;
}

void distance::updateDistanceStack()
{
		arr_front_stack[g_DistanceStackIndex] = d_front;
		arr_right_stack[g_DistanceStackIndex] = d_right;
		arr_left_stack[g_DistanceStackIndex] = d_left;

		g_DistanceStackIndex++;
		if (g_DistanceStackIndex >= DISTANCE_STACK_MAX) g_DistanceStackIndex = 0;

		avr_front = getAvrDistance(arr_front_stack,DISTANCE_STACK_MAX);
		avr_right = getAvrDistance(arr_right_stack,DISTANCE_STACK_MAX);
		avr_left = getAvrDistance(arr_left_stack,DISTANCE_STACK_MAX);
		avr_TOTAL = avr_front + avr_right + avr_left ;


		// Debug Log +
		DEBUG_PRINT(">>>>>>>> g_DistanceStackIndex ");
		DEBUG_PRINTLN(g_DistanceStackIndex);
		DEBUG_PRINTLN("updateDistanceStack ______________ BEGIN");
		DEBUG_PRINT("avr_front  ");
		DEBUG_PRINT(avr_front);
		DEBUG_PRINT(" avr_right  ");
		DEBUG_PRINT(avr_right);
		DEBUG_PRINT(" avr_left  ");
		DEBUG_PRINT(avr_left);
		DEBUG_PRINT(" [[avr_TOTAL]]  ");
		DEBUG_PRINTLN(avr_TOTAL);

		DEBUG_PRINT("d_front  ");
		DEBUG_PRINT(d_front);
		DEBUG_PRINT(" d_right  ");
		DEBUG_PRINT(d_right);
		DEBUG_PRINT(" d_left  ");
		DEBUG_PRINT(d_left);
		DEBUG_PRINT(" <<d_TOTAL>>  ");
		DEBUG_PRINTLN(d_front + d_right + d_left);
		DEBUG_PRINTLN(" ______________ END ");
		// Debug Log -
}

int distance::ping_mean_cm(NewPing& sonar, int N, int max_cm_distance)
{
	int sum_dist = 0;
	int temp_dist = 0;
	int i = 0;
	unsigned long t;

	while(i<=N)
	{
		t = micros();
		temp_dist = sonar.ping_cm(max_cm_distance);
		if(temp_dist > 0 && temp_dist <= max_cm_distance)
		{
			sum_dist += temp_dist;
			i++;
		}
		else
		{
			if(N==1)
			{
				return 0; //error result every ping
			}
			N--;
		}
		//add delay between ping (hardware limitation)
		if(i<=N && (micros() - t) < PING_MEAN_DELAY)
		{
			delay((PING_MEAN_DELAY + t - micros())/1000);
		}
	}

	return (sum_dist/N);
}

int distance::get_distance(NewPing sonar , int min , int max , int last_distance)
{

	int d = ping_mean_cm(sonar, GET_DISTANCE_COUNTS, MAX_DISTANCE);


	if (last_distance > 0 && (d < min || d > max))
	{
		d = last_distance;
	}
	else
	{
		if (d < min) d = min;
		if (d > max) d = max;
	}
	return d;
}

void distance::updateDistance()
{
	if(Last_distance_check < (millis()-CHECK_DISTANCE_DELAY))
	{
		updateDistanceStack();

		d_front = get_distance(sonar_front, MIN_DISTANCE, MAX_DISTANCE, MAX_DISTANCE );// phphat the 4th param should be previous value , not MAX_DISTANCE !!!
		d_right = get_distance(sonar_right, MIN_DISTANCE, MAX_DISTANCE, MAX_DISTANCE);
		d_left = get_distance(sonar_left, MIN_DISTANCE, MAX_DISTANCE, MAX_DISTANCE);


		Last_distance_check = millis();
	}
}
