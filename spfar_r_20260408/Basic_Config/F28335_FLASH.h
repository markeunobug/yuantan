/*
 * F28335_FLASH.h
 *
 *  Created on: 2023Äê6ÔÂ11ÈÕ
 *      Author: xikai
 */

#ifndef BASIC_CONFIG_F28335_FLASH_H_
#define BASIC_CONFIG_F28335_FLASH_H_

#include "Public_Var.h"



void F28335Flash_Init();
void F28335Flash_Program(unsigned int *DataBuffer,unsigned long datalength);
void F28335Flash_Eraser();
void F28335Flash_Read(unsigned int *DataBuffer,unsigned long datalength);
void ReadSetting(void);
void SaveSetting(void);


#endif /* BASIC_CONFIG_F28335_FLASH_H_ */
