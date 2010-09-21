#include "RobzDemo.h"

int main(void)
{
	initUART();
	uartDemo();
	
	initMotors();
	motorDemo();
	
	initServo();
	servoDemo();
	
	initLineSensor();
	lineSensorDemo();
	
	initIRSensor();
	IRSensorDemo();
	
	initEncoders();
	encoderDemo();
	
	while(1);
}
