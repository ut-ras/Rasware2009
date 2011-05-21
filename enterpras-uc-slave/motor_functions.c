/*
 * motor_functions.c
 * Provides functions for control of the motors.
 * Author: Joshua James, Nicu Stiurca, Cruz Monrreal II
 *
 */
 
#include "inc/hw_types.h"		// tBoolean
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART

#include "RASLib/timer.h"
#include "RASLib/servo.h"
#include "RASLib/init.h"

#include "motor_functions.h"
#include "definitions.h"
#include "settings.h"
#include "utils.h"
#include "common.h"

/*void setMotorSpeeds(signed short left_speed, signed short right_speed, signed short angle)
{
	char left_speed_output = 127 - SATURATE(left_speed, -128, 127);
	char right_speed_output = 127 - SATURATE(right_speed, -128, 127);
	char angle_output = SATURATE(angle, -128, 127);//SATURATE(angle, -MAX_TURN_ANGLE, MAX_TURN_ANGLE) * (127 / MAX_TURN_ANGLE);
	
	if(drive_type == ACKERMANN) 
	{
		//TODO?: modulate left and right speeds based on angle to provide electronic differential and / or skid protection
		
		desired_steering_angle = angle_output;
		//Wait(100); //wait for the front wheels to turn to the correct angle before we try to drive----TODO: make this proportional to the angle it needs to travel through
	}
	
	SetServoPosition(LEFT_JAGUAR, left_speed_output);
	SetServoPosition(RIGHT_JAGUAR, right_speed_output);
	
	//Wait(10);
}*/

void SetJaguarVoltage(servo_t device, signed char voltage) //for servo_t device, use port labels defined in definitions.h
{
	if(voltage != 0)
	{
		UARTprintf("Setting jaguar voltage to %d.\n", voltage);
	}
	SetServoPosition(device, 127-voltage);
}

void joyDrive(NunchuckData joy_data)
{
	signed short l_vel;
	signed short r_vel;
    int xpow;
	int ypow;
	signed short speed;
	signed short angle;
	
	if(joy_data.valid) //if we were given valid data, use it.
	{
		xpow = (joy_data.x_position - X_CENTER);
	    ypow = (joy_data.y_position - Y_CENTER);
	}
	else
	{
		xpow = 0;
		ypow = 0;
	}
	
	if(drive_type == DIFFERENTIAL)
	{
		xpow /= 2;
		ypow /= 2;
		l_vel = (xpow - ypow) * 2;
	    r_vel = (xpow + ypow) * 2;
	    
		//keep it between -127 and +127
		l_vel = l_vel > 127 ? 127 : l_vel;
	    r_vel = r_vel > 127 ? 127 : r_vel;
	    l_vel = l_vel < -127 ? -127 : l_vel;
	    r_vel = r_vel < -127 ? -127 : r_vel;
		
		if(joy_data.c_button == 0) 
		{
			l_vel *= (6 / battery_voltage);
			r_vel *= (6 / battery_voltage);
		} 
		else //if C button is pressed, enter turbo mode
		{
			l_vel *= (12 / battery_voltage);
			r_vel *= (12 / battery_voltage);
		}
		
		//correct for jaguar silliness
		l_vel = 127 - l_vel; 
		r_vel = 127 - r_vel;
	    
		//output new velocity to motors
		//SetServoPosition(LEFT_JAGUAR, (unsigned char) l_vel);
		//SetServoPosition(RIGHT_JAGUAR, (unsigned char) r_vel);
		
		angle = angle; //getting rid of warnings
		
		//setMotorSpeeds(l_vel, r_vel, 0);												//UNCOMMENT ME!!
	}
	else if(drive_type == ACKERMANN)
	{
		angle = xpow * (MAX_TURN_ANGLE / 127); //xpow is between -127 and 127, so angle is between -MAX_TURN_ANGLE and +MAX_TURN_ANGLE
		
		if(joy_data.c_button == 0) 
		{
			speed = ypow * (6 / battery_voltage);
		} 
		else //if C button is pressed, enter turbo mode
		{
			speed = ypow * (12 / battery_voltage);
		}
		
		//correct for jaguar silliness
		speed = 127 - speed;
		
		//SetServoPosition(LEFT_JAGUAR, speed);
		//SetServoPosition(RIGHT_JAGUAR, speed);
		//SetServoPosition(STEERING_SERVO, angle);
		//setMotorSpeeds(speed, speed, angle);							//UNCOMMENT ME!!
	}
	
    Wait(100);
}

void pidSteeringServo(signed long desired_angle, signed long actual_angle)
{
	float pTerm;
    float dTerm;
    float iTerm;
	float factor;
	float angle_error;
	
	//UARTprintf("wtfz");
		desired_angle *= 8;
	angle_error = desired_angle-actual_angle; //negative is too far right, positive is too far left
	
	if(angle_error * angle_error < 100)
	{
		SetServoPosition(STEERING_JAGUAR, 127);
		return;
	}
	
    
    //Proportional
    pTerm = steeringPID.pGain * angle_error;
    
    //Integral
    steeringPID.iState += angle_error;
	steeringPID.iState = SATURATE(steeringPID.iState, steeringPID.iMin, steeringPID.iMax); //keep iState between the min and max angles it could be
    iTerm = steeringPID.iGain * steeringPID.iState;
    
    //Derivative
    dTerm = steeringPID.dGain * (desired_angle - steeringPID.dState); 
    steeringPID.dState = actual_angle;
    
    factor = SATURATE(pTerm + iTerm - dTerm, steeringPID.iMin, steeringPID.iMax);
	
	
	//UARTprintf("PIDing steering\n");
	
	//UARTprintf("\ndesired:\t%d\n", (int) desired_angle);
	UARTprintf("current:\t%d\n", actual_angle);
	//UARTprintf("error:\t\t%d\n", (int) angle_error);
	//UARTprintf("pTerm:\t\t%d\n", (int) pTerm);
	//UARTprintf("iTerm:\t\t%d\n", (int) iTerm);
	//UARTprintf("dTerm:\t\t%d\n", (int) dTerm);
	//UARTprintf("Factor:\t\t%d\n", (int) factor);
	
	//UARTprintf("pGain:\t\t%d\n", (int) (steeringPID.pGain));
	//UARTprintf("iGain:\t\t%d\n", (int) (steeringPID.iGain));
	//UARTprintf("dGain:\t\t%d\n", (int) (steeringPID.dGain));
	
	SetServoPosition(STEERING_JAGUAR, (int) 127.0 - factor);
}

void pidSteeringServoDumb(signed long desired_angle, signed long actual_angle)
{
	signed long pTerm;
    signed long dTerm;
    signed long iTerm;
	signed long factor;
	signed long angle_error;
	
	desired_angle *= 8;
	angle_error = desired_angle-actual_angle; //negative is too far right, positive is too far left
    
    //Proportional
    pTerm = (steeringPID.pGain * angle_error) / 1000;
    /*
desired:        0
current:        950
error:          -950
pTerm:          4294017
iTerm:          4294713
dTerm:          4292117
Factor:         127
*/
    //Integral
    steeringPID.iState += angle_error;
	steeringPID.iState = SATURATE(steeringPID.iState, steeringPID.iMin, steeringPID.iMax); //keep iState between the min and max angles it could be
    iTerm = (steeringPID.iGain * steeringPID.iState) / 1000;
    
    //Derivative
    dTerm = (steeringPID.dGain * (desired_angle - steeringPID.dState)) / 1000; 
    steeringPID.dState = actual_angle;
    
    factor = SATURATE(pTerm + iTerm - dTerm, steeringPID.iMin, steeringPID.iMax);
	
	
	//UARTprintf("PIDing steering\n");
	
	UARTprintf("\ndesired:\t%d\n", (int) desired_angle);
	UARTprintf("current:\t%d\n", (int) actual_angle);
	UARTprintf("error:\t\t%d\n", (int) angle_error);
	UARTprintf("pTerm:\t\t%d\n", pTerm);
	UARTprintf("iTerm:\t\t%d\n", iTerm);
	UARTprintf("dTerm:\t\t%d\n", dTerm);
	UARTprintf("Factor:\t\t%d\n", factor);
	
	UARTprintf("pGain:\t\t%d\n", (int) (steeringPID.pGain));
	UARTprintf("iGain:\t\t%d\n", (int) (steeringPID.iGain));
	UARTprintf("dGain:\t\t%d\n", (int) (steeringPID.dGain));
	
	SetServoPosition(STEERING_JAGUAR, (int) 127.0 - factor);//127-factor);
}
