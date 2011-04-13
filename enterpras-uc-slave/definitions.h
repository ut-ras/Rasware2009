/*
 * definitions.h
 * Provides definitions and structs used throughout the code.
 * Author: Joshua James
 *
 */


#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "inc/hw_types.h"		// tBoolean

//==========COMM DEFINITIONS==========
//various offset values in bytes
#define CODE_START		1

#define LENGTH1			6
#define DATA1			8

#define LENGTH2			6
#define DATA2			8

//==========ROBOT DEFINITIONS==========
//control_modes
#define AUTONOMOUS 		0x00
#define NUNCHUCK 		0x01

//drive_types
#define ACKERMANN 		0x00
#define DIFFERENTIAL 	0x01

//physical constants
#define MAX_TURN_ANGLE	75	//maximum turning angle of the front wheels in degrees (MUST BE LESS THAN 127)

//connections
#define LEFT_JAGUAR 	SERVO_0
#define RIGHT_JAGUAR 	SERVO_1
#define STEERING_SERVO 	SERVO_2
#define LIDAR_SERVO		SERVO_3

#define DRIVE_JAGUAR 	SERVO_0 //if only one drive motor is used (as in the case of a mechanical differential), plug it in to servo port 0

#define POTENTIOMETER	0

//==========TIMER DEFINITIONS==========
#define WATCHDOG_PERIOD 50000 * g_ulTicksPerUs //robot will stop moving within 50ms of receiving no heartbeat

//==========I2C DEFINITIONS==========
#define codeSelect 		GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5) >> 2

//==========CHAR PROCESSING DEFINITIONS==========
#define isNum(x)		(x>='0' && x<='9')
#define isChar(x)		((x>='a' && x<='z') || (x>='A' && x<='Z'))
#define isHex(x)		((x>='a' && x<='f') || (x>='A' && x<='F')) || isNum(x)
#define charToByte(x)	(isNum(x) ? x-'0': ((x>='a' ? x-'a': x-'A')+10))

//==========WII NUNCHUCK DEFINITIONS==========
typedef struct NunchuckData 
{
	short x_position;
	short y_position;
	short x_acceleration;
	short y_acceleration;
	short z_acceleration;
	short c_button;
	short z_button;
	tBoolean valid;
} NunchuckData;

#define X_CENTER 128
#define Y_CENTER 128

//==========GENERAL CODES==========
#define MAINTAIN_STEERING_ANGLE -32768

//==========PID STRUCT=============
typedef struct
{
    double dState; // Last position input
    double iState; // Integrator state
    double iMin; // Minimum allowable integrator state
    double iMax; // Maximum allowable integrator state
    double iGain; // integral gain
    double pGain; // proportional gain
    double dGain; // derivative gain
} PIDdata;

#endif
