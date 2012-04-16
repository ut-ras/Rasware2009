#include "ADS7830.h"

 //includes copied from I2C demo
#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"		// input/output over UART
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "driverlib/gpio.h"		// input/output over UART
#include "driverlib/adc.h"
#include "driverlib/i2c.h"

#include "RASLib/i2c.h"
#include "../src/i2c.c"

void ADS7830_Init() {
    // Init I2C Bus
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

	GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);
    I2CMasterInitExpClk(I2C0_MASTER_BASE, SysCtlClockGet(), false);
    
    // 0x84: 1 CH# 01 XX for request conversion. e.g 1 010 01 00 is for channel 2
    I2CSend(ADS7830 << 1, 1, 0x84);
}

/*
* data: pointer to varaible (char)
* num:  port to read (0-7)
*/
void ADS7830_Read(unsigned char* data,unsigned char num)
{
    I2CSend(ADS7830 << 1, 0x84 | (0x10*num), 1);
    I2CRecieve(ADS7830 << 1, data, 1);
}

/*
* data: pointer to 8 element array (type char)
*/
void ADS7830_BurstRead(unsigned char* data)  // data: Pointer to array
{
    char i;
    
    for(i=0; i<0x80; i+=0x10){
        I2CSend(ADS7830 << 1, 0x84 | i, 1);
        I2CRecieve(ADS7830 << 1, data++, 1);
    }
}
