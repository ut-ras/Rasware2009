#include <stdlib.h>				// malloc & free
#include <stdarg.h>				// va_---- functions
#include <string.h>				// String functions
#include <math.h>
#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"		// input/output over UART
#include "inc/hw_ints.h"		// input/output over UART
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "driverlib/gpio.h"		// input/output over UART
#include "driverlib/adc.h"
#include "driverlib/interrupt.h"
#include "RASLib/init.h"
#include "RASLib/timer.h"		// input/output over UART
#include "RASLib/servo.h"
#include "RASLib/motor.h"


void (*UART1RXHandler)(unsigned char) = 0;

void UART0IntHandler(void){
	volatile unsigned long ulStatus = UARTIntStatus(UART0_BASE, true);
	
	while(UARTCharsAvail(UART0_BASE))
		UARTCharPut(UART0_BASE, UARTCharGet(UART0_BASE));

  UARTIntClear(UART0_BASE, ulStatus);
}

void UART0Write(unsigned char c){ UARTCharPut(UART0_BASE, c); }


void UART1IntHandler(void){
	volatile unsigned long ulStatus = UARTIntStatus(UART1_BASE, true);
	
	while(UARTCharsAvail(UART1_BASE))
		UART1RXHandler(UARTCharGet(UART1_BASE));
	
	UARTIntClear(UART1_BASE, ulStatus);
}

void UART1Write(unsigned char c){ UARTCharPut(UART1_BASE, c); }

void PrintHexToAscii(unsigned char c){
		UART0Write('0');
		UART0Write('x');
		UART0Write((c >> 4) <= 9 ? (c>>4) + 0x30 : (c>>4) + 0x41 - 10);
		UART0Write((c & 0xF) < 0xA ? (c&0xF) + 0x30 : (c&0xF) + 0x41 - 10);
}

volatile unsigned char XBeeRXPtr = 6;
static unsigned char XBeeRXBuff[7];

// Handle XBee ACK Msg
void XBeeRXHandler(unsigned char data){
	if (XBeeRXPtr != 6){
		XBeeRXBuff[++XBeeRXPtr] = data;
	}else if (data == 0x7E){
		XBeeRXPtr = 0;
		XBeeRXBuff[0] = data;
	}
	
	if (XBeeRXPtr == 6){ 	// Check Msg
		if (0xFF - ((XBeeRXBuff[3] + XBeeRXBuff[4] + XBeeRXBuff[5]) & 0xFF) != XBeeRXBuff[6]){
			PrintHexToAscii(XBeeRXBuff[4]);
			UART0Write(':');
			UART0Write('N');
			UART0Write('\r');
			UART0Write('\n');
		}
	}
}

static unsigned char apiID=1;
unsigned char XBeeBuff[32];
unsigned char XBeeBuffSize = 0;
void XBeeWrite(unsigned char c){
	if (XBeeBuffSize < 32)
		XBeeBuff[XBeeBuffSize++] = c;
}
void XBeeSend(){
	unsigned char chk = 0, tmp = 0;
	
	UART1Write(0x7E);	// Delim
	
	UART1Write(((XBeeBuffSize + 5) >> 8) & 0xFF);	// Data Length
	UART1Write((XBeeBuffSize + 5) & 0xFF);
	
	chk += 0x01;	// API
	chk += apiID; // API Frame ID
	chk += 0x00;  // Destination
	chk += 0x00;	
	chk += 0x00;	// Option Byte
	UART1Write(0x01);
	UART1Write(apiID++);
	UART1Write(0x00);
	UART1Write(0x00);
	UART1Write(0x00);
	
	while (XBeeBuffSize - tmp){
		chk += XBeeBuff[tmp];
		UART1Write(XBeeBuff[tmp++]);
	}
	
	UART1Write(0xFF - chk);
	
	XBeeBuffSize = 0;	// Reset Buffer
}



unsigned long adcValues[4];
unsigned char adcValuesPtr;

void ADCSeq0IntHandler(){
	volatile unsigned long ulStatus = ADCIntStatus(ADC0_BASE, 0, 0);
	
	ADCSequenceDataGet(ADC0_BASE, 0, adcValues);
	
	ADCIntClear(ADC0_BASE, 0);
}




int main(void){
  LockoutProtection();
	InitializeMCU();
	
	/*
	// Init GPIO
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	GPIODirModeSet(GPIO_PORTD_BASE, GPIO_PIN_7, GPIO_DIR_MODE_IN);
	GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_7, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
	*/
	
	// Init UART0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 921600, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	IntEnable(INT_UART0);
	UARTIntEnable(UART0_BASE, UART_INT_RX);
	
	
	// Init UART1
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
	GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_3);
	UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	UART1RXHandler = &XBeeRXHandler;	// Handle XBee Response Packets
	IntEnable(INT_UART1);
	UARTIntEnable(UART1_BASE, UART_INT_RX);
	
	
	// Init ADC
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0);
	ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH1);
	ADCSequenceStepConfigure(ADC0_BASE, 0, 2, ADC_CTL_CH2);
	ADCSequenceStepConfigure(ADC0_BASE, 0, 3, ADC_CTL_CH3 | ADC_CTL_IE | ADC_CTL_END);
	ADCHardwareOversampleConfigure(ADC0_BASE, 8);
	
	ADCIntEnable(ADC0_BASE, 0);
	IntEnable(INT_ADC0SS0);
	ADCSequenceEnable(ADC0_BASE, 0);
	

	// Enable All Interrupts
	IntMasterEnable();
	
	
	while(1){
		ADCProcessorTrigger(ADC0_BASE, 0);
		Wait(10);
		
		XBeeWrite(adcValues[0] >> 8);		// ADC is 10 bit
		XBeeWrite(adcValues[0]);
		XBeeWrite(adcValues[1] >> 8);
		XBeeWrite(adcValues[1]);
		XBeeWrite(adcValues[2] >> 8);
		XBeeWrite(adcValues[2]);
		XBeeWrite(adcValues[3] >> 8);
		XBeeWrite(adcValues[3]);
		XBeeSend();
	}
}
