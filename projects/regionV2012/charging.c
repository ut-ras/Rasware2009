#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "charging.h"
#include "driverlib/sysctl.h"

void InitializeAnalog0()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC);
	ADCSequenceConfigure(ADC_BASE,0, ADC_TRIGGER_PROCESSOR, 1);
	ADCSequenceStepConfigure(ADC_BASE, 0, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0);
	ADCSequenceEnable(ADC_BASE, 0);

}

long GetAnalog(int port)
{
	unsigned long ulADCValue;
	ADCProcessorTrigger(ADC_BASE, port ); 
	while(!ADCIntStatus(ADC_BASE, port, false)); 
	ADCSequenceDataGet(ADC_BASE, port, &ulADCValue);
	return ulADCValue;
}

int GetCapacitorCharge(void){
	return GetAnalog(0);
}


int charge(void){
  //returns 0 if not charging (source is not on) 
	//1 if fully charged
	return 0;	
}




