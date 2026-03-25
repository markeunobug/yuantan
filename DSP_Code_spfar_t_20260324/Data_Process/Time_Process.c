/*
 * Time_Process.c
 *
 *  Created on: 2023年7月2日
 *      Author: xikai
 */

#include "Time_Process.h"

//BCD码转为十进制
unsigned char BCD2Decimal(unsigned char BCDVal)//BCD码转换为十进制数。范围0~99，8bit
{
    unsigned char HighVal;
    unsigned char LowVal;
    unsigned char TempVal;
    HighVal = (BCDVal >> 4)&0x0f;
    LowVal = BCDVal & 0x0f;
    TempVal = (HighVal*10) + (LowVal);
    return TempVal;
}

unsigned char Decimal2BCD(unsigned char DecimalVal) //十进制数转换为BCD码。范围0~99，8bit
{
    unsigned char HighVal;
    unsigned char LowVal;
    unsigned char TempVal;
    if(DecimalVal>99)
        DecimalVal = 99;
    HighVal = DecimalVal / 10;
    LowVal = DecimalVal % 10;
    TempVal = ((HighVal&0x0f)<<4) + (LowVal&0x0f);
    return TempVal;
}

unsigned char Second_Decimal2BCD(unsigned char DecimalVal) //十进制数转换为BCD码。范围0~99，8bit
{
    unsigned char HighVal;
    unsigned char LowVal;
    unsigned char TempVal;
    if(DecimalVal>99)
        DecimalVal = 99;
    HighVal = DecimalVal / 10;// CH = 0，开启OSC
    LowVal = DecimalVal % 10;
    TempVal = ((HighVal&0x0f)<<4) + (LowVal&0x0f);
    return TempVal;
}



