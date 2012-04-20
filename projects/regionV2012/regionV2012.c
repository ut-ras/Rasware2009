#include "inc/hw_types.h"		// tBoolean
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "RASLib/init.h"
#include "RASLib/motor.h"
#include "utils/uartstdio.h"

#define let init();
#define the for
#define monkeys (;;)
#define roll run();
#define CHARGING_THRESHOLD 20


#include "travel.h"
#include "charging.h"
#include "panel.h"
#include "fan.h"
#include "ADS7830.h"
#include "clock.h"

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

	//Initialize Franks interrupt
	//InitializeGPIOIntTest();
	
	//initPanelServos();
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
	
	int capCharge = GetCapacitorCharge();
	
	
	while(sourcesVisited<2&&capCharge<CHARGING_THRESHOLD){
		while(sourcesVisited<3){//first three minutes, only one source on
			
			//findWall
			
			
			
			if(NoiseDetection()){//NoiseDetection returns an int; not sure what value should be around when fan is on
				gotoCorner(FAN);
				charge(FAN);
				if(capCharge<GetCapacitorCharge()){
					sourcesVisited++;
					
					while(capCharge<GetCapacitorCharge())//charge until source turns off
						capCharge=GetCapacitorCharge();
				}
			}
				
			
			else{
				gotoCorner(ELECTRIC)
				charge(ELECTRIC);
				if(capCharge<GetCapacitorCharge()){
					sourcesVisited++;
					
					while(capCharge<GetCapacitorCharge())
						capCharge=GetCapacitorCharge();
				}
				
				/*if(isLightOn()){
					gotoCorner(LIGHT);
					lowerPanel();
					charge(LIGHT)
					
					if(capCharge<GetCapacitorCharge()){
						sourcesVisited++;
					
						while(capCharge<GetCapacitorCharge())
							capCharge=GetCapacitorCharge();
					}
					
					raisePanel();
				}*/	
		/*	}
	
		//3 minutes have passed, all sources on
		//want to go to the best
		gotoCorner(BEST_SOURCE);
		while(capCharge<GetCapacitorCharge())
			capCharge=GetCapacitorCharge();
		
		
		}
		//fully charged
		gotoCorner(FLAG);
		charge(FLAG);
		
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
	goForward();
	UARTprintf("go monkeys");
	InitializeCharge();
	//ADS7830_Init();
	InitializeClock();

	WallFollow(2,10000,1);//goWall();	
	BackOut(); 
	WallFollow(0,0,1);  
	//isFanTripped();
	for (;;);
	//UARTprintf("you shouldn't get here..");
}


