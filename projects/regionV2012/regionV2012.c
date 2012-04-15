#include "inc/hw_types.h"		// tBoolean
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "RASLib/init.h"
<<<<<<< HEAD
#define ;; ever
=======
#include "travel.h"
#include "charging.h"
#include "panelServos.h"
#include "ADS7830.h"

>>>>>>> 60ecd2ff0a09f357b8881ea362ae24911ae57353


// Order of sources
// The order indicates which one we want first
#define BEST_SOURCE ELECTRIC

//
// Goes to location
//


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
	
	travelInit();
}


//
//Run method
//
void run(void) {
	//I need to comment this to compile

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
	/*	
		
	int sourcesVisited = 0;
	//assuming we find each source during its on period
	//after we have found three different sources on
	//the three minutes should be up and we can go to
	//the best source
	
	while(charging()!=2){
	
		while(sourcesVisited<3){//only one source on

			gotoCorner(ELECTRIC);//go to the electric source, this is the default place to go
			if(charging()==1){
				sourcesVisited++;
				while(charging()==1);//does nothing but charge so long as the source is on and not fully charged
			}

			if(light source on){
				gotoCorner(LIGHT);
				lowerPanel();//lower solar panel
				
				if(charging()==1){
					sourcesVisited++;
					while(charging()==1);
				}
				
				raisePanel();
			}


			else{
				gotoCorner(FAN);
				
				if(charging()==1){
					sourcesVisited++;
					while(charging())==1;
				}
			}
		}
	}
	
	//3 minutes have passed, all sources on
	//want to go to the best
	
	gotoCorner(BEST_SOURCE);
	while(charging()!=2);//charge until fully charged
	
	
	//fully charged
	gotoCorner(FLAG);
	*/

}

//
//Don't put any important code in main
//Use main freely to test functions and stuff
//
//For competition main should simply call init and run
//
int main(void) {	
	init();
<<<<<<< HEAD
	for(ever) run();
=======
	for (;;) UARTprintf("HI");//run();
>>>>>>> 60ecd2ff0a09f357b8881ea362ae24911ae57353
}


