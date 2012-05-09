#include "inc/hw_types.h"
#include "utils/uartstdio.h"

#include "cb_travel.h"

#include "cb_encoder.h"
#include "cb_motor.h"

void ForwardHandler(signed long * data) {
	Motor_Set(data[0]<=data[1]?127:127-(16*(data[0]-data[1])), data[1]<=data[0]?127:127-(16*(data[1]-data[0])));
}

void Travel_Init(void) {
	Motor_Init(false,true);
	Encoder_Init(true,false);
}

void Travel_Forward(signed long dist) {
	Encoder_Background_Read(&ForwardHandler);
	Encoder_Callback(0,0,0);
	Encoder_Callback(1,0,0);
	Motor_Set(MOTOR_FORWARD,MOTOR_FORWARD);
}

void Travel_Turn(signed long dist) {
	Encoder_Background_Read(0);
}

void Travel_Stop(void) {
	Encoder_Background_Read(0);
	Encoder_Callback(0,0,0);
	Encoder_Callback(1,0,0);
}
