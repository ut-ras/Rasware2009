#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#include "RASLib/encoder.h"

#include "RobzDemo.h"

void initEncoders(void) {
	InitializeEncoders(false, false);
}

void encoderDemo(void) {
	UARTprintf("Press:\nany key-read encoder values\n");
	UARTprintf("any key after read begins-quit\n");
	
	{
		signed long encoder0, encoder1;
		PresetEncoderCounts(0, 0);
		while(!keyWasPressed()) {
			GetEncoderCounts(&encoder0, &encoder1);
			UARTprintf("enc0:%l\tenc1:%l\r",encoder0,encoder1);
		}
	}
}
