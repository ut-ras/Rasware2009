#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#include "RASLib/linesensor.h"

#include "RobzDemo.h"

void initLineSensor(void) {
	InitializeLineSensorAsync();
}

void lineSensorDemo(void) {
	UARTprintf("Press:\nany key-read line sensor\n");
	UARTprintf("any key after read begins-quit\n");
	
	while(!keyWasPressed()) {
		unsigned char lineSensorByte = ReadLineSensorAsync();
	 	int lineSensorArray[8];
		int i;
		for(i = 0; i < 8; i++) {
			lineSensorArray[i] = lineSensorByte & 0x01;
			lineSensorByte = lineSensorByte >> 1;
		}
		
 		//black is 1, white is 0
		for(i = 0; i < 8; i++) 
			UARTprintf("%u",lineSensorArray[i]);
		UARTprintf("\r");
	}
}
