#include "inc/hw_types.h"
#include "utils/uartstdio.h"

#include "cb_i2c.h"

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/interrupt.h"


volatile status_t I2C_Status = DONE;
static volatile unsigned char data;
static unsigned char address;
static status_t (*callback)(unsigned char) = 0;


void I2CIntHandler(void) {
	data = I2C_Get(address);
	I2C_Status = DONE;
	I2CMasterIntClear(I2C0_MASTER_BASE);
	if (callback) I2C_Status = (*callback)(data);
}

void I2C_Init() {
  // Init I2C Bus
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

	GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);	
	I2CMasterInitExpClk(I2C0_MASTER_BASE, SysCtlClockGet(), true);
  I2CMasterEnable(I2C0_MASTER_BASE);

	I2CMasterIntEnable(I2C0_MASTER_BASE);
	IntEnable(INT_I2C0);
}

unsigned char I2C_Get(unsigned char address) {
	I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, address>>1, true);
	I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
	while(I2CMasterBusy(I2C0_MASTER_BASE));
	return I2CMasterDataGet(I2C0_MASTER_BASE);
}

void I2C_Put(unsigned char address, unsigned char value) {
	I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, address>>1, false);
	I2CMasterDataPut(I2C0_MASTER_BASE, value);
	I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);
	while(I2CMasterBusy(I2C0_MASTER_BASE));
}

unsigned char I2C_Request(unsigned char address, unsigned char value) {
	I2C_Background_Request(address, value, 0);
	while(I2C_Status == BUSY);
	return data;
}

void I2C_Background_Request(unsigned char address, unsigned char value, status_t (*cb)(unsigned char)) {
	while(I2C_Status == BUSY);
	callback = cb;
	address = address;
	I2C_Status = BUSY;
	I2C_Put(address,value);
}
