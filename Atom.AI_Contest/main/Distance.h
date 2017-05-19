#ifndef __DISTANCE_H__
#define __DISTANCE_H__

#include <NewPing.h>
#include "Define.h"
#include "Arduino.h"
#include "Steering.h"

// Sensor HC SR04
//no matching function for call to 'NewPing::NewPing()'
extern NewPing sonar_front, sonar_right, sonar_left;

class distance{
private:
  //int m_angle, m_delta_angle;

  uint16_t Last_active_sensor = millis();
  uint8_t g_current_condition = 0;
  uint16_t Last_distance_check = millis();
  uint8_t arr_front_stack[];
  uint8_t arr_left_stack[];
  uint8_t arr_right_stack[];
public:
  distance();
  const int &getAvrTotal();
  void init();
  int avr_front , avr_right , avr_left, avr_TOTAL;  
  int d_front,d_right,d_left;
  bool isFollowLeft, isWallOnLeft, isWallOnRight, isWallInFront, isSlowSpeed, m_isMinFront;
  //void setAngle(const int &angle);
  //void setDeltaAngle(const int &delta_angle);
  static distance* GetInstance();
  int get_distance(NewPing sonar , int min , int max , int last_distance = MAX_DISTANCE);
  int ping_mean_cm(NewPing& sonar, int N, int max_cm_distance);
  uint8_t getAvrDistance(uint8_t stack[], uint8_t max_count);
  void updateDistanceStack();
  void updateDistance();
};

#endif
