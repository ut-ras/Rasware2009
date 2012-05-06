#include "cb_adc.h"

#include "inc/hw_types.h"
#include "utils/uartstdio.h"

#include "cb_i2c.h"

volatile unsigned char ADC_Values[8] = {0,0,0,0,0,0,0,0};
volatile unsigned char adcindex;
void * adccallback;
tBoolean adcsingle;

void ADC_Init(void) {
	I2C_Init();
}

status_t ADCReadHandler(unsigned char val) {
	ADC_Values[adcindex] = val;
	if (adcsingle) {
		if (adccallback) (*((void (*)(unsigned char))adccallback))(val);
	} else {
		if (++adcindex < 8) {
			I2C_Background_Request(ADS7830,0x84|(adcindex<<4),&ADCReadHandler);
		} else {
			if (adccallback) (*((void (*)(unsigned char *))adccallback))((unsigned char *)ADC_Values);
		}
	}
	
	return I2C_Status;
}

unsigned char ADC_Single_Read(unsigned char i) {
	return ADC_Values[i] = I2C_Request(ADS7830,0x84|(i<<4));
}

void ADC_Background_Single_Read(unsigned char i,void (*cb)(unsigned char)) {
	adccallback = cb;
	adcsingle = true;
	I2C_Background_Request(ADS7830,0x84|((adcindex=i)<<4),&ADCReadHandler);
}

unsigned char * ADC_Read(void) {
	ADC_Background_Read(0);
	while (adcindex < 8);
	return (unsigned char *)ADC_Values;
}

void ADC_Background_Read(void (*cb)(unsigned char *)) {
	adccallback = cb;
	adcsingle = false;
	I2C_Background_Request(ADS7830,0x84|((adcindex=0)<<4),&ADCReadHandler);
}
