#include "RASLib/motor.h"
#include "travel.h"

void travelInit(void) {
    //motors
	InitializeMotors(false, false);
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