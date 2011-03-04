/*
 * UART_Functions.c
 * Provides functions that deal with UART I/O.
 * Author: Joshua James and Robby?
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

#define WATCHDOG_PERIOD 50000 * g_ulTicksPerUs

signed short speed;
signed short angle;

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

char getc(void) {
	char ch = UARTgetc();
	return ch;
}

void putc(char ch) {
	UARTprintf("%c",ch);
}

int charIsAvailable(void) {
	if (UARTCharsAvail(UART0_BASE))	
		return 1;
	else
		return 0;
}

/*void uartDemo(void) {
	UARTprintf("tell me something!\n-> ");
	
	{
		char charArray[100];
		UARTgets(charArray, 100);
		UARTprintf("you said, \"%s\"\n", charArray);
		UARTprintf("thanks for the input!\n");
	}
	
	{
		char newline = 13;
		char ch = getc();
		while(ch != newline) {
			ch = getc();
			putc(ch);
		}
	}
}*/

void setDifferentialMotorSpeeds(void)
{
	signed short l_vel;
	signed short r_vel;
	
	//if watchdog isn't fed, speed is reset to zero.
	l_vel = speed + angle;
	r_vel = speed - angle;
	
	l_vel = SATURATE(l_vel, -128, 127);
	r_vel = SATURATE(r_vel, -128, 127);
	
	l_vel = 127 - l_vel;
	r_vel = 127 - r_vel;
	
	SetServoPosition(SERVO_0, l_vel);
	SetServoPosition(SERVO_2, r_vel);
}

void resetWatchdogTimer(void)
{
	WatchdogReloadSet(WATCHDOG0_BASE, WATCHDOG_PERIOD);
	PWMOutputState(PWM_BASE, PWM_OUT_0_BIT | PWM_OUT_2_BIT, true);
}

void WatchdogIntHandler(void)
{
	WatchdogIntClear(WATCHDOG0_BASE);
	
	speed = 0;
	PWMOutputState(PWM_BASE, PWM_OUT_0_BIT | PWM_OUT_2_BIT, false);
	setDifferentialMotorSpeeds();
	
	//resetWatchdogTimer();
}


void initWatchdog()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);
	IntEnable(INT_WATCHDOG);
	resetWatchdogTimer();
	WatchdogEnable(WATCHDOG0_BASE);
}



void simpleCommTest(void)
{
	char buffer[80];
	
	
	initWatchdog();
	
	while(1)
	{
		getMessage(buffer, 80);
		
		switch(buffer[0])
		{
			case 'S': speed = SATURATE(atoi(&buffer[1]), -128, 127);
					  resetWatchdogTimer();
					  break;
			case 'A': angle = SATURATE(atoi(&buffer[1]), 127, -128);
					  resetWatchdogTimer();
					  break;
			default : break;
		}
		
		setDifferentialMotorSpeeds();
	}
}
