/*
 * FPGA_SDRAM_Config.h
 *
 *  Created on: 2023年6月11日
 *      Author: xikai
 */

#ifndef BASIC_CONFIG_FPGA_SDRAM_CONFIG_H_
#define BASIC_CONFIG_FPGA_SDRAM_CONFIG_H_

#include "Public_Var.h"


//FPGA SDRAM接口初始化
void FPGA_SDRAM_Init(void);


//读取FPGA内计算输出数据
struct FPGA_data FPGA_ReadData(void);


//控制FPGA工作状态
void FPGA_Set_Freq();
void FPGA_Set_AMP();
void FPGA_Stop();

#endif /* BASIC_CONFIG_FPGA_SDRAM_CONFIG_H_ */
