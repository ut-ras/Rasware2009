/*
 * adc_functions.c
 * Provides functions for control of devices attached to the adc ports.
 * Author: Joshua James
 *
 */
 

#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"		// input/output over UART
#include "driverlib/adc.h"
#include "driverlib/sysctl.h"
#include "settings.h"
#include "definitions.h"
 
 void initADC(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC);
	
	ADCSequenceConfigure(ADC0_BASE,POTENTIOMETER, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, POTENTIOMETER, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0);
	ADCSequenceEnable(ADC0_BASE, POTENTIOMETER);
	
	/*
	ADCSequenceConfigure(ADC0_BASE,3, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH1);
	ADCSequenceEnable(ADC0_BASE, 3);
	*/
}

signed char getCurrentSteeringAngle(void)
{
	unsigned long potValue;
	
	ADCProcessorTrigger(ADC0_BASE, POTENTIOMETER); 				//ask for pot.
	while(!ADCIntStatus(ADC0_BASE, POTENTIOMETER, false)); 		//wait 'till we get it.
	ADCSequenceDataGet(ADC0_BASE, POTENTIOMETER, &potValue);	//once we have it,
	potValue = potValue * 1; 									//prepare it for consumption
	
	return potValue;											//and distribute it!
}
