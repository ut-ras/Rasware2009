#include "inc/hw_types.h"		// tBoolean
#include "regionV2012.h"
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "RASLib/init.h"

//
//Follow Wall method?
//
void followWall(void) {
	//TODO figure out what exactly should go here
}


//
//Initialization method
//
void init(void) {
	//Necessary inits for chip
	LockoutProtection();
	InitializeMCU();
	
	//Various driver inits
	initUART();	
}

//
//Run method
//
void run(void) {
	while (1) {
		//Put actual game code here
		UARTprintf("Hello World\n");
	}
}

//
//Don't put any important code in main
//Use main freely to test functions and stuff
//
//For competition main should simply call init and run
//
int main(void) {	
	init();
	run();
}
