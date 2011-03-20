/*
 * motor_functions.c
 * Provides functions for control of the motors.
 * Author: Joshua James, Nicu Stiurca, Cruz Monrreal II
 *
 */
 
#include "inc/hw_types.h"		// tBoolean

#include "RASLib/timer.h"
#include "RASLib/servo.h"
#include "RASLib/init.h"

#include "motor_functions.h"
#include "definitions.h"
#include "settings.h"

void setMotorSpeeds(signed short left_speed, signed short right_speed, signed short angle)
{
	char left_speed_output = 127 - SATURATE(left_speed, -128, 127);
	char right_speed_output = 127 - SATURATE(right_speed, -128, 127);
	char angle_output = SATURATE(angle, -MAX_TURN_ANGLE, MAX_TURN_ANGLE) * (127 / MAX_TURN_ANGLE);
	
	if(drive_type == ACKERMANN) 
	{
		//TODO?: modulate left and right speeds based on angle to provide electronic differential and / or skid protection
		
		SetServoPosition(STEERING_SERVO, angle_output);
		Wait(100); //wait for the front wheels to turn to the correct angle before we try to drive----TODO: make this proportional to the angle it needs to travel through
	}
	
	SetServoPosition(LEFT_JAGUAR, left_speed_output);
	SetServoPosition(RIGHT_JAGUAR, right_speed_output);
	
	Wait(10);
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
		SetServoPosition(LEFT_JAGUAR, (unsigned char) l_vel);
		SetServoPosition(RIGHT_JAGUAR, (unsigned char) r_vel);
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
		
		SetServoPosition(LEFT_JAGUAR, speed);
		SetServoPosition(RIGHT_JAGUAR, speed);
		SetServoPosition(STEERING_SERVO, angle);
	}
	
    Wait(100);
}
