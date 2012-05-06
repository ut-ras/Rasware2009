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


unsigned char c = 0;

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
	
	for (;;) {
		ADC_Read();
		Sonar_Read();
		UARTprintf("ADC[%3d %3d %3d %3d %3d %3d %3d %3d]  S[%7d]  E[%3d %3d]  c:%d\n",
			ADC_Values[0],ADC_Values[1],ADC_Values[2],ADC_Values[3],ADC_Values[4],ADC_Values[5],ADC_Values[6],ADC_Values[7],
			Sonar_Value,
			Encoder_Values[0],Encoder_Values[1],
			c
		);
		LED_Set(LED_0,c);
		LED_Set(LED_1,c+64);
		LED_Set(LED_2,c+128);
		LED_Set(LED_3,c+192);
		c++;
		WaitUS(100000);
	}
}

