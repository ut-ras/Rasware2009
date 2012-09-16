#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "driverlib/gpio.h"

#include "RASLib/init.h"
#include "RASLib/timer.h"

void init_motors() {
	
}

void set_motors() {
	
}

void init_input() {
	
}

void read_input() {
	
}

int main() {
	LockoutProtection();
	InitializeMCU();
	
	//init uart
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);				
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);	
	UARTStdioInit(0);
	
	init_motors();
	init_input();
	
	for (;;) {
		read_input();
		//blah
		set_motors();
	}
}

