#include "inc/hw_types.h"		// tBoolean
#include "regionV2012.h"
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "RASLib/init.h"


// Definitions of corners
#define FLAG 0
#define DIRECT 1 //couldn't remember name of this one
#define LIGHT 2
#define FAN 3
#define TREE -1

// Order of sources
// The order indicates which one we want first
#define SOURCE_1 LIGHT
#define SOURCE_2 FAN
#define SOURCE_3 DIRECT

// Current location of robot
signed char currentCorner = TREE;

// These functions are up in the air at this point
//
//void followWall(void) {
//	//TODO figure out what exactly should go here
//}
//
//void moveForward(void) {
//	//-128 to 127
//	SetMotorPowers(127,127);
//}
//
//

//
// Goes to location
//
void gotoCorner(dest) {
	if (dest==TREE || dest==currentCorner) return; //return if invalid

	//TODO move to robot to this corner

	currentCorner = dest;
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
	signed char randoms = 3;

	//first when they are randomly on
	for (;randoms>=0;randoms--) {
		gotoCorner(nextSource());
			//TODO charge for some time?
		gotoCorner(FLAG);
			//TODO discharge?
	}

	//then when they are all on
	gotoCorner(SOURCE_1);
		//TODO charge
	gotoCorner(FLAG);
		//TODO discharge
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


