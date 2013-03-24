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

void UART0WriteChar(unsigned char c){ UARTCharPut(UART0_BASE, c); }
void UART0Write(const unsigned char* buff, int len){
	while(len--)
		UARTCharPut(UART0_BASE, *buff++);
}


void UART1IntHandler(void){
	volatile unsigned long ulStatus = UARTIntStatus(UART1_BASE, true);
	
	while(UARTCharsAvail(UART1_BASE))
		UART1RXHandler(UARTCharGet(UART1_BASE));
	
	UARTIntClear(UART1_BASE, ulStatus);
}

void PrintHexToAscii(unsigned char c){
		UART0WriteChar('0');
		UART0WriteChar('x');
		UART0WriteChar((c >> 4) <= 9 ? (c>>4) + 0x30 : (c>>4) + 0x41 - 10);
		UART0WriteChar((c & 0xF) < 0xA ? (c&0xF) + 0x30 : (c&0xF) + 0x41 - 10);
}

void UART1WriteChar(unsigned char c){ UARTCharPut(UART1_BASE, c); }
void UART1Write(const unsigned char* buff, int len){
	while(len--)
		UARTCharPut(UART1_BASE, *buff++);
}


unsigned char XBeeRXPtr = 0;
volatile unsigned char XBeeRXFrameSize = 0;
static unsigned char XBeeRXBuff[32];

unsigned char XBeeRXDataSize = 0;
static unsigned char XBeeRXData[32];

// Handle XBee RX Msg
// TODO: Use ACK Msg
void XBeeRXHandler(unsigned char data){
	if (XBeeRXPtr == 0){
		if (data == 0x7E)
			XBeeRXBuff[XBeeRXPtr++] = data;
	}else{
		XBeeRXBuff[XBeeRXPtr++] = data;
		
		if (XBeeRXPtr == 3){
			XBeeRXFrameSize = XBeeRXBuff[1] << 8 | XBeeRXBuff[2];
	
			if (XBeeRXFrameSize <= 5){	// Toss out garbage data
				XBeeRXPtr = 0;
				XBeeRXFrameSize = 0;
			}
		}else if (XBeeRXPtr == 4 && data != 0x01){	// We only care about data, not XBee status updates
			XBeeRXPtr = 0;
			XBeeRXFrameSize = 0;
		}else if (XBeeRXPtr == XBeeRXFrameSize + 3){
			unsigned char ptr;
			
			XBeeRXDataSize = 0;
			for(ptr = 8; ptr < XBeeRXFrameSize + 3; ptr++)
				XBeeRXData[XBeeRXDataSize++] = XBeeRXBuff[ptr];
			
			XBeeRXPtr = 0;
			XBeeRXFrameSize = 0;
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
void XBeeSend(unsigned short addr){
	unsigned char chk = 0, tmp = 0;
	
	UART1WriteChar(0x7E);	// Delim
	
	UART1WriteChar(((XBeeBuffSize + 5) >> 8) & 0xFF);	// Data Length
	UART1WriteChar((XBeeBuffSize + 5) & 0xFF);
	
	chk += 0x01;	// API
	chk += apiID; // API Frame ID
	chk += addr >> 8;  // Destination
	chk += addr & 0xFF;	
	chk += 0x00;	// Option Byte
	UART1WriteChar(0x01);
	UART1WriteChar(apiID++);
	UART1WriteChar(addr >> 8);
	UART1WriteChar(addr & 0xFF);
	UART1WriteChar(0x00);
	
	while (XBeeBuffSize - tmp){
		chk += XBeeBuff[tmp];
		UART1WriteChar(XBeeBuff[tmp++]);
	}
	
	UART1WriteChar(0xFF - chk);
	
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
	
	// Configure XBeeUART0Write("Configuring Xbee\r\n", 18);
	
	Wait(2000);
	UART1Write("+++", 3);
	Wait(2000);
	UART1Write("ATCHC\r", 6);
	Wait(500);
	UART1Write("ATID657\r", 8);
	Wait(500);
	UART1Write("ATDH0\r", 6);
	Wait(500);
	UART1Write("ATDLFFFF\r", 9);
	Wait(500);
	UART1Write("ATMY00\r", 7);
	Wait(500);
	UART1Write("ATCE1\r", 6);
	Wait(500);
	UART1Write("ATA10\r", 6);
	Wait(500);
	UART1Write("ATA24\r", 6);
	Wait(500);
	UART1Write("ATAPI1\r", 7);
	Wait(500);
	UART1Write("ATWR\r", 5);
	Wait(500);
	UART1Write("ATAC\r", 5);
	Wait(500);
	UART1Write("ATCN\r", 5);
	
	Wait(1000);
	

	// Enable All Interrupts
	IntMasterEnable();
	
	UART0WriteChar('\r');
	UART0WriteChar('\n');
	UART0WriteChar('\r');
	UART0WriteChar('\n');
	
	
	while(1){
		ADCProcessorTrigger(ADC0_BASE, 0);
		Wait(10);
		
		XBeeWrite(adcValues[2] >> 8);		// ADC is 10 bit
		XBeeWrite(adcValues[2]);
		XBeeWrite(adcValues[3] >> 8);
		XBeeWrite(adcValues[3]);
		XBeeSend(0xA);
		UART0WriteChar('\r');
		UART0WriteChar('\n');
		
		XBeeWrite(adcValues[0] >> 8);
		XBeeWrite(adcValues[0]);
		XBeeWrite(adcValues[1] >> 8);
		XBeeWrite(adcValues[1]);
		XBeeSend(0xB);
		UART0WriteChar('\r');
		UART0WriteChar('\n');
	}
}
