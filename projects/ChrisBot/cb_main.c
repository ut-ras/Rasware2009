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
#include "cb_motor.h"
#include "cb_jumper.h"

#include "cb_travel.h"

#define IR_LEFT        6
#define IR_RIGHT       3
#define IR_FRONT_LEFT  5
#define IR_FRONT_RIGHT 4
#define IR_LONG_LEFT   2
#define IR_LONG_RIGHT  0
#define IR_FRONT       1

unsigned char c;

void panicir(unsigned char);
void avoidir(unsigned char * adc) {
	unsigned char offset = 
		(adc[IR_FRONT_LEFT]- adc[IR_FRONT_RIGHT])/80 + 
		(adc[IR_LONG_LEFT] - adc[IR_LONG_RIGHT] )/50;
	Travel_Offset(offset);
	LED_Set(LED_3,offset);
	
	LED_Set(LED_0,Sonar_Value/10000);
	LED_Set(LED_1,adc[IR_FRONT]);
	
	if (adc[IR_FRONT] > 100 || Sonar_Value > 1700000)
		panicir(255);
	else
		ADC_Background_Read(&avoidir);
	
	LED_Set(LED_2,c++);
}

void avoidsonar(unsigned long eh) {
	Sonar_Background_Read(&avoidsonar);
}
	
void panicir(unsigned char val) {
	if (val < 100 && Sonar_Value < 1700000) {
		Travel_Go(FULL_SPEED);
		avoidir(0);
	} else {
		ADC_Single_Background_Read(IR_FRONT,&panicir);
	}
}



int main(void) {
	LockoutProtection();
	InitializeMCU();
	
	//init uart
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);				
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);	
	UARTStdioInit(0);
	
	LED_Init();
	Jumper_Init();
	
	ADC_Init();
	Sonar_Init();
	
	Travel_Init(Jumper_Value & 0x8);
	
	if ((Jumper_Value & 0x7) == 0x1) {
		Travel_Go(FULL_SPEED);
		avoidsonar(0);
		avoidir(0);
		for (;;);
	}
	
	//if no jumpers are set, enter debug mode
	
	for (;;c++) {
		ADC_Background_Read(0);
		Sonar_Background_Read(0);
		Encoder_Background_Read(0);
		Jumper_Read();
		
		UARTprintf("ADC[%3d %3d %3d %3d %3d %3d %3d %3d]  S[%7d]  E[%3d %3d]  J[%1x] c:%d\n",
			ADC_Values[0],ADC_Values[1],ADC_Values[2],ADC_Values[3],ADC_Values[4],ADC_Values[5],ADC_Values[6],ADC_Values[7],
			Sonar_Value,
			Encoder_Values[0],Encoder_Values[1],
			Jumper_Value,
			c
		);
		
		LED_Set(LED_0,c);
		LED_Set(LED_1,c+64);
		LED_Set(LED_2,c+128);
		LED_Set(LED_3,c+192);

		WaitUS(20000);
	}
}

