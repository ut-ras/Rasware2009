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
#include "nunchuck_functions.h"
#include "motor_functions.h"
#include "time_functions.h"
#include "settings.h"
#include "definitions.h"
#include "adc_functions.h"
#include "gpio_functions.h"

//Globals
volatile long leftEncoderCount;
volatile long rightEncoderCount;

volatile long leftVelocity;
volatile long rightVelocity;

volatile long ticks; //each tick is 1ms

volatile unsigned char control_mode;
volatile unsigned char drive_type; 
volatile unsigned char battery_voltage;

volatile signed char desired_steering_angle;

volatile PIDdata steeringPID; //TODO: init steeringPID

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
	initADC();
	
	//start encoder value interrupts (100ms)
	
	//start heartbeat flag clearing interrupts (~110ms)
	
	//start ticker interrupts
	
	//do general setup------------TODO: get this data from switches
	battery_voltage = 24;
	control_mode = NUNCHUCK;
	drive_type = ACKERMANN;
}

int main()
{
	//TODO: put servo pwm in an interrupt
	
	init(); //do all necessary initializations (including interrupts)
	
	
	while(1)
	{
		if(control_mode == AUTONOMOUS)
		{
			//TODO: output to GPIO controlling flashing warning light
			if(controlSwitchPosition() == NUNCHUCK)
			{
				control_mode = NUNCHUCK;
			}
			else
			{
				if(charIsAvailable()) //if someone is trying to talk to us, go figure out what they want.
				{
					//TODO: Make this less blocky
					char incomingRequest[100];
					getMessage(incomingRequest, 100); //wait until we get a valid message
					//processRequest(incomingRequest); //process the message
				}
				pidSteeringServo(desired_steering_angle, getCurrentSteeringAngle()); //if we're bored, do some PID
			}
		}
		else if(control_mode == NUNCHUCK)
		{
			if(controlSwitchPosition() == AUTONOMOUS)
			{
				control_mode = AUTONOMOUS;
			}
			else
			{
				joyDrive(getNunchuckData()); //otherwise, be joyous!
				pidSteeringServo(desired_steering_angle, getCurrentSteeringAngle());
			}
		}
		else //something really bad happened, so lets try nunchucking it again
		{
			control_mode = NUNCHUCK;
		}
	}
}
