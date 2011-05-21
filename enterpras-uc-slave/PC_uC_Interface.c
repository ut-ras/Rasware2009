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
#include "RASLib/encoder.h"
#include "RASLib/timer.h"

#include "UART_Functions.h"
#include "message_processor.h"
#include "nunchuck_functions.h"
#include "motor_functions.h"
#include "time_functions.h"
#include "settings.h"
#include "definitions.h"
#include "adc_functions.h"
#include "gpio_functions.h"
#include "sorted_buf.h"
#include "utils.h"
#include "common.h"
#include "filtering_functions.h"
#include "driverlib/gpio.h" 
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"

#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART

volatile unsigned char control_mode;
volatile unsigned char drive_type; 
volatile unsigned char battery_voltage;
volatile unsigned char steering_pider;
volatile unsigned char drive_pider;

//Functions
void init()
{
	//prevent lockout
	LockoutProtection();
	
	//initialize all physical sub-systems
	InitializeMCU();
	initUART();
	UARTprintf("\n-------------------------------------------------------\n");
	UARTprintf("Initializing...");
	InitializeServos();
	InitializeMotors(false, false);
	InitializeEncoders(false, false);
	//InitializeI2C();
	//initNunchuck();
	initWatchdog();
	initADC();
	initTimers();
	initFilters();
	initGPIO();
	
	//initialize steeringPID (THESE VALUES SUCK)
	/*steeringPID.dState = getCurrentSteeringAngle(); // Last position input*/
    steeringPID.iState = 0; 						// Integrator state
    steeringPID.iMin = -127; 						// Minimum allowable integrator state
    steeringPID.iMax = 127; 						// Maximum allowable integrator state
    steeringPID.pGain = 0.1; 						// proportional gain
    steeringPID.iGain = 0.2; 						// integral gain
	steeringPID.dGain = 0.01; 						// derivative gain
    
	battery_voltage = 24;
	control_mode = controlSwitchPosition(); ///TODO: Unfix controlSwitchPosition()
	desired_steering_angle = 0;
	drive_type = ACKERMANN;
	steering_pider = COMPUTER;
	drive_pider = COMPUTER;
}

int main()
{	
	//int i;
	//unsigned char forward;
	//signed char voltage = 23;
	init(); //do all necessary initializations (including interrupts)
	
	UARTprintf("Initialization complete!\n");
	
	
	/*i = 0;
	forward = 1;
	while(1)
	{
		SetJaguarVoltage(STEERING_JAGUAR, voltage);
		for(i = 0; i < 15; i++)
		{
			resetWatchdogTimer();
			WaitUS(30000);
		}
		
		SetJaguarVoltage(STEERING_JAGUAR, -1 * voltage);
		for(i = 0; i < 15; i++)
		{
			resetWatchdogTimer();
			WaitUS(30000);
		}
		
		if(forward)
		{
			i++;
		}
		else
		{
			i--;
		}
		
		WaitUS(30000);
		if(i >= 127 || i <= -127)
		{
			forward ^= 1;
		}
	}// */
	
	while(1)
	{
		if(control_mode == AUTONOMOUS)
		{
			if(controlSwitchPosition() == NUNCHUCK)
			{
				control_mode = NUNCHUCK;
			}
			else
			{
				if(charIsAvailable()) //if someone is trying to talk to us, go figure out what they want.
				{
					handleCommMessage();
				}
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
