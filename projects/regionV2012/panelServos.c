#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "utils/uartstdio.h"
#include "driverlib/gpio.h"
#include "driverlib/sysct1.h"
#include "RASLib/servo.h"

//these, and most of the code really, is from servoDemo

#include "panelServos.h"


//THIS HAS NOT BEEN TESTED!!!!!!




int servo_0_hi = 10;//servo 0 value when the panel is in the up position
int servo_1_hi = 245;//same as above but for servo 1

int servo_0_lo = 0;//servo 0 value when the panel is down
int servo_1_lo = 255;//same as above but for servo 1

void initPanelServos(void){//same code from servoDemo
	InitializeServos();
}

void raisePanel(void){
	//sets servo positions to the servo values of the panel in the raised condition
	SetServoPosition(SERVO_0, int servo_0_hi);
	SetServoPosition(SERVO_1, int servo_1_hi);
}

void lowerPanel(void){
	//sets servo positions to the servo values of the panel in the lower, light-collecting condition
	SetServoPosition(SERVO_0, int servo_0_lo);
	SetServoPosition(SERVO_1, int servo_1_lo);
}	