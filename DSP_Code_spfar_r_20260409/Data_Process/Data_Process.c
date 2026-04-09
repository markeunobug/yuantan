/*
 * Data_Process.c
 *
 *  Created on: 2023年6月12日
 *      Author: xikai
 */

#include "Data_Process.h"

//清空Buffer(Float)
void Clear_BUF_Uint64(signed long long temp[],Uint16 Num)
{
    unsigned int cnt=0;
    for(cnt=0;cnt<Num;cnt++)
    {
        temp[cnt]=0;
    }
}

//清空Buffer(Uint16)
void Clear_BUF_Uint16(Uint16 temp[],Uint16 Num)
{
    Uint16 cnt=0;
    for(cnt=0;cnt<Num;cnt++)
    {
        temp[cnt]=0;
    }
}

//清空Buffer(Uint16)
void Clear_BUF_Uchar(unsigned char temp[],Uint16 Num)
{
    Uint16 cnt=0;
    for(cnt=0;cnt<Num;cnt++)
    {
        temp[cnt]=0xff;
    }
}



