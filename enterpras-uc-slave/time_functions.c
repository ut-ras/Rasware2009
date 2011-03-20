/*
 * time_functions.c
 * Provides functions for watchdog timeouts and timestamps.
 * Author: Joshua James, Nicu Stiurca
 *
 */
 
#include <stdlib.h>				// malloc & free
#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

//for watchdog
#include "inc/hw_ints.h"		
#include "driverlib/watchdog.h"
#include "driverlib/interrupt.h"

#include "RASLib/init.h"
#include "RASLib/servo.h"

#include "motor_functions.h"
#include "time_functions.h"
#include "definitions.h"
#include "settings.h"

void resetWatchdogTimer(void)
{
	WatchdogReloadSet(WATCHDOG0_BASE, WATCHDOG_PERIOD);
	PWMOutputState(PWM_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT | PWM_OUT_2_BIT, false);
	control_mode = AUTONOMOUS;
	PWMOutputState(PWM_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT | PWM_OUT_2_BIT, true);
}

void WatchdogIntHandler(void)
{
	WatchdogIntClear(WATCHDOG0_BASE);
	PWMOutputState(PWM_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT | PWM_OUT_2_BIT, false);
	control_mode = NUNCHUCK;
	PWMOutputState(PWM_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT | PWM_OUT_2_BIT, true);
	//setMotorSpeeds(0,0,MAINTAIN_STEERING_ANGLE);
}

void initWatchdog()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);
	IntEnable(INT_WATCHDOG);
	resetWatchdogTimer();
	WatchdogEnable(WATCHDOG0_BASE);
}
