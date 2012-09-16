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


#define InitializeUART()  																		\
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);								\
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);	\
	UARTStdioInit(0);
	
	
tBoolean power;
unsigned long forward;
unsigned long sideward;


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
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_7);
	
	ADCSequenceConfigure(ADC_BASE, 0, ADC_TRIGGER_ALWAYS, 0);
	ADCSequenceStepConfigure(ADC_BASE, 0, 0, ADC_CTL_CH0);
	ADCSequenceStepConfigure(ADC_BASE, 0, 1, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH1);
	ADCIntEnable(ADC_BASE, 0);
	IntEnable(INT_ADC0SS0);
	ADCSequenceEnable(ADC_BASE, 0);
}

void input_handler() {
	ADCIntClear(ADC_BASE, 0);
	ADCSequenceDataGet(ADC_BASE, 0, &forward);
	ADCSequenceDataGet(ADC_BASE, 1, &sideward);
}

void read_input() {
	power = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_7) >> 7;
	//the other inputs are updated through interrupts
}


int main() {
	LockoutProtection();
	InitializeMCU();
	InitializeUART();
	IntMasterEnable();
	
	init_motors(true, true);
	init_input();
	
	for (;;) {
		read_input();
		
		if (power) {
			set_motors(0,0);
			UARTprintf("!%d:%d!",forward,sideward);
			//UARTprintf("- Hi I am Couch! -");
			//TODO check the inputs range
		} else {
			set_motors(0,0);
		}
	}
}

