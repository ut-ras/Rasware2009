#include "cb_sonar.h"

#include "inc/hw_types.h"
#include "utils/uartstdio.h"

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"

#include "RASLib/init.h"

#define US(x) ((x)*g_ulTicksPerUs)
#define MS(x) (US(x)*1000)
#define MAX_SONAR_TIME MS(36)

#define TIME(x) {													\
	TimerLoadSet(TIMER2_BASE,TIMER_A,(x));	\
	TimerEnable(TIMER2_BASE, TIMER_A);			\
}

volatile unsigned long Sonar_Value = 0;
volatile enum {READY,PULSE,WAIT,TIMING,DELAY} sonarstatus;
volatile tBoolean sonarwaiting = false;
void (*sonarcallback)(unsigned long);

void BeginSonarSequence(void) {
	sonarwaiting = false;
	sonarstatus = PULSE;
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2);
	TIME(US(8));
}

void SonarTimerIntHandler(void) {
	TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	if (sonarstatus == PULSE) {
		sonarstatus = WAIT;
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0);
		TIME(MAX_SONAR_TIME);
	} else if (sonarstatus == DELAY) {
		sonarstatus = READY;
		if (sonarwaiting)
			BeginSonarSequence();
	} else {
		UARTprintf("er");
		Sonar_Value = (unsigned long)-1;
		sonarstatus = READY;
		if (sonarcallback) (*sonarcallback)(Sonar_Value);
	}
}

void SonarGPIOIntHandler(void) {
	GPIOPinIntClear(GPIO_PORTD_BASE, GPIO_PIN_3);
	if (GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_3)) {
		sonarstatus = TIMING;
		TIME(MAX_SONAR_TIME);
	} else {
		Sonar_Value = TimerValueGet(TIMER2_BASE, TIMER_A);
		sonarstatus = DELAY;
		if (sonarcallback) (*sonarcallback)(Sonar_Value);
		TIME(MS(10));
	}
}


void Sonar_Init(void) {
	// initialize gpio
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	GPIOPinTypeGPIOInput (GPIO_PORTD_BASE, GPIO_PIN_3);
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_2);
	
	GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_BOTH_EDGES);
	GPIOPinIntEnable(GPIO_PORTD_BASE, GPIO_PIN_3);
	IntEnable(INT_GPIOD);
	
	//initialize timer
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
	TimerConfigure(TIMER2_BASE, TIMER_CFG_32_BIT_OS);
	
	IntEnable(INT_TIMER2A);
	TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	
	sonarstatus = READY;
}

unsigned long Sonar_Read(void) {
	Sonar_Background_Read(0);
	while(sonarstatus != DELAY && sonarstatus != READY);
	return Sonar_Value;
}

void Sonar_Background_Read(void (*cb)(unsigned long)) {
	sonarcallback = cb;
	if (sonarstatus == READY) {
		BeginSonarSequence();
	} else if (sonarstatus == DELAY) {
		sonarwaiting = true;
	}
}
