#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "utils/uartstdio.h"
#include "driverlib/gpio.h"
#include "driverlib/sysct1.h"
#include "RASLib/servo.h"
#include "panelServos.h"


int servo_0_hi = 10;
int servo_1_hi = 245;
int servo_0_lo = 0;
int servo_1_lo = 255;

void initPanelServos(void){
	InitializeServos();
}

void raisePanel(void){
	SetServoPosition(SERVO_0, int servo_0_hi);
	SetServoPosition(SERVO_1, int servo_1_hi);
}

void lowerPanel(void){
	SetServoPosition(SERVO_0, int servo_0_lo);
	SetServoPosition(SERVO_1, int servo_1_lo);
}	