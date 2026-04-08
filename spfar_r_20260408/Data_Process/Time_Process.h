/*
 * Timer_Process.h
 *
 *  Created on: 2023年7月2日
 *      Author: xikai
 */

#ifndef DATA_PROCESS_TIME_PROCESS_H_
#define DATA_PROCESS_TIME_PROCESS_H_

#include "Public_Var.h"


//时间格式转换
unsigned char BCD2Decimal(unsigned char BCDVal);
unsigned char Decimal2BCD(unsigned char DecimalVal);
unsigned char Second_Decimal2BCD(unsigned char DecimalVal);
void SetSampleTime(void);   //设定采集开始时间


#endif /* DATA_PROCESS_TIME_PROCESS_H_ */
