/*
 * UART_Functions.c
 * Provides functions that deal with UART I/O.
 * Author: Joshua James
 *
 */
 
#include <stdlib.h>				// malloc & free
#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

//for watchdog
#include "inc/hw_ints.h"		// input/output over UART
#include "driverlib/watchdog.h"
#include "driverlib/interrupt.h"
#include "RASLib/init.h"

#include "RASLib/servo.h"

#include "UART_Functions.h"
#include "time_functions.h"
#include "motor_functions.h"

void initUART(void) 
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);				
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);	
	UARTStdioInit(0);
}

void getMessage(char* buffer, char size)
{
	while(1)
	{
		UARTgets(buffer, size);
		return;
		
		/*if(buffer[0] == '{') //check that the communication is not an error
		{
			//TODO: Add additional check for length of request
			
			return;
		}*/
	}
}

void sendMessage(char* message)
{
	UARTprintf("%s", message);
}

char getc(void) 
{
	char ch = UARTgetc();
	return ch;
}

void putc(char ch) 
{
	UARTprintf("%c",ch);
}

tBoolean charIsAvailable(void) 
{
	if (UARTCharsAvail(UART0_BASE))	
		return true;
	else
		return false;
}

/*void simpleCommTest(void)
{
	char buffer[80];
	signed char speed;
	signed char angle;
	
	initWatchdog();
	
	while(1)
	{
		getMessage(buffer, 80);
		
		switch(buffer[0])
		{
			case 'S': speed = SATURATE(atoi(&buffer[1]), -128, 127);
					  resetWatchdogTimer();
					  break;
			case 'A': angle = SATURATE(atoi(&buffer[1]), -128, 127);
					  resetWatchdogTimer();
					  break;
			default : break;
		}
		
		setMotorSpeeds(speed,speed,angle);
	}
}*/
