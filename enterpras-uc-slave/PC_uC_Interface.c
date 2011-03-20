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
#include "message_processor.h"
#include "Nunchuck_Functions.h"
#include "motor_functions.h"
#include "time_functions.h"
#include "settings.h"
#include "definitions.h"

//Globals
volatile long leftEncoderCount;
volatile long rightEncoderCount;

volatile long leftVelocity;
volatile long rightVelocity;

volatile long ticks; //each tick is 1ms

volatile unsigned char control_mode;
volatile unsigned char drive_type; 
volatile unsigned char battery_voltage;

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
	initNunchuck();
	initWatchdog();
	
	//start encoder value interrupts (100ms)
	
	//start heartbeat flag clearing interrupts (~110ms)
	
	//start ticker interrupts
	
	//do general setup------------TODO: get this data from switches or something
	battery_voltage = 24;
	control_mode = NUNCHUCK;
	drive_type = DIFFERENTIAL;
}

int main()
{
	init(); //do all necessary initializations (including interrupts)
	
	while(1)
	{
		if(control_mode == AUTONOMOUS)
		{
			char incomingRequest[100];
			getMessage(incomingRequest, 100); //wait until we get a valid message
			//processRequest(incomingRequest); //process the message
		}
		else if(control_mode == NUNCHUCK) //control will automatically change to nunchuck whenever there is no heartbeat (TODO).
		{
			if(charIsAvailable())
			{
				control_mode = AUTONOMOUS; //if someone is trying to talk to us, go figure out what they want.
			}
			else
			{
				joyDrive(getNunchuckData()); //otherwise, be joyous!
			}
		}
		else //something really bad happened, so lets try nunchucking it again
		{
			control_mode = NUNCHUCK;
		}
	}
}
