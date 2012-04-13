#include "inc/hw_types.h"		// tBoolean
#include "regionV2012.h"
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "RASLib/init.h"
#define ;; ever

// Definitions of corners
#define FLAG 0
#define FAN 1 //couldn't remember name of this one
#define ELECTRIC 2
#define LIGHT 3
#define TREE -1

// Order of sources
// The order indicates which one we want first
#define BEST_SOURCE ELECTRIC

// Current location of robot
signed char currentCorner = TREE;

//
// Goes to location
//
void gotoCorner(signed char dest) {
	if (dest<0 || dest==currentCorner) return;

	if (currentCorner==TREE) {
		//just move to electric source
		currentCorner = ELECTRIC;
		if (dest != currentCorner) gotoCorner(dest);
		return;
	}
	
	//TODO go there	

	currentCorner = dest;
}

boolean charging(void){
	//returns true if capacitor is charging
	//aka the source is on
	return;	
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
	/*signed char randoms = 3;

	//first when they are randomly on
	for (;randoms>=0;randoms--) {
		gotoCorner(nextSource());
			//TODO charge for some time?
		gotoCorner(FLAG);
			//TODO discharge?
	}

	//then when they are all on
	gotoCorner(BEST_SOURCE);
		//TODO charge
	gotoCorner(FLAG);
		//TODO discharge*/
		
		
		
	//timer thingy
	
	while(not fully charged){
	
		while(time<=180s){//only one source on

			goToCorner(ELECTRIC);//go to the electric source, this is the default place to go
			while(charging());


			if(light source on){
				goToCorner(LIGHT);
				while(charging());
			}

			else{
				goToCorner(FAN);
				while(charging());
			}
		}

		//3 minutes have passed, all sources on
		//want to go to the best
	
	if(at the best && charging()){
		//do nothing, wait until fully charged
	}
	
	else{
		wall follow to best source;
		while(charging());
	}
		
	}
	
	//fully charged
	goToCorner(FLAG);

}

//
//Don't put any important code in main
//Use main freely to test functions and stuff
//
//For competition main should simply call init and run
//
int main(void) {	
	init();
	for (ever) run();
}


