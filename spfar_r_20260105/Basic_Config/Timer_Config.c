/*
 * Timer_Config.c
 *
 *  Created on: 2023年6月11日
 *      Author: xikai
 */
#include "Timer_Config.h"


//定时器0中断回调函数,定时20ms，用于检测帧是否结束
interrupt void cpu_timer0_isr(void)
{
   CpuTimer0.InterruptCount++;
   Framedata_rxflag=1;                              //帧结束标志置1
   RxBufCnt=0;                                      //帧结束，清零接收字符数
   StopCpuTimer0();                                 //停止该定时器
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;          //清除中断标志位
}

//定时器1中断回调函数，用于定时单个频率工作时间
interrupt void cpu_timer1_isr(void)
{
   Frame_Flag=1;                                    //帧结束标志位
   EDIS;                                            //直接确认，无需PIE
}

//定时器2中断回调函数，用于定时所有频率工作时间
interrupt void cpu_timer2_isr(void)
{
   MicroFrame_Flag = 1;                             //10ms结束
   EDIS;                                            //直接确认，无需PIE
}

void Timer_Init(void)
{
    InitCpuTimers();                                            //初始化CPU定时器基础
    //注册中断回调函数
    EALLOW;
    PieVectTable.TINT0 = &cpu_timer0_isr;//定时器0 用于20ms超时 rs485通讯结束，Framedata_rxflag置1
    PieVectTable.XINT13 = &cpu_timer1_isr;//定时器1 用于5s超时 1个工作频率周期结束
    PieVectTable.TINT2 = &cpu_timer2_isr;//定时器2 用于10ms超时 1个fpga采样间隔周期结束
    EDIS;
    ConfigCpuTimer(&CpuTimer0, 150, 20000);                     // 20ms没接受到数据默认该帧结束
}

