#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

#include "utils/uartstdio.h"	// input/output over UART

#include "driverlib/uart.h"		// input/output over UART
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/adc.h"
#include "driverlib/interrupt.h"

#include "RASLib/init.h"


#define InitializeUART()																			\
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);								\
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);	\
	UARTStdioInit(0);
	
	
volatile tBoolean power;
volatile unsigned long forward;
volatile unsigned long sideward;


void init_motors(tBoolean inv0, tBoolean inv1) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
	GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	
	PWMGenConfigure(PWM_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenPeriodSet(PWM_BASE, PWM_GEN_2, 256);
	PWMOutputInvert(PWM_BASE, PWM_OUT_4_BIT, inv0);
	PWMOutputInvert(PWM_BASE, PWM_OUT_5_BIT, inv1);
	PWMOutputState(PWM_BASE, PWM_OUT_4_BIT | PWM_OUT_5_BIT, true);
	PWMPulseWidthSet(PWM_BASE, PWM_OUT_4, 0);
	PWMPulseWidthSet(PWM_BASE, PWM_OUT_5, 0);
	PWMGenEnable(PWM_BASE, PWM_GEN_2);
	
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_7);
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_5);
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0x00);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0x00);
}

void set_motors(signed char m0, signed char m1) {
	PWMPulseWidthSet(PWM_BASE, PWM_OUT_4, m0+128);
	PWMPulseWidthSet(PWM_BASE, PWM_OUT_5, m1+128);
}

void init_input() {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_7);
	GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_7, GPIO_BOTH_EDGES);
	GPIOPinIntEnable(GPIO_PORTD_BASE, GPIO_PIN_7);
	IntEnable(INT_GPIOD);
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC);
	ADCSequenceConfigure(ADC_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC_BASE, 0, 0, ADC_CTL_CH0);
	ADCSequenceStepConfigure(ADC_BASE, 0, 1, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH1);
	ADCIntEnable(ADC_BASE, 0);
	IntEnable(INT_ADC0SS0);
	ADCSequenceEnable(ADC_BASE, 0);
	ADCProcessorTrigger(ADC_BASE, 0);
}

void gpiod_handler() {
	GPIOPinIntClear(GPIO_PORTD_BASE, GPIO_PIN_7);
	power = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_7) >> 7;
}

void adc_handler() {
	unsigned long temp[8];
	
	ADCIntClear(ADC_BASE, 0);
	ADCSequenceDataGet(ADC_BASE, 0, temp);
	forward = temp[0];
	sideward = temp[1];
	
	ADCProcessorTrigger(ADC_BASE, 0);
}


int main() {
	LockoutProtection();
	InitializeMCU();
	InitializeUART();
	IntMasterEnable();
	
	init_motors(true, true);
	init_input();
	
	UARTprintf("- Hi I am Couch! -");
	
	for (;;) {
#ifdef DEBUG
		UARTprintf("!%d:%d:%d!\n",forward,sideward,power);
#endif
		
		if (power) {
			//input range is 0 to 1024
			//TODO set up turning
			set_motors(
				(forward>>2)-128,
				(forward>>2)-128
			);
		} else {
			set_motors(0,0);
		}
	}
}

