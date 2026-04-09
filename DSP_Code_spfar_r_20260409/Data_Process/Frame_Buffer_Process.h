/*
 * FLASH_Buffer.h
 *
 *  Created on: 2023年7月2日
 *      Author: xikai
 */

#ifndef DATA_PROCESS_FRAME_BUFFER_PROCESS_H_
#define DATA_PROCESS_FRAME_BUFFER_PROCESS_H_


#include "Public_Var.h"

//帧数据Buffer


void Send_Frame_Start(void);                                //写入每个频率相同的信息
void Send_Frame_End(void);



void Send_Frame_UnsignedChar(unsigned char *Ulval,unsigned int Num);
void Send_Frame_UnsignedLong(unsigned long *Senddata,unsigned int Num);
void Send_Frame_UnsignedInt(unsigned int *Senddata,unsigned int Num);
void Send_Frame_Float(float *Senddata,unsigned int Num);
void Send_Frame_Time(unsigned char *Time);
void Send_Frame_WithFreq();


#endif /* DATA_PROCESS_FRAME_BUFFER_PROCESS_H_ */
