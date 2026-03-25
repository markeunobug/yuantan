/*
 * MCP23008_Config.h
 *
 *  Created on: 2023Äê6ÔÂ11ÈÕ
 *      Author: xikai
 */

#ifndef BASIC_CONFIG_MCP23008_CONFIG_H_
#define BASIC_CONFIG_MCP23008_CONFIG_H_

#include "Public_Var.h"

#include "SoftWare_IIC.h"

#define MCP23008_SCL_PIN    0
#define MCP23008_SDA_PIN    1
#define MCP23008_SCL_HOLD_TIME   100
#define IO_DIR_REG              0x00
#define OLAT_REG                0x09
//#define MCP23008_ADDR           0x4E
#define MCP23008_R1A_ADDR      0x40
#define MCP23008_R1B_ADDR      0x42
#define MCP23008_R1C_ADDR      0x44
#define MCP23008_R2A_ADDR      0x46
#define MCP23008_R2B_ADDR      0x48
#define MCP23008_R2C_ADDR      0x4A
#define MCP23008_R3U_ADDR      0x4C
#define MCP23008_R3D_ADDR      0x4E

extern unsigned char SleepMode;


void MCP23008_Init(void);
void MCP23008_Set_Amp();
void Single_Write_MCP23008(unsigned char Reg_Address,unsigned char Reg_Data);
#endif /* BASIC_CONFIG_MCP23008_CONFIG_H_ */
