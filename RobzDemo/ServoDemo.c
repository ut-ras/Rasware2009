#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#include "RASLib/servo.h"

#include "RobzDemo.h"

void initServo(void) {
	InitializeServos();
}

void servoDemo(void) {
	UARTprintf("Press:\nw-move 'up'\ns-move 'down'\nenter-quit\n");
	
	{
		unsigned char position = 0;
		char newline = 10;
		char ch;
		
		UARTprintf("position: %u\n",position);
		ch = getc();
		while(ch != newline) {
			ch = getc();
			if (ch == 'w')
				position++;
			else if (ch == 's')
				position--;
			SetServoPosition(SERVO_0, position);
			UARTprintf("position: %u",position);
		}
	}
}
