#ifndef __IR_DISTANCE_H__
#define __IR_DISTANCE_H__

#include "Define.h"
#include "Arduino.h"
#include "Steering.h"

// Digital pin 3: DC Motor #2 / Stepper #1 (activation/speed control)
// Digital pin 5: DC Motor #3 / Stepper #2 (activation/speed control)
// Digital pin 6: DC Motor #4 / Stepper #2 (activation/speed control)
// Digital pin 10: Servo #2 control
#define IR_PIN_1 3
#define IR_PIN_2 4
#define IR_PIN_3 6
#define IR_PIN_4 10

#define IR_STUCK_INTERVAL 2000

class ir_distance{
private:
public:
  enum IR{
    IR_1 = 0,
    IR_2,
    IR_3,
    IR_4,
    IR_COUNT,
    IR_LEFT,
    IR_RIGHT,
    IR_ANY
  };
  //int pin1,pin2,pin3,pin4;
  bool IR_status[4];
  bool IR_stuck_status[4];
  unsigned long IR_start_counter_time[4];
  ir_distance();
  void init();
  bool getIR_Status(const IR&);
  bool IsThereAnyStuck();
  static ir_distance* GetInstance();
  void updateIR_distance();
};

#endif
