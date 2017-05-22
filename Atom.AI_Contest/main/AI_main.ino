#include <Arduino.h>

#include "Define.h" //Huy.LH All define should be in here
#include "DebugUtils.h"
#include "math.h"
#include "Motor.h"
#include "Distance.h"

#include "Steering.h"

#if USE_COMPASS
  #include "Compass.h"
#endif


void setup()
{
	#ifdef DEBUG_BUILD
	 DebugUtils_init();
	#else
	Serial.begin(9600);
	#endif
	DEBUG_PRINTLN("Simple Motor Shield sketch");

	// Use the default "Servo" library of Arduino.
	// Attach the pin number to the servo library.
	// This might also set the servo in the middle position.

	DEBUG_PRINTLN("Setup start!");

	DISTANCE->init();

	DIRECTION->init();
	STEERING->init();
	//Huy.LH + compass sensor
	/* Initialise the sensor */
#if USE_COMPASS
	if(!Compass::GetInstance()->begin())
	{
		/* There was a problem detecting the HMC5883 ... check your connections */
		//Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
		while(1);
	}
	else
	{
		/* Display some basic information on this sensor */
		Compass::GetInstance()->displaySensorDetails();
	}
#endif
	//Huy.LH -
}

#if TEST_SERVO_SPIN==1
const int servoStep = 5;
void testServoSpin(Servo& servo)
{
	for (int i = SERVO_LEFT_MAX; i <= SERVO_RIGHT_MAX; i+=servoStep) //i: angle
  {
		DEBUG_PRINT("changed servo to angle: ");
	  DEBUG_PRINTLN(i);

	  servo.write(i);

	  delay(1500);
  }
}
#endif //TEST_SERVO_SPIN

#if TEST_PING==1
void testPing()
{
	int distance = sonar_left.ping_cm(MAX_DISTANCE);
	DEBUG_PRINT("left distance: "); DEBUG_PRINT(distance); DEBUG_PRINTLN("cm");
  distance = sonar_front.ping_cm(MAX_DISTANCE);
  DEBUG_PRINT("front distance: "); DEBUG_PRINT(distance); DEBUG_PRINTLN("cm");
  distance = sonar_right.ping_cm(MAX_DISTANCE);
  DEBUG_PRINT("right distance: "); DEBUG_PRINT(distance); DEBUG_PRINTLN("cm");
  DEBUG_PRINTLN("___________________");
  delay(1500);
}
#endif //TEST_PING


void loop()
{
	//delay(500); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay

	#if TEST_SERVO_SPIN==1
	testServoSpin(STEERING->SteeringServo);
	#endif
	#if TEST_PING==1
	testPing();
	#endif

	#if (TEST_SERVO_SPIN==1) || (TEST_PING==1)
	return;
	#endif
	//Update distance from sonar sensor
	DISTANCE->updateDistance();
	//Update distance from IR sensor

	//Update direction base on distance

	//Huy.LH + Backward to escape wall stuck
	if(MOTOR->m_isBackward)
	{
		if(MOTOR->last_trigger_backward < millis() - BACKWARD_TIME)
		{
			MOTOR->m_isBackward = false;
			delay(BACKWARD_DELAY);
		}
		else
		{
			LCD_PRINT_1 (0,"LOOP RETURN");
			//return;
		}

	}
  else
  {
    //Direction 1st
    DIRECTION->updateDirection(DISTANCE->d_left,DISTANCE->d_front,DISTANCE->d_right);
    //Steering 2nd
    STEERING->updateSteeringServo();

    	//Update speed for motor base on direction : Back <-> Forward
    MOTOR->updateMotor(DIRECTION->g_current_condition,DIRECTION->isFollowLeft);
  }



	// Debug +
	LCD_PRINT_1 (0,"B:" + (String)MOTOR->m_isBackward + " Spd:" + (String)MOTOR->g_currentSpeed);

	LCD_PRINT_2 (0,"L" + (String)(DISTANCE->d_left) + " F" + (String) (DISTANCE->d_front) + " R" + (String) (DISTANCE->d_right) );

	//delay (1000);
	lcd.backlight();
	// Debug -

}
