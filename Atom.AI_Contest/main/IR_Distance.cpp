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
	IR_status[4] = {false};
	IR_stuck_status[4] = {false};
	IR_start_counter_time[4] = {0};
}

bool ir_distance::getIR_Status(const IR& ir)
{
	switch (ir) {
		case IR_1:
		case IR_2:
		case IR_3:
		case IR_4:
		{
			return IR_status[ir];
			// break;
		}
		case IR_LEFT:
		{
			return IR_status[IR_1] || IR_status[IR_2];
		}
		case IR_RIGHT:
		{
			return IR_status[IR_3] || IR_status[IR_4];
		}
		case IR_ANY:
			return IR_status[IR_1] || IR_status[IR_2] || IR_status[IR_3] || IR_status[IR_4];
		default:
		{
			return false;
		}
	}
}

bool ir_distance::IsThereAnyStuck()
{
	return IR_stuck_status[IR_1] || IR_stuck_status[IR_2] || IR_stuck_status[IR_3] || IR_stuck_status[IR_4];
}

void ir_distance::init()
{
	pinMode(IR_PIN_1,INPUT);
	pinMode(IR_PIN_2,INPUT);
	pinMode(IR_PIN_3,INPUT);
	pinMode(IR_PIN_4,INPUT);
}

void ir_distance::updateIR_distance()
{
	IR_status[0]=digitalRead(IR_PIN_1);
	IR_status[1]=digitalRead(IR_PIN_2);
	IR_status[2]=digitalRead(IR_PIN_3);
	IR_status[3]=digitalRead(IR_PIN_4);

	for (int i = 0 ; i < IR_COUNT ; i++)
	{
		if(!IR_status[i])
		{
			IR_start_counter_time[i] = 0;
			IR_stuck_status[i] = false;
		}
		else if(IR_start_counter_time[i] == 0) //if wall detected
		{
			IR_start_counter_time[i] = millis();
		} else if((millis() - IR_start_counter_time[i]) > IR_STUCK_INTERVAL) //if wall detected for a long time
		{
			IR_stuck_status[i] = true;
		}

		DBG("updateir_distance [] ");
		DBG((String)i);
		DBG ("] : ");
		DBG_LN((String)IR_status[i]);
	}

}
