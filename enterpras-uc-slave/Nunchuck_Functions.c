#include <stdlib.h>				// malloc & free
#include <stdarg.h>				// va_---- functions
#include <string.h>				// String functions
#include <math.h>
#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"		// input/output over UART
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "driverlib/gpio.h"		// input/output over UART
#include "driverlib/adc.h"
#include "driverlib/i2c.h"
#include "RASLib/init.h"
#include "RASLib/timer.h"		// input/output over UART
#include "RASLib/servo.h"
#include "RASLib/uart.h"
#include "RASLib/motor.h"
#include "RASLib/encoder.h"
#include "RASLib/i2c.h"
#include "../src/i2c.c"


#define codeSelect GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5) >> 2
#define isNum(x)		(x>='0' && x<='9')
#define isChar(x)		((x>='a' && x<='z') || (x>='A' && x<='Z'))
#define isHex(x)		((x>='a' && x<='f') || (x>='A' && x<='F')) || isNum(x)
#define charToByte(x)	(isNum(x) ? x-'0': ((x>='a' ? x-'a': x-'A')+10))

void InitializeI2C()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

	GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);
    I2CMasterInitExpClk(I2C0_MASTER_BASE, SysCtlClockGet(), false);
}

void init_nunchuck()
{
    UARTprintf("Initializing Wireless Nunchuck\n\n");   
    while(1){
        I2CSend(0x52<<1, 2, 0xF0, 0x55);
        if (I2CMasterErr(I2C0_MASTER_BASE) == I2C_MASTER_ERR_NONE){
            Wait(1);
            I2CSend(0x52<<1, 2, 0xFB, 0x00);
            if (I2CMasterErr(I2C0_MASTER_BASE) == I2C_MASTER_ERR_NONE)
                break;
        }
        
        Wait(10);
    }
}

void ackermannNunchuckTest()
{
	char i;
	unsigned char data[16];
    short wiichuck[7], xinit=0, yinit=0, l_vel, r_vel;
    int xpow, ypow;
	
    // Wireless Nunchucks Zero @ 128
    xinit = yinit = 128;
        
	while(1){
		//Start Recalculating Values
        Wait(1);
		I2CSend(0x52<<1, 1, 0x00);
        Wait(1);   
		I2CSend(0x52<<1, 1, 0x00);
        Wait(1);     
		I2CSend(0x52<<1, 1, 0x00);
        
        if (I2CMasterErr(I2C0_MASTER_BASE) != I2C_MASTER_ERR_NONE){
            UARTprintf("Send Zero Error:\n");
            switch(I2CMasterErr(I2C0_MASTER_BASE)){
                case I2C_MASTER_ERR_ADDR_ACK:
                    UARTprintf(" I2C_MASTER_ERR_ADDR_ACK\n");
                    break;
                case I2C_MASTER_ERR_DATA_ACK:
                    UARTprintf(" I2C_MASTER_ERR_DATA_ACK\n");
                    break;
                case I2C_MASTER_ERR_ARB_LOST:
                    UARTprintf(" I2C_MASTER_ERR_ARB_LOST\n");
                    break;
                default:
                    UARTprintf("WTF: %d\n", I2CMasterErr(I2C0_MASTER_BASE));
            }
            
            // Reinitialize Nunchuck on error
            init_nunchuck();
        }else{
            Wait(1);
            I2CRecieve(0x52<<1, data, 6);   // Nunchuck data is 6 bytes, but for whatever reason, MEMOREX Wireless Nunchuck wants to send 8...
            
            if (I2CMasterErr(I2C0_MASTER_BASE) != I2C_MASTER_ERR_NONE){
                UARTprintf("Send Zero Error:\n");
                switch(I2CMasterErr(I2C0_MASTER_BASE)){
                    case I2C_MASTER_ERR_ADDR_ACK:
                        UARTprintf(" I2C_MASTER_ERR_ADDR_ACK\n");
                        break;
                    case I2C_MASTER_ERR_DATA_ACK:
                        UARTprintf(" I2C_MASTER_ERR_DATA_ACK\n");
                        break;
                    case I2C_MASTER_ERR_ARB_LOST:
                        UARTprintf(" I2C_MASTER_ERR_ARB_LOST\n");
                        break;
                }
                
                // Reinitialize Nunchuck on error
                init_nunchuck();
            }else{
				unsigned char l_vel2 = 127;
				unsigned char r_vel2 = 127;
                //for(i=0; i<6; i++)
                //    data[i] = (data[i] ^ 0x17) + 0x17;  // Nintendo decided to encrypt thir data...
        
        		// Save Joystick Data
        		wiichuck[0] = data[1];                                          // X Axis Joystick
        	    wiichuck[1] = data[0];                                          // Y Axis Joystick
        		wiichuck[2] = (((unsigned short) data[2]) << 2) + (((unsigned short) data[5]) & (3<<2));    // X Axis Accel
        		wiichuck[3] = (((unsigned short) data[3]) << 2) + (((unsigned short) data[5]) & (3<<4));    // Y Axis Accel
        		wiichuck[4] = (((unsigned short) data[4]) << 2) + (((unsigned short) data[5]) & (3<<6));    // Z Axis Accel
        		wiichuck[5] = data[5] & (1 << 1) ? 0 : 1;                              //'C' Button 
        		wiichuck[6] = data[5] & (1 << 0) ? 0 : 1;                              //'Z' Button
            
            //if (xinit == 0 && yinit == 0){
            //    xinit = wiichuck[0]-127;
            //    yinit = wiichuck[1]-127;
           //}else{
                xpow = (wiichuck[0]-xinit)/2;
                ypow = (wiichuck[1]-yinit)/2;
				
                l_vel = (xpow - ypow)*2;
                r_vel = (xpow + ypow)*2;
                
                l_vel = l_vel > 127 ? 127 : l_vel;
                r_vel = r_vel > 127 ? 127 : r_vel;
                l_vel = l_vel < -127 ? -127 : l_vel;
                r_vel = r_vel < -127 ? -127 : r_vel;
				
				if(wiichuck[5]==0) {
					l_vel >>=1;
					r_vel >>=1;
				}
				
				//l_vel = (l_vel > 0) ? 127 - l_vel : 128;
				//r_vel = (r_vel > 0) ? 127 - r_vel : 128;
				l_vel2 = 127 - l_vel;
				r_vel2 = 127 - r_vel;
                
                //UARTprintf("X: %d\tY: %d\n", xpow*2, ypow*2);
                //SetMotorPowers(l_vel / (wiichuck[5]==0 ? 2 : 1), r_vel / (wiichuck[5]==0 ? 2 : 1));
				
				SetServoPosition(SERVO_0, l_vel2 );
				SetServoPosition(SERVO_2, r_vel2 );
				
                UARTprintf("Motor L: %d\tMotor R: %d\n", l_vel, r_vel);
                //SetServoPosition(SERVO_0, wiichuck[6]==1 ? 255 : 140);
                UARTprintf("Nunchuck Data:\n");
                for(i=0; i<7; i++){
                    UARTprintf(" %d\n", wiichuck[i]);
                }NL;
                
                Wait(100);
            }
        }
	}
}
