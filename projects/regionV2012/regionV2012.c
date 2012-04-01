#include "inc/hw_types.h"		// tBoolean
#include "regionV2012.h"
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "RASLib/init.h"


//
//Follow Wall method?
//
void followWall(void) {
	//TODO figure out what exactly should go here
}

void moveForward(void) {
	//-128 to 127
	SetMotorPowers(127,127);
}


//
//Initialization method
//
void init(void) {
	//Necessary inits for chip
	LockoutProtection();
	InitializeMCU();
	
	//Various driver inits
	//initUART
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);				
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);	
	UARTStdioInit(0);	

	//motors
	InitializeMotors(false, false);
}

//
//Run method
//
void run(void) {
	//Put actual game code here
	UARTprintf("Hello World\n");

	//Temp
	moveForward();
}

//
//Don't put any important code in main
//Use main freely to test functions and stuff
//
//For competition main should simply call init and run
//
int main(void) {	
	init();
	while(1){
		run();
	}
}


