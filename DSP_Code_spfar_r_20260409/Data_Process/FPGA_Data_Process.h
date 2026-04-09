/*
 * FPGA_Buffer.h
 *
 *  Created on: 2023年7月2日
 *      Author:
 */

#ifndef DATA_PROCESS_FPGA_DATA_PROCESS_H_
#define DATA_PROCESS_FPGA_DATA_PROCESS_H_

#include "Public_Var.h"

//处理FPGA端读回来的数据
void GET_Sector_Data(signed long long temp_res[],unsigned int temp_gama[],unsigned int num_res[],unsigned int num_gama[]);
void SingleAverage_data_res(signed long long temp[],unsigned int num[]);
void SingleAverage_data_gama(unsigned int temp[],unsigned int num[]);
void Relative_Phase_Compensate(void);
#endif /* DATA_PROCESS_FPGA_DATA_PROCESS_H_ */

