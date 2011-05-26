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
    
	battery_voltage = 24;
	control_mode = controlSwitchPosition(); ///TODO: Unfix controlSwitchPosition()
	drive_type = ACKERMANN;
	steering_pider = COMPUTER;
	drive_pider = COMPUTER;
}

int main()
{
	init(); //do all necessary initializations (including interrupts)
	
	UARTprintf("Initialization complete!\n");
	
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
			}
		}
		else //something really bad happened, so lets try nunchucking it again
		{
			control_mode = NUNCHUCK;
		}
	}
}
