/*
 * Debug_UART_Config.h
 *
 *  Created on: 2024Äê1ÔÂ17ÈÕ
 *      Author: zxk
 */

#ifndef BASIC_CONFIG_MS578H_UART_CONFIG_H_
#define BASIC_CONFIG_MS578H_UART_CONFIG_H_

#include "Public_Var.h"

#define MAX_WRITE_LEN 100

void MS578H_Uart_Init(void);
void MS578H_SendOneByte(unsigned char a);
void MS578H_Sendstring(unsigned char * Str);

void MS578H_Require_sector(void);
void MS578H_Require_Angle(void);

#endif /* BASIC_CONFIG_MS578H_UART_CONFIG_H_ */
