/*
 * DS1629_Config.h
 *
 *  Created on: 2023年6月11日
 *      Author: xikai
 */

#ifndef BASIC_CONFIG_DS1629_CONFIG_H_
#define BASIC_CONFIG_DS1629_CONFIG_H_

#include "Public_Var.h"

/****************端口宏定义*****************/
#define DS1629_SCL_PIN      27
#define DS1629_SDA_PIN      26
#define DS1629_SCL_HOLD_TIME        50
/****************************************/

/****************常量宏定义*****************/
#define TRUE    1
#define FALSE   0
/****************************************/


//定义读写地址
#define SlaveAddress_Write      0x9e    //定义器件在IIC总线中的写地址
#define SlaveAddress_Read       0x9f    //定义器件在IIC总线中的读地址

//DS1629 command_byte
#define Access_Config           0xAC    //配置字节
#define Start_Convert_T         0xEE    //开始转换温度
#define Stop_Convert_T          0x22    //停止转换温度
#define Read_Temperature        0xAA    //读取温度
#define Access_Clock            0xC0    //访问时钟命令字节
#define Access_ClockAlarm       0xC7    //访问时钟报警命令字节

//DS1629 内部时钟寄存器地址
#define DS1629_Clock_Start_Address 0x00
#define DS1629_Seconds          0x00
#define DS1629_Minutes          0x01
#define DS1629_Hours            0x02
#define DS1629_Day              0x03
#define DS1629_Date             0x04
#define DS1629_Month            0x05
#define DS1629_Year             0x06

#define DS1629_SecondsAlarm     0x00
#define DS1629_MinutesAlarm     0x01
#define DS1629_HoursAlarm       0x02
#define DS1629_DayAlarm         0x03


void DS1629_Init(void);
void DS1629_ReadTimeAll(void);
void DS1629_SetTimeAll(unsigned char *SetVal);  //配置所有时钟寄存器。秒分时周日月年，十进制
Uint16 DS1629_ReadTemperature(void);//读取温度

#endif /* BASIC_CONFIG_DS1629_CONFIG_H_ */
