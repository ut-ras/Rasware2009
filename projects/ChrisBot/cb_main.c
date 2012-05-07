#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "driverlib/gpio.h"

#include "RASLib/init.h"
#include "RASLib/timer.h"

#include "cb_adc.h"
#include "cb_compass.h"
#include "cb_sonar.h"
#include "cb_led.h"
#include "cb_encoder.h"

unsigned char a0,a1,a2,c = 0;

void EncoderCallback(signed long * data) {
	a2=!a2;
	LED_Set(LED_1,a2?LED_ON:LED_OFF);
}

void ADCCallback(unsigned char * data) {
	a0=!a0;
	LED_Set(LED_2,a0?LED_ON:LED_OFF);
	ADC_Background_Read(&ADCCallback);
}

void SonarCallback(unsigned long data) {
	a1=!a1;
	LED_Set(LED_3,a1?LED_ON:LED_OFF);
	Sonar_Background_Read(&SonarCallback);
}

int main(void) {
	LockoutProtection();
	InitializeMCU();
	
	//init uart
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);				
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);	
	UARTStdioInit(0);
	
	ADC_Init();
	Sonar_Init();
	Encoder_Init(true,false);
	LED_Init();
	
	UARTprintf("a");
	ADC_Background_Read(&ADCCallback);
	UARTprintf("s");
	Sonar_Background_Read(&SonarCallback);
	UARTprintf("e");
	Encoder_Background_Read(&EncoderCallback);
	
	for (;;) {
		UARTprintf("ADC[%3d %3d %3d %3d %3d %3d %3d %3d]  S[%7d]  E[%3d %3d]  c:%d\n",
			ADC_Values[0],ADC_Values[1],ADC_Values[2],ADC_Values[3],ADC_Values[4],ADC_Values[5],ADC_Values[6],ADC_Values[7],
			Sonar_Value,
			Encoder_Values[0],Encoder_Values[1],
			c
		);
		LED_Set(LED_0,c);
		//LED_Set(LED_1,c+64);
		//LED_Set(LED_2,c+128);
		//LED_Set(LED_3,c+192);
		c++;
		WaitUS(20000);
	}
}

