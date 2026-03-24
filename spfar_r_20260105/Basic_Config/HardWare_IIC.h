/*
 * IIC.h
 *
 *  Created on: 2023Äê10ÔÂ23ÈÕ
 *      Author: xikai
 */

#ifndef BASIC_CONFIG_IIC_H_
#define BASIC_CONFIG_IIC_H_

#include "Basic_Config.h"


void Init_I2CA(void);
Uint16 IIC_WriteData( Uint16 SlaveAddress, Uint16 RomAddress,Uint16 number, unsigned char *Wdata);


#endif /* BASIC_CONFIG_IIC_H_ */
