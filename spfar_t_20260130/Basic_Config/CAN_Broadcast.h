/*
 * CAN_Broadcast.h
 *
 *  Created on: 2024Äê9ÔÂ3ÈÕ
 *      Author: zxk
 */

#ifndef BASIC_CONFIG_CAN_BROADCAST_H_
#define BASIC_CONFIG_CAN_BROADCAST_H_

#include "Public_Var.h"

#define CAN_Local_ID   0x0011
#define CAN_Set_CMD    0x0101
#define CAN_Inqure_CMD 0x0100

void Can_Set_Freq_AMP(unsigned char ID_Number,unsigned char Freq_Word,unsigned char Amp_Word);
void CAN_Init(void);

#endif /* BASIC_CONFIG_CAN_BROADCAST_H_ */
