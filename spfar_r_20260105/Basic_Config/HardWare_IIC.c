/*
 * IIC.c
 *
 *  Created on: 2023年10月23日
 *      Author: xikai
 */
#include "HardWare_IIC.h"

// IIC通信错误标志，记录但不影响下一帧数据发送
Uint16 BUS_BUSY_ERROR = 0;
Uint16 WriteReady_ERROR = 0;
Uint16 NOACK_ERROR = 0;


void Init_I2CA(void)
{
//    EALLOW;
//    GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;    // 使能(SDAA)上拉
//    GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;    //  使能 (SCLA)上拉
//    GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  // 不同步
//    GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  // 不同步
//    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;   // 配置 GPIO32为 SDAA
//    GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;   // 配置 GPIO33 为SCLA
//    EDIS;

    I2caRegs.I2CMDR.all = 0x0000;           // 复位IIC

    // 预分频——时钟模块的频率
    I2caRegs.I2CPSC.all = 14;               //预分频   IIC模块时钟需设置为7-12MHz，本实验设置为 (150/15 = 10MHz)
    I2caRegs.I2CCLKL = 95;                  //时钟低电平时间值
    I2caRegs.I2CCLKH = 95;                   //时钟高电平时间值
    I2caRegs.I2CIER.all = 0x00;             // Enable SCD & ARDY interrupts

    I2caRegs.I2CFFTX.all = 0x0000;          // Disable FIFO mode and TXFIFO
    I2caRegs.I2CFFRX.all = 0x0000;          // Disable RXFIFO, clear RXFFINT,
    I2caRegs.I2CMDR.all = 0x0020;           // IIC模块使能
}


Uint16 I2C_Check_WriteReady()
{
    unsigned int Error = 0;
    while(I2caRegs.I2CSTR.bit.XRDY==0)
    {
        Error++;
        if(Error==2255)
        {
            return 0;
        }
    }
    return 1;
}

Uint16 I2C_Check_ReadReady()
{
    unsigned int Error = 0;
    while(I2caRegs.I2CSTR.bit.RRDY==0)
    {
        Error++;
        if(Error==65255)
        {
            return 0;
        }
    }
    return 1;
}

/**
 *  @brief                  IIC写数据
 *  @parameter              SlaveAddress：从机地址；  RomAddress：寄存器地址； number：写入数据的字节数；  *Wdata：写入数据的地址
 *  @return_value           状态标志
 */
Uint16 IIC_WriteData( Uint16 SlaveAddress, Uint16 RomAddress,Uint16 number, unsigned char *Wdata)
{
   I2caRegs.I2CMDR.all = 0x0020;
   DELAY_US(2);
   if(I2caRegs.I2CSTR.bit.BB == 1)
   {
      BUS_BUSY_ERROR++;
      I2caRegs.I2CMDR.all = 0x0000;
      DELAY_US(2);
      I2caRegs.I2CMDR.all = 0x0020;
      return I2C_ERROR;
    // I2caRegs.I2CSTR.bit.BB = 0;              //上一帧数据未接收到应答信号，未发送结束，总线仍然占用，手动释放
   }

   I2caRegs.I2CSAR = SlaveAddress&0xff;         //设备从地址
   I2caRegs.I2CCNT = number + 1;                //需要发送的字节数

   DELAY_US(2);                                 //等待数据完全赋值到I2CDXR，再使能IIC，必须要加上！
   I2caRegs.I2CMDR.all = 0x6E20;                //发送起始信号，内部数据计数器减到0时发送停止信号，主机发送模式，使能IIC模式，
   if(I2caRegs.I2CSTR.bit.BB == 1)
    {
       BUS_BUSY_ERROR++;
       I2caRegs.I2CMDR.all = 0x0000;
       DELAY_US(2);
       I2caRegs.I2CMDR.all = 0x0020;
    //   I2caRegs.I2CSTR.bit.BB = 0;              //上一帧数据未接收到应答信号，未发送结束，总线仍然占用，手动释放
       return I2C_ERROR;
    }
   DELAY_US(2);
   I2caRegs.I2CDXR = RomAddress&0xff;           //第一个发送字节为发送数据的目标寄存器地址
   if(I2C_Check_WriteReady()==0)           //等待上一个字节发送完毕
   {
       WriteReady_ERROR++;
       I2caRegs.I2CMDR.all = 0x0000;
       DELAY_US(2);
       I2caRegs.I2CMDR.all = 0x0020;
       return I2C_ERROR;
   }
   DELAY_US(2);
   I2caRegs.I2CDXR = *Wdata&0xFF;           //第一个发送字节为发送数据的目标寄存器地址
   if(I2C_Check_WriteReady()==0)           //等待上一个字节发送完毕
   {
      WriteReady_ERROR++;
      I2caRegs.I2CMDR.all = 0x0000;
      DELAY_US(2);
      I2caRegs.I2CMDR.all = 0x0020;
      return I2C_ERROR;
    }
   DELAY_US(500);                               //保证最后一字节数据发送
   return I2C_SUCCESS;                          //发送成功
}


