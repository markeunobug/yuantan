/*
 * Basic_Config.c
 *
 *  Created on: 2023年6月9日
 *      Author: xikai
 */

#include "Basic_Config.h"

//系统初始化
void System_Init(void)
{
    InitSysCtrl();          //初始化系统时钟，PLL，看门狗，启用外设时钟
    InitGpio();             //初始化GPIO
    DINT;                   //禁止和清除所有CPU中断
    InitPieCtrl();          //初始化 PIE 控制寄存器到默认状态，默认状态是全部 PIE 中断被禁用和标志位被清除
    IER = 0x0000;
    IFR = 0x0000;           // 禁用 CPU 中断和清除所有 CPU 中断标志位
    InitPieVectTable();     // 初始化 PIE 中断向量表

    //脱机运行时执行以下代码
#ifdef _FLASH
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);                   //将FLASH中的程序COPY到RAM中运行，加快程序的运行速度
    MemCopy(&Flash28_API_LoadStart, &Flash28_API_LoadEnd, &Flash28_API_RunStart);       //将FLASH_API程序COPY进RAM中运行
    InitFlash();                                                                        //此函数必须在RAM中运行
#endif
}

void Delay_Process(void)
{
    while(DelayTimeFinishFlag)
     {
         DS1629_ReadTimeAll();
         if(Framedata_rxflag)
         {
             //此处为RS485接口接收数据的解码判断。通过查表方式判断。
             Decode_Cmd();
             Framedata_rxflag = 0;
         }
         DELAY_MS(5000);//每5秒为一个周期判断1次
         if((ReadTime[6]<24)||(ReadTime[5]>12)||(ReadTime[4]>31)||(ReadTime[2]>23)||(ReadTime[1]>59)||(ReadTime[0]>59))//时钟失效，不必延迟
             DelayTimeFinishFlag=0;
         else if(ReadTime[6] > The_End_Time[6])//年执行延迟结束
             DelayTimeFinishFlag=0;
         else if(ReadTime[6] == The_End_Time[6])//年相等
         {
            if(ReadTime[5] > The_End_Time[5]) //判断月
                DelayTimeFinishFlag=0;
            else if(ReadTime[5] == The_End_Time[5])
            {
                if(ReadTime[4] > The_End_Time[4])//判断日
                    DelayTimeFinishFlag=0;
                else if(ReadTime[4] == The_End_Time[4])
                {
                    if(ReadTime[2] > The_End_Time[2])//判断时
                        DelayTimeFinishFlag=0;
                    else if(ReadTime[2] == The_End_Time[2])//判断时
                   {
                       if(ReadTime[1] > The_End_Time[1])//判断分
                           DelayTimeFinishFlag=0;
                       else if(ReadTime[1] == The_End_Time[1])//判断
                       {
                           if(ReadTime[0] >= The_End_Time[0])//判断秒
                               DelayTimeFinishFlag=0;
                       }

                   }
                }


            }
         }

     }
 SamplingFlag = 1;//延迟结束后自启动
}

void LED_Init(void)
{
    EALLOW;

    GpioCtrlRegs.GPAMUX1.bit.GPIO15=0;   //设置为通用GPIO功能
    GpioCtrlRegs.GPADIR.bit.GPIO15=1;    //设置GPIO方向为输入
    GpioCtrlRegs.GPAPUD.bit.GPIO15=0;    //使能GPIO上拉电阻
    GpioDataRegs.GPASET.bit.GPIO15=0;
    EDIS;
}

void LED_Toggle(void)
{
    GpioDataRegs.GPATOGGLE.bit.GPIO15 = 1;
}
