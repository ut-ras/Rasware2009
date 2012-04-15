#include "inc/hw_types.h"		// tBoolean
#include "RASLib/motor.h"
#include "travel.h"


// Current location of robot
signed char currentCorner = TREE;

<<<<<<< HEAD
// Array fro reading sensors
signed char[8] sensors;

=======
unsigned short *readSensors(void) {
  static unsigned short values[8] = {100,100,100,100,100,100,100,100};
  //return an array of short values for the sensors
<<<<<<< HEAD
  return &values[0];  //values instead of &values?
=======
  return &values[0];    //values instead of &value?
>>>>>>> b672fc660f1d4c989f19fba8b90b724d4f6e6978
}
>>>>>>> 81e400b7a9951044d918231d570b7207871aeba4

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

void testSensors(void) {
	ADS7830_BurstRead(sensors);
	UARTprintf("[%d %d %d %d %d %d %d %d]",sensors[0],sensors[1],sensors[2],sensors[3],sensors[4],sensors[5],sensors[6],sensors[7]);
}


