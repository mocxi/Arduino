#include "Encoder.h"

encoder* encoder::GetInstance()
{
	static encoder* _encoder = NULL;
	if(!_encoder)
	{
		_encoder = new encoder();
	}
	return _encoder;
}

int encoder::m_blink = 0;

void encoder::init()
{
  last_check_encoder = 0;
  m_blink = 0;
  pinMode(ENCODER_INTERUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER_INTERUPT_PIN), encoder::speedBlink, FALLING);
}

const float& encoder::getEncoderSpeed()
{
  return m_speed;
}

void encoder::speedBlink()
{
  m_blink++;
}

void encoder::updateEncoder()
{
  if(last_check_encoder < millis() - ENCODER_INTERVAL)
  {
    m_speed = m_blink / ENCODER_PULSE;
    m_blink = 0;
    last_check_encoder = millis();
  }
}
