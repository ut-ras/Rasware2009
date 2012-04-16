#include "ADS7830.h"

 //includes copied from I2C demo
#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"		// input/output over UART
#include "inc/hw_ints.h"
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "driverlib/gpio.h"		// input/output over UART
#include "driverlib/adc.h"
#include "driverlib/i2c.h"
#include "driverlib/interrupt.h"

#include "RASLib/i2c.h"
#include "RASLib/timer.h"
#include "../src/i2c.c"

unsigned char ADS7830_Values[8];
unsigned char index;

void ADS7830_Init() {
    // Init I2C Bus
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

	GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);	
	I2CMasterInitExpClk(I2C0_MASTER_BASE, SysCtlClockGet(), false);
    I2CMasterEnable(I2C0_MASTER_BASE);

    // 0x84: 1 CH# 01 XX for request conversion. e.g 1 010 01 00 is for channel 2
	I2CMasterIntEnable(I2C0_MASTER_BASE);
	IntEnable(INT_I2C0);
	IntMasterEnable();
	
	I2CSend(ADS7830, 1, 0x84);
}



void I2CIntHandler(void) {
	UARTprintf("!!!");
	//I2CRecieve(ADS7830, &ADS7830_Values[index++], 1);
	//if (index < 8) I2CSend(ADS7830, 1, 0x84 | (index<<8)); 
	I2CMasterIntClear(I2C0_MASTER_BASE);
}



void ADS7830_Read(void) {
	index = 0;   
	I2CSend(ADS7830, 1, 0x84);
	//while (index < 8);
}
