#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "Define.h"
#include "Arduino.h"
#include "Steering.h"

// Digital pin 3: DC Motor #2 / Stepper #1 (activation/speed control)
// Digital pin 5: DC Motor #3 / Stepper #2 (activation/speed control)
// Digital pin 6: DC Motor #4 / Stepper #2 (activation/speed control)
// Digital pin 10: Servo #2 control

// Encoder pin
#define ENCODER_INTERUPT_PIN 2
#define ENCODER_PULSE 20

#define ENCODER_INTERVAL 1000 //millis()

class encoder{
private:
  float m_speed;
  static int m_blink;
  unsigned long last_check_encoder;

public:
  static void speedBlink();
  encoder();
  void init();
  const float& getEncoderSpeed();
  static encoder* GetInstance();
  void updateEncoder();
};

#endif
