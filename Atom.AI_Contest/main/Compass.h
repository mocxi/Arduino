#ifndef Compass_h
#define Compass_h
#include "Define.h"
#if USE_COMPASS
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>


class Compass{
  /* Assign a unique ID to this sensor at the same time */
public:
  static Compass* GetInstance();
  Adafruit_HMC5883_Unified mag;
  bool begin();

  static float g_CurrentDirection;
  static float g_LastDirection;
  static float g_StartSteeringDirection;

  const float& getCompassDirect();
  void displaySensorDetails(void);
  void updateCompass();

};

#endif //USE_COMMPASS
#endif
