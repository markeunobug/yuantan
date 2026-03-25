/*
 * FPGA_SDRAM_Config.h
 *
 *  Created on: 2023年6月11日
 *      Author: xikai
 */

#ifndef BASIC_CONFIG_FPGA_SDRAM_CONFIG_H_
#define BASIC_CONFIG_FPGA_SDRAM_CONFIG_H_

#include "Public_Var.h"

struct FPGA_data
{                                       // F1       F2      F3      F4      F5
    signed long long R1A_Q;             // 1        1       1
    signed long long R1A_I;             // 1        1       1
    signed long long R1B_Q;             // 1        1       1
    signed long long R1B_I;             // 1        1       1
    signed long long R1C_Q;             // 1        1       1
    signed long long R1C_I;             // 1        1       1
    signed long long R2A_Q;             //          1       1       1
    signed long long R2A_I;             //          1       1       1
    signed long long R2B_Q;             //          1       1       1
    signed long long R2B_I;             //          1       1       1
    signed long long R2C_Q;             //          1       1       1
    signed long long R2C_I;             //          1       1       1
    signed long long R3U_Q;             //                          1       1
    signed long long R3U_I;             //                          1       1
    signed long long R3D_Q;             //                          1       1
    signed long long R3D_I;             //                          1       1
    signed long long TX_Q;             // 1        1       1       1       1
    signed long long TX_I;             // 1        1       1       1       1
    unsigned long    GAMMA;
};
//FPGA SDRAM接口初始化
void FPGA_SDRAM_Init(void);


//读取FPGA内计算输出数据
void FPGA_ReadData(struct FPGA_data *FPGA_Read_data);


//控制FPGA工作状态
void FPGA_Set_Freq();
void FPGA_Set_AMP();
void FPGA_CLOSE_AMP();
void FPGA_Stop();

#endif /* BASIC_CONFIG_FPGA_SDRAM_CONFIG_H_ */
