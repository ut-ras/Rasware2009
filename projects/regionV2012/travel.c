#include "inc/hw_types.h"		// tBoolean
#include "RASLib/motor.h"
#include "travel.h"


// Current location of robot
signed char currentCorner = TREE;

unsigned short *readSensors(void) {
  static unsigned short values[8] = {100,100,100,100,100,100,100,100};
  //return an array of short values for the sensors
  return &values[0];  //values instead of &values?
}

void travelInit(void) {
    //motors
	InitializeMotors(false,false);
}

void gotoCorner(signed char dest) {
	if (dest<0 || dest==currentCorner) return;

	if (currentCorner==TREE) {
		//just move to electric source
		currentCorner = ELECTRIC;
		if (dest != currentCorner) gotoCorner(dest);
		return;
	}
	
	//TODO go there	

	currentCorner = dest;
}
