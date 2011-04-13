/*
 * gpio_functions.c
 * Provides functions for control of devices attached to the gpio ports.
 * Author: Joshua James
 *
 */

#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"		// input/output over UART
#include "driverlib/gpio.h" 
#include "driverlib/sysctl.h"
#include "utils/uartstdio.h"
#include "settings.h"
#include "definitions.h"

 
void initGPIO(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
 	GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_2);
}

char controlSwitchPosition(void)
{
	return GPIOPinRead(GPIO_PORTA_BASE,GPIO_PIN_2);
}
