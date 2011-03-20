#ifndef SETTINGS_H
#define SETTINGS_H

/*
0x00 = autonomous
0x01 = nunchuck
*/
extern volatile unsigned char control_mode;

/*
0x00 = ackermann
0x01 = differential
*/
extern volatile unsigned char drive_type; 

extern volatile unsigned char battery_voltage; //can be anything, but should most likely be 12 or 24


#endif
