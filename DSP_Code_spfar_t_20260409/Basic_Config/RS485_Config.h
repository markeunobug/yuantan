/*
 * RS485_Config.h
 *
 *  Created on: 2023Äê6ÔÂ11ÈÕ
 *      Author: xikai
 */

#ifndef BASIC_CONFIG_RS485_CONFIG_H_
#define BASIC_CONFIG_RS485_CONFIG_H_


#include "Public_Var.h"

#define RS485_NRE1 22
#define RS485_NRE2 11
#define RS485_NRE3 23 //12

void RS485B_TXMODE();
void RS485B_RXMODE();
void RS485C_TXMODE();
void RS485C_RXMODE();
void RS485_Init(void);

void RS485_Send_SimulateID(void);
void RS485_Send_MWDID(void);
void RS485_Send_Arry(unsigned char *p,unsigned char num);
void RS485_SendOneByte(unsigned char a);
void RS485_Sendstring(unsigned char * Str);
void RS485_SendUnsignedInt(unsigned int Senddata);
void RS485_Send3byte(unsigned long Senddata);
void RS485_SendLong(unsigned long Senddata);
void RS485_SendSixbytes(unsigned long long Senddata);
void RS485_SendFloat(float Senddata);
unsigned char Rs485_RecieveOneByte(void);
void RS485_SendTime(unsigned char *str);
unsigned int Send_FloatToInt_BUF(float temp[],unsigned int idx_begin,unsigned int idx_end);

void RS485_SetBsp(void);

void RS485_Send_BackCmd(void);

#endif /* BASIC_CONFIG_RS485_CONFIG_H_ */
