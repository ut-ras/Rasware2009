/*
 * PC_uC_Interface.c
 * Provides the microcontroller side of the link between the PC and the microcontroller
 * Author: Joshua James
 *
 */
 
//Includes 
#include "inc/hw_types.h"

#include "RASLib/init.h"
#include "RASLib/motor.h"
#include "RASLib/servo.h"
 
#include "UART_Functions.h"
#include "MessageProcessor.h"
#include "Nunchuck_Functions.h"

//Globals
volatile long leftEncoderCount;
volatile long rightEncoderCount;

volatile long leftVelocity;
volatile long rightVelocity;

volatile long ticks; //each tick is 1ms

//Functions
void init()
{
	//prevent lockout
	LockoutProtection();
	
	//initialize all physical sub systems
	InitializeMCU();
	InitializeServos();
	InitializeMotors(false, false);
	InitializeI2C();
	initUART();
	init_nunchuck();
	
	//start encoder value interrupts (100ms)
	
	//start heartbeat flag clearing interrupts (~110ms)
	
	//start ticker interrupts
}

int main()
{
	//int i;
	//int j = -107;
	init(); //do all necessary initializations (incl interrupts)
	
	//ackermannNunchuckTest();
	
	simpleCommTest();
	
	
	/*while(1)
	{
		for(i = -127; i < 128; i++)
		{
			SetServoPosition(SERVO_0, j);
			SetServoPosition(SERVO_2, j);
			WaitUS(30000);
		}
		for(i = 128; i >= -127; i--)
		{
			SetServoPosition(SERVO_0, j);
			SetServoPosition(SERVO_2, j);
			WaitUS(30000);
		}
	}*/
	
	
	/*while(1)
	{
		char incomingRequest[100];
		getMessage(incomingRequest, 100); //wait until we get a valid message
		processRequest(incomingRequest); //process the message
	}*/
}
