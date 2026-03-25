/*
 * SoftWare_IIC.h
 *
 *  Created on: 2023Äê6ÔÂ11ÈÕ
 *      Author: xikai
 */

#ifndef BASIC_CONFIG_SOFTWARE_IIC_H_
#define BASIC_CONFIG_SOFTWARE_IIC_H_

#include "Public_Var.h"

void IIC_Hold_Time(Uint16 hold_time);
void IIC_Start(Uint32 SCL_GPIO,Uint32 SDA_GPIO,Uint16 hold_time);
void IIC_Stop(Uint32 SCL_GPIO,Uint32 SDA_GPIO,Uint16 hold_time);
void IIC_Master_ACK(Uint32 SCL_GPIO,Uint32 SDA_GPIO,Uint16 hold_time);
void IIC_Master_NO_ACK(Uint32 SCL_GPIO,Uint32 SDA_GPIO,Uint16 hold_time);
void IIC_Write_Byte(Uint32 SCL_GPIO,Uint32 SDA_GPIO,Uint8 Data,Uint16 hold_time);
unsigned char IIC_Read_Byte(Uint32 SCL_GPIO,Uint32 SDA_GPIO,Uint16 hold_time);
unsigned char IIC_Check_ACK(Uint32 SCL_GPIO,Uint32 SDA_GPIO,Uint16 hold_time);
void IIC_WritetwoByte_WithStartAndStop(Uint32 SCL_GPIO,Uint32 SDA_GPIO,Uint8 Device_Address,Uint8 Reg_Address,Uint16 hold_time);
void IIC_WriteThreeByte_WithStartAndStop(Uint32 SCL_GPIO,Uint32 SDA_GPIO,Uint8 Device_Address,Uint8 Reg_Address,Uint8 Reg_Data,Uint16 hold_time);
void IIC_WriteFourByte_WithStartAndStop(Uint32 SCL_GPIO,Uint32 SDA_GPIO,Uint8 Device_Address,Uint8 command,Uint8 Reg_Address,Uint8 Reg_Data,Uint16 hold_time);



#endif /* BASIC_CONFIG_SOFTWARE_IIC_H_ */
