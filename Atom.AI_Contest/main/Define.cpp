#include "Define.h"

bool g_needUpdateMotor = false;
uint8_t g_DistanceStackIndex = 0;

int g_currentCommand = -1;
bool g_isBoosting = false;
float g_startBoostTime = 0.f;

#if USE_COMPASS
//#include "Compass.h"
//Compass* g_compass = new Compass();

  uint16_t last_check_position = millis();


  //typedef struct position Position;


#endif
