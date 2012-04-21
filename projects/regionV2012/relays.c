//relay.c
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "relays.h"


void InitializeRelays(void){ 
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_5| GPIO_PIN_6 | GPIO_PIN_7);
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_5| GPIO_PIN_6 | GPIO_PIN_7, 0x00);
}

void CapacitorsSeries(void){
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_7, GPIO_PIN_7);
}

void CapacitorsParalell(void){
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_7, 0x00);
}

void WindOn(void){
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, GPIO_PIN_6);
}

void WindOff(void){
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, 0x00);
}

void SolarOn(void){
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_5, GPIO_PIN_5);
}

void SolarOff(void){
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_5, 0x00);
}

