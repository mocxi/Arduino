#ifndef __IR_DISTANCE_H__
#define __IR_DISTANCE_H__

#include "Define.h"
#include "Arduino.h"
#include "Steering.h"


class ir_distance{
private:
public:
  int pin1,pin2,pin3,pin4;
  int IR_status[4] = {0};
  ir_distance();
  void init();
  static ir_distance* GetInstance();
  void updateIR_distance();
};

#endif
