/*
 * Basic_Config.h
 *
 *  Created on: 2023年6月8日
 *      Author: xikai
 */

#ifndef BASIC_CONFIG_BASIC_CONFIG_H_
#define BASIC_CONFIG_BASIC_CONFIG_H_

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "Public_Var.h"

//系统初始化函数
void System_Init(void);
void Delay_Process(void);
void LED_Init(void);
void LED_Toggle(void);

#define DEBUG

#endif /* BASIC_CONFIG_BASIC_CONFIG_H_ */
