/*
 * Debug_UART_Config.c
 *
 *  Created on: 2024年1月17日
 *      Author: zxk
 */
#include "MS578H_UART_Config.h"

char scic_Rxbuffer[100];
volatile Uint16 scic_RxPos=0;

/*static char MS578H_verify(Uint16 Num)
{
    char xorvalue=0xff;
    int i;
    for(i=2;i<Num-1;i++)
    {
        xorvalue ^= scic_Rxbuffer[i];
    }
    if(xorvalue==scic_Rxbuffer[Num-1])
        return 1;
    else
        return 0;
}
*/
interrupt void scicRx_isr(void)
{
/*     while(ScicRegs.SCIRXST.bit.RXRDY!=1) {}
     if(ScicRegs.SCIRXST.bit.PE==0)                 //如果校验位正确的话
     {
         scic_Rxbuffer[scic_RxPos] = ScicRegs.SCIRXBUF.all;
         scic_RxPos++;
         //进入表示接收到帧头，操作防止Buffer有累积数据
         if(scic_Rxbuffer[scic_RxPos-1]==0x55 && scic_Rxbuffer[scic_RxPos-2]==0xFF)
         {
             scic_RxPos = 2;
             scic_Rxbuffer[0] = 0xFF;
             scic_Rxbuffer[1] = 0x55;
         }
         else if(scic_RxPos==10 && scic_Rxbuffer[2]==0x04)  // sector
         {
             //校验
             if(MS578H_verify(10))
             {
                 Cur_sector = (scic_Rxbuffer[5] << 8) | scic_Rxbuffer[6];
                 Cur_sector = Cur_sector/2250;//角度除100，除22.5
                 Cur_sector = Cur_sector % 16;
             }
         }
         else if(scic_RxPos==22 && scic_Rxbuffer[2]==0x05)  // all angle
         {
             //校验
             if(MS578H_verify(22))
             {
                 Uint16 Temp;
                 // 5 6 重力工具面
                 // 7 8 磁工具面
                 // 9 10 井斜
                 Temp = scic_Rxbuffer[9]*256 + scic_Rxbuffer[10];
                 lnc = Temp / 10;
                 // 11 12 总磁场强度
                 //13 14 方位角
                 Temp = scic_Rxbuffer[13]*256 + scic_Rxbuffer[14];
                 Azi = Temp / 10;
                 //15 16 总加速度
                 //17 18 温度传感器
                 Temp = scic_Rxbuffer[17]*256 + scic_Rxbuffer[18];
                 Temperature = Temp / 100;
                 //19 20 转速
                 Temp = scic_Rxbuffer[19]*256 + scic_Rxbuffer[20];
                 Rpm = Temp / 100;
             }
         }
         else
         {

         }
     }
     else    //校验位错误，标志位清0
     {
           ScicRegs.SCICTL1.bit.SWRESET=0;          //软件复位
           while(ScicRegs.SCIRXST.bit.PE==1);
           ScicRegs.SCICTL1.bit.SWRESET=1;          //软件复位
           scic_RxPos = 0;
     }
     PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;        //清除中断标志位
     */
}

void MS578H_Uart_Init(void)
{
    //初始化GPIO
    InitScicGpio();
    //注册中断服务函数
    EALLOW;
    PieVectTable.SCIRXINTC = &scicRx_isr;
    EDIS;

    ScicRegs.SCICCR.all =0x0007;            //1个停止位，8位字符，没有循环检测,无校验
    ScicRegs.SCICTL1.all =0x0003;           // 使能TX，使能RX
    ScicRegs.SCICTL2.bit.TXINTENA =0;       //关闭发送中断
    ScicRegs.SCICTL2.bit.RXBKINTENA =0;     //关闭中断接收
    /*  ScicRegs.SCIHBAUD    =0x0000;           // 115200 baud @LSPCLK = 37.5MHz.
    ScicRegs.SCILBAUD    =0x0027;*/
    ScicRegs.SCIHBAUD    =0x0000;           // 460800 baud @LSPCLK = 37.5MHz.   scibaud=37500000/(8*baud)-1
    ScicRegs.SCILBAUD    =0x009;

    ScicRegs.SCICTL1.all =0x0023;           //退出SCI时复位

    ScicRegs.SCIFFTX.bit.SCIFFENA=0;        //禁止FIFO功能
}

//发送字符
void MS578H_SendOneByte(unsigned char a)
{
    while(ScicRegs.SCICTL2.bit.TXRDY==0) {}
    ScicRegs.SCITXBUF=a;
}

void MS578H_Require_sector(void)
{
    unsigned char cnt=0;
    unsigned char sum=0xff;
    unsigned int delay_num=0;
 //   PieCtrlRegs.PIEIER9.bit.INTx3 = 0;   //开启SCIB的接收中断,SCIRXINTB
    ScibRegs.SCICTL2.bit.RXBKINTENA =0;     //关闭485mwd总线模式中断接收
 /*   if((ScibRegs.SCIRXST.all & 0xfc) != 0x00)
    {
     ScibRegs.SCICTL1.bit.SWRESET=0;          //软件复位
     while((ScibRegs.SCIRXST.all & 0xfc) != 0x00);   //RXERROR RXRDY BRKDT FE OE PE等待接收状态寄存器scirxst到复位状态，清除
     ScibRegs.SCICTL1.bit.SWRESET=1;          //软件复位
    }

    if((ScicRegs.SCIRXST.all & 0xfc) != 0x00)
    {
     ScicRegs.SCICTL1.bit.SWRESET=0;          //软件复位
     while((ScicRegs.SCIRXST.all & 0xfc) != 0x00);   //RXERROR RXRDY BRKDT FE OE PE等待接收状态寄存器scirxst到复位状态，清除
     ScicRegs.SCICTL1.bit.SWRESET=1;          //软件复位
    }*/
    RS485C_TXMODE();
    MS578H_SendOneByte(0xFF);
    MS578H_SendOneByte(0xAA);
    MS578H_SendOneByte(0x04);
    MS578H_SendOneByte(0x31);
    MS578H_SendOneByte(0x00);
    MS578H_SendOneByte(0xCA);
    while(ScicRegs.SCICTL2.bit.TXEMPTY==0);
    RS485C_RXMODE();
  //  DELAY_US(10);                             //延时10us
    while((ScicRegs.SCIRXST.bit.RXRDY!=1)&&(delay_num<1000))//首字节处理，等待时间稍长
    {
        delay_num++;
            if (delay_num == 1000)
            {
                ScibRegs.SCICTL2.bit.RXBKINTENA =1;     //函数返回前必须把scib接收打开
                return;
            }

        DELAY_US(1);
    }
    scic_Rxbuffer[0] = ScicRegs.SCIRXBUF.all;
    for(cnt=1;cnt<10;cnt++)
    {
        delay_num=0;
        while((ScicRegs.SCIRXST.bit.RXRDY!=1)&&(delay_num<100))
        {
            delay_num++;
            if (delay_num == 100)
            {
              ScibRegs.SCICTL2.bit.RXBKINTENA =1;     //函数返回前必须把scib接收打开
              return;
            }
            DELAY_US(1);
        }

        scic_Rxbuffer[cnt] = ScicRegs.SCIRXBUF.all;
   //     if((cnt>1)&&(cnt<9))
     //       sum = sum^scic_Rxbuffer[cnt];
    }
    for(cnt=2;cnt<9;cnt++)
        sum = sum^scic_Rxbuffer[cnt];
    if(sum == scic_Rxbuffer[9])
    {
        float temp = 0.0f;
        temp = ((float)((scic_Rxbuffer[5] << 8) | scic_Rxbuffer[6]))/100;
        temp = 360.0 - temp;
        if((temp<=11.25)||(temp>348.75))
            Cur_sector = 0;
        else
            Cur_sector = (unsigned char)((temp-11.25)/22.5) + 1;
     //   Cur_sector = Cur_sector/2250;//角度除100，除22.5
    //    Cur_sector = 15-(Cur_sector % 16);//扇区传感器反相安装所致
    }
 //   ScicRegs.SCIRXBUF.all;
   ScibRegs.SCICTL2.bit.RXBKINTENA =1;     //开启485mwd总线模式中断接收
/*    if((ScibRegs.SCIRXST.all && 0xfc) != 0x00)
    {
     ScibRegs.SCICTL1.bit.SWRESET=0;          //软件复位
     while((ScibRegs.SCIRXST.all && 0xfc) != 0x00);   //RXERROR RXRDY BRKDT FE OE PE等待接收状态寄存器scirxst到复位状态，清除
     ScibRegs.SCICTL1.bit.SWRESET=1;          //软件复位
    }*/
 //   PieCtrlRegs.PIEIER9.bit.INTx3 = 1;   //开启SCIB的接收中断,SCIRXINTB
}

void MS578H_Require_Angle(void)
{
    unsigned char cnt=0;
    unsigned char sum=0xff;
    unsigned int delay_num=0;
 //   PieCtrlRegs.PIEIER9.bit.INTx3 = 0;   //开启SCIB的接收中断,SCIRXINTB
    ScibRegs.SCICTL2.bit.RXBKINTENA =0;     //关闭485mwd总线模式中断接收
 /*   if((ScibRegs.SCIRXST.all & 0xfc) != 0x00)
    {
     ScibRegs.SCICTL1.bit.SWRESET=0;          //软件复位
     while((ScibRegs.SCIRXST.all & 0xfc) != 0x00);   //RXERROR RXRDY BRKDT FE OE PE等待接收状态寄存器scirxst到复位状态，清除
     ScibRegs.SCICTL1.bit.SWRESET=1;          //软件复位
    }

    if((ScicRegs.SCIRXST.all & 0xfc) != 0x00)
    {
     ScicRegs.SCICTL1.bit.SWRESET=0;          //软件复位
     while((ScicRegs.SCIRXST.all & 0xfc) != 0x00);   //RXERROR RXRDY BRKDT FE OE PE等待接收状态寄存器scirxst到复位状态，清除
     ScicRegs.SCICTL1.bit.SWRESET=1;          //软件复位
    }*/

    RS485C_TXMODE();
    MS578H_SendOneByte(0xFF);
    MS578H_SendOneByte(0xAA);
    MS578H_SendOneByte(0x05);
    MS578H_SendOneByte(0x31);
    MS578H_SendOneByte(0x00);
    MS578H_SendOneByte(0xCB);
    while(ScicRegs.SCICTL2.bit.TXEMPTY==0);//确保数据发送完毕
    RS485C_RXMODE();
    //DELAY_US(10);                             //延时10us
    while((ScicRegs.SCIRXST.bit.RXRDY!=1)&&(delay_num<1000))//首字节处理，等待时间稍长
    {
        delay_num++;
        if (delay_num == 1000)
        {
         ScibRegs.SCICTL2.bit.RXBKINTENA =1;     //函数返回前必须把scib接收打开
         return;
        }
        DELAY_US(1);
    }
    scic_Rxbuffer[0] = ScicRegs.SCIRXBUF.all;
    for(cnt=1;cnt<22;cnt++)
    {
        delay_num=0;
        while((ScicRegs.SCIRXST.bit.RXRDY!=1)&&(delay_num<100))
        {
           delay_num++;
           if (delay_num == 100)
           {
              ScibRegs.SCICTL2.bit.RXBKINTENA =1;     //函数返回前必须把scib接收打开
              return;
           }
            DELAY_US(1);
        }

        scic_Rxbuffer[cnt] = ScicRegs.SCIRXBUF.all;
  //     if((cnt>1)&&(cnt<9))
    //       sum = sum^scic_Rxbuffer[cnt];
    }
    for(cnt=2;cnt<21;cnt++)
        sum = sum^scic_Rxbuffer[cnt];
    if(sum == scic_Rxbuffer[21])
    {
         Uint16 Temp;
         // 5 6 重力工具面
         // 7 8 磁工具面
         // 9 10 井斜
         Temp = (scic_Rxbuffer[9]<<8) | scic_Rxbuffer[10];
         lnc =180- Temp / 10;//扇区传感器反向安装所致
         // 11 12 总磁场强度
         //13 14 方位角
         Temp = (scic_Rxbuffer[13]<<8) |  scic_Rxbuffer[14];
         Azi = Temp / 10;
         //15 16 总加速度
         //17 18 温度传感器
         Temp = (scic_Rxbuffer[17]<<8) |  scic_Rxbuffer[18];
         Temperature = Temp / 100;
         //19 20 转速
         Temp = (scic_Rxbuffer[19]<<8) |  scic_Rxbuffer[20];
         Rpm = Temp / 100;
    }
//    ScicRegs.SCIRXBUF.all;
     ScibRegs.SCICTL2.bit.RXBKINTENA =1;     //开启485mwd总线模式中断接收
/*    if((ScibRegs.SCIRXST.all && 0xfc) != 0x00)
    {
     ScibRegs.SCICTL1.bit.SWRESET=0;          //软件复位
     while((ScibRegs.SCIRXST.all && 0xfc) != 0x00);   //RXERROR RXRDY BRKDT FE OE PE等待接收状态寄存器scirxst到复位状态，清除
     ScibRegs.SCICTL1.bit.SWRESET=1;          //软件复位
    }*/
//    PieCtrlRegs.PIEIER9.bit.INTx3 = 1;   //开启SCIB的接收中断,SCIRXINTB
}

void MS578H_Sendstring(unsigned char * Str)
{
    while( * Str != '\0')
    {
        MS578H_SendOneByte(*(Str++));
    }
}
