#include "inc/hw_types.h"		// tBoolean
#include "RASLib/motor.h"
#include "travel.h"


// Current location of robot
signed char currentCorner = TREE;
bool backwards = 0;
unsigned char lMotorSpeed = 0;
unsigned char rMotorSpeed = 0;

#define BACK_LEFT 0
#define FRONT_LEFT 1
#define FRONT 2
#define FRONT_LEFT 3
#define BACK_RIGHT 4
#define BACK 5

#define BACK_OFFSET (backwards?3:0)
#define till(x) for(ADS7830_BurstRead(x);sensors[x]<sensor_bounds[x];ADS7830_BurstRead(x))
#define tillnot(x) for(ADS7830_BurstRead(x);!(sensors[x]<sensor_bounds[x]);ADS7830_BurstRead(x))

// Array for reading sensors
unsigned char[8] sensors;

const unsigned char[6] sensor_bounds = {400,400,400,400,400,400};


void travelInit(void) {
	//motors
	InitializeMotors(false,false);
	ADS7830_Init();
}

void goForward(void) {
	SetMotorPowers(127,127);
}

void goBackward(void) {
	SetMotorPowers(-128,-128);
}

void goCorner(void) {
	goWall();
}

void goWall(void) {
	SetMotorPowers(-128,-128);

}

void gotoCorner(signed char dest) {
	if (dest<0 || dest==currentCorner) return;

	if (currentCorner==TREE) {
		till(FRONT) goForward();
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


