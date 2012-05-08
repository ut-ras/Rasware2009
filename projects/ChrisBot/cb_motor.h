
//MOTOR_TYPE determines how the motor runs
//can be PWM or BINARY
#ifndef MOTOR_TYPE
#define MOTOR_TYPE PWM
#endif

#define MOTOR_FORWARD 1
#define MOTOR_STOP 0
#define MOTOR_BACKWARD -1

void Motor_Init(tBoolean,tBoolean,tBoolean);
void Motor_Set(signed char, signed char);
