#include "Compass.h"
#include "Steering.h"
#include "Motor.h"

#if USE_COMPASS

float Compass::g_CurrentDirection;
float Compass::g_LastDirection;
float Compass::g_StartSteeringDirection;
Compass* Compass::GetInstance()
{
  static Compass* compass = NULL;
  if(compass == NULL)
  {
    compass = new Compass();
  }

  return compass;
}
bool Compass::begin()
{
  last_check_position = millis();
  this->mag = Adafruit_HMC5883_Unified(12345);
  return this->mag.begin();
}

const float& Compass::getCompassDirect()
{
  sensors_event_t event;
  mag.getEvent(&event);
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  //Magnetic declination: -0� 28' to radians
  float declinationAngle = 0.0049;

  heading += declinationAngle;
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;

  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;

  // Convert radians to degrees for readability.
  //float headingDegrees = heading * 180/M_PI;
  return heading * 180/M_PI;
};

void Compass::updateCompass()
{
  if(last_check_position < millis() - CHECK_POSITION_DELAY)
  {
    g_LastDirection = g_CurrentDirection;
    g_CurrentDirection = getCompassDirect();

    Serial.print("Heading Degrees: "); Serial.print(g_CurrentDirection); Serial.print("  ");Serial.println("uT");

    //Checking distance moved
    //float movedDistance = getDistanceBetween2Point(g_CurrentDirection, g_LastPos);
    //if(movedDistance < STUCK_DISTANCE)
    {
      //stuck detected here, move backward?
      if(!MOTOR->m_isBackward && (STEERING->isSteeringLeft || STEERING->isSteeringRight))
      {
        //Checnk the angle steered
        uint8_t streeredAngle = fabs(g_CurrentDirection - g_StartSteeringDirection);
        //Just a test variable
        uint8_t steeringAngle = 45;

        //If you steer 40 degrees, 32~40 is accepted, if not, then
        if(streeredAngle < steeringAngle * ACCEPTED_MIN_ACCURACY_PERCENT)
        {
          //backward here until we go to an accepted degrees and go forward

        }
      }
      else if(MOTOR->m_isBackward)
      {
        //if(streeredAngle > steeringAngle * ACCEPTED_MAX_ACCURACY_PERCENT)
      }
    }

    last_check_position = millis();
  }
};

void Compass::displaySensorDetails(void)
{
  sensor_t sensor;
  mag.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
};
#endif
