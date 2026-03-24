/*
 * GAMMA_Count.c
 *
 *  Created on: 2024年8月28日
 *      Author: zxk
 */
#include "GAMMA_Count.h"
//不能在dsp中采集gamma计数，在fpga中采集
interrupt void GAMMA_Count_ISR(void)
{
    GAMMA_Current_Count++;
    PieCtrlRegs.PIEACK.bit.ACK1 = 1;//写1清零PIEACK对应位
}

void GAMMA_Count_EXTI_Init(void)
{
    //配置GPIO为下拉输入模式
    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO5 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1;
    EDIS;

    //GPIO挂接中断，设定中断向量表
    EALLOW;
    GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 5;//XINT1 对应 GPIO5脚
    PieVectTable.XINT1 = &GAMMA_Count_ISR;
    EDIS;

    //设置中断触发方式 使能中断
    XIntruptRegs.XINT1CR.bit.POLARITY = 0;  //上升沿触发有效
    XIntruptRegs.XINT1CR.bit.ENABLE = 1;    //使能外部中断
}



