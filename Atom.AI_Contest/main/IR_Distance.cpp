#include "IR_Distance.h"
#include <DebugUtils.h>

ir_distance* ir_distance::GetInstance()
{
	static ir_distance* _ir_distance = NULL;
	if(!_ir_distance)
	{
		_ir_distance = new ir_distance();
	}
	return _ir_distance;
}

ir_distance::ir_distance()
{
  pin1 = 2;
  pin2 = 3;
  pin3 = 4;
  pin4 = 5;
}

void ir_distance::init()
{
	pinMode(pin1,INPUT);
	pinMode(pin2,INPUT);
	pinMode(pin3,INPUT);
	pinMode(pin4,INPUT);
}

void ir_distance::updateIR_distance()
{
	IR_status[0]=digitalRead(pin1);
	IR_status[1]=digitalRead(pin2);
	IR_status[2]=digitalRead(pin3);
	IR_status[3]=digitalRead(pin4);

	for (int i = 0 ; i < 4 ; i++)
	{		
		DBG("updateir_distance [] ");
		DBG((String)i);
		DBG ("] : ");
		DBG_LN((String)IR_status[i]);
	}

}
