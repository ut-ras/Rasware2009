#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "charging.h"
#include "driverlib/sysctl.h"

enum {NOT_CHARGING,CHARGING} charging;


void InitializeCharge(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC);
	ADCSequenceConfigure(ADC_BASE,0, ADC_TRIGGER_PROCESSOR, 1);
	ADCSequenceStepConfigure(ADC_BASE, 0, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0);
	ADCSequenceEnable(ADC_BASE, 0);
	charging = NOT_CHARGING;
}

long GetAnalog(int port)
{
	unsigned long ulADCValue;
	ADCProcessorTrigger(ADC_BASE, port ); 
	while(!ADCIntStatus(ADC_BASE, port, false)); 
	ADCSequenceDataGet(ADC_BASE, port, &ulADCValue);
	return ulADCValue;
}


void UpdateCapState(void){
	static int last3=0,last2=0,last1=0,last0=0;
	last3=last2;
	last2=last1;
	last1=last0;
	last0=GetAnalog(0);

	#define CHARGING_THRESHOLD 20
	if( ( (last2-last3) + (last1-last2) + (last0-last1) ) > CHARGING_THRESHOLD ){
		 charging = CHARGING;
	}
	else{
		 charging = NOT_CHARGING;
	}
}

tBoolean Charging(void){
	return charging;
}

int GetCapacitorCharge(void){
	return GetAnalog(0);
}


int charge(void){
  //returns 0 if not charging (source is not on) 
	//1 if fully charged
	return 0;	
}




