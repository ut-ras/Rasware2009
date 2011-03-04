/*
 * UART_Functions.h
 * Provides functions that deal with UART I/O.
 * Author: Joshua James
 *
 */
 
#ifndef UART_FUNCTIONS_H
#define UART_FUNCTIONS_H

extern void initUART(void);
extern void sendMessage(char* message);
extern void getMessage(char* buffer, char size);
extern void simpleCommTest(void);

#endif
