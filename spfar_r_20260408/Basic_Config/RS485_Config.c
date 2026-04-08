/*
 * RS485_Config.c
 *
 *  Created on: 2023年6月11日
 *      Author: xikai
 */

#include "RS485_Config.h"


void RS485_InitGpio()
{
      InitScibGpio();
    //  Enable_Pullup_Gpio(RS485_NRE);//上拉
    //  EALLOW;
    //        GpioCtrlRegs.GPADIR.bit.GPIO29 = 1;           //GPIO29为输出
   //    EDIS;
      Set_GPIO_Out(RS485_NRE1);//初始化输出方向
      Set_GPIO_Out(RS485_NRE2);//初始化输出方向
      Set_GPIO_Out(RS485_NRE3);//初始化输出方向
   //   RS485_RXMODE();
}

void RS485SetBspTo9600()
{
     ScibRegs.SCIHBAUD    =0x0001;  // 9600 baud @LSPCLK = 37.5MHz.
     ScibRegs.SCILBAUD    =0x00E7;
}

void RS485SetBspTo115200()
{
      ScibRegs.SCIHBAUD    =0x0000;  // 115200 baud @LSPCLK = 37.5MHz.
      ScibRegs.SCILBAUD    =0x0027;
}


void RS485SetBspTo460800()
{
      ScibRegs.SCIHBAUD    =0x0000;  // 460800 baud @LSPCLK = 37.5MHz.
      ScibRegs.SCILBAUD    =0x0009;
}

void RS485SetBspTo921600()
{
      ScibRegs.SCIHBAUD    =0x0000;  // 468750 baud @LSPCLK = 37.5MHz.
      ScibRegs.SCILBAUD    =0x0004;
}

void RS485SetBsp_2343750()
{
    ScibRegs.SCIHBAUD    =0x0000;  // 468750 baud @LSPCLK = 37.5MHz.
    ScibRegs.SCILBAUD    =0x0001;
}



void RS485_SetBsp(void)
{
    switch(baudflag)
    {
        case 0x01:
        {
            RS485SetBspTo9600();
            break;
        }
        case 0x02:
        {
            RS485SetBspTo115200();
            break;
        }
        case 0x03:
        {
            RS485SetBspTo460800();
            break;
        }
        case 0x04:
        {
            RS485SetBspTo921600();
            break;
        }
        case 0x05:
        {
            RS485SetBsp_2343750();
        }
    }
}


void RS485B_TXMODE()
{
 //   Set_GPIO_High(RS485_NRE);
    GpioDataRegs.GPASET.bit.GPIO12 = 1;
}

void RS485B_RXMODE()
{
 //    DELAY_US(200);
  //  Set_GPIO_Low(RS485_NRE);
    GpioDataRegs.GPACLEAR.bit.GPIO12 = 1;
}

void RS485C_TXMODE()
{
 //   Set_GPIO_High(RS485_NRE);
    GpioDataRegs.GPASET.bit.GPIO10 = 1;
}

void RS485C_RXMODE()
{
 //    DELAY_US(200);
  //  Set_GPIO_Low(RS485_NRE);
    GpioDataRegs.GPACLEAR.bit.GPIO10 = 1;
}

/*
 * RX中断触发的中断源包括RXRDY、BRKDT
 */

interrupt void scibRx_isr(void)
{
//    unsigned int cnt=0;
    ReloadCpuTimer0();                             //重装CPU定时器0
    StartCpuTimer0();                              //开启定时器0，用于检测帧结束
   //  while((ScibRegs.SCIRXST.bit.RXRDY!=1)&&(cnt<1000))
    // {
      //  cnt++;
        //DELAY_US(1);
    // }
     if(ScibRegs.SCIRXST.bit.RXRDY==1)//仅仅是确认一下rxbuf是否有数据，其实不确认也应该可以吧，毕竟是中断触发的
     {
         if(ScibRegs.SCIRXST.bit.PE==0)                 //如果校验位正确的话
         {
             RxBuf[RxBufCnt]=ScibRegs.SCIRXBUF.all;
             RxBufCnt++;
         }
         else    //校验位错误，标志位清0
         {
               ScibRegs.SCICTL1.bit.SWRESET=0;          //软件复位
               while(ScibRegs.SCIRXST.bit.PE==1);
               ScibRegs.SCICTL1.bit.SWRESET=1;          //软件复位
         }
     }
     else
     {
         ScibRegs.SCICTL1.bit.SWRESET=0;          //软件复位
         while((ScibRegs.SCIRXST.all & 0xfc) != 0x00);   //RXERROR RXRDY BRKDT FE OE PE等待接收状态寄存器scirxst到复位状态，清除
         ScibRegs.SCICTL1.bit.SWRESET=1;          //软件复位
     }
 //    ScibRegs.SCIRXBUF.all;//清除ScibRegs.SCIRXST.bit.RXRDY
     PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;        //清除中断标志位
}

void RS485_Init(void)
{
    //初始化GPIO
    RS485_InitGpio();
    //注册中断服务函数
    EALLOW;
    PieVectTable.SCIRXINTB = &scibRx_isr;
    EDIS;

    ScibRegs.SCICCR.all =0x0067;            //1个停止位，8位字符，没有循环检测,偶校验
    ScibRegs.SCICTL1.all =0x0003;           // 使能TX，RX
    ScibRegs.SCICTL2.bit.TXINTENA =0;       //关闭发送中断
    ScibRegs.SCICTL2.bit.RXBKINTENA =1;     //中断接收
    RS485SetBspTo9600();                    // BRR = LSPCLK/(Bsp*8)-1
    ScibRegs.SCICTL1.all =0x0023;           //退出SCI时复位

    ScibRegs.SCIFFTX.bit.SCIFFENA=0;        //禁止FIFO功能
    RS485B_RXMODE();                         //配置RS485为接收模式
}


//发送字符
void RS485_SendOneByte(unsigned char a)
{
    while(ScibRegs.SCICTL2.bit.TXRDY==0);//发送缓冲寄存器就绪标准，1:scitxbuf准备好接受下一组要发送的数据，0:scitxbuf已满
    ScibRegs.SCITXBUF=a;

}


void RS485_Send_SimulateID(void)
{
    RS485B_TXMODE();
    RS485_SendOneByte(0x00);
    RS485_SendOneByte(0x16);
//    RS485_TXMODE();
//    RS485_SendOneByte(0xff);
//    RS485_SendOneByte(0xff);
}

void RS485_Send_MWDID(void)
{
    RS485B_TXMODE();
    //ScibRegs.SCICTL1.bit.TXWAKE =1;
    RS485_SendOneByte(0xFF);
    //ScibRegs.SCICTL1.bit.TXWAKE =0;
    RS485_SendOneByte(0x16);
}





//发送字符串
void RS485_Sendstring(unsigned char * Str)
{
    while( * Str != '\0')
    {
        RS485_SendOneByte(*(Str++));
    }
}


void RS485_SendUnsignedInt(unsigned int Senddata)
{
    RS485_SendOneByte(((Senddata>>8)&0x00ff));
    RS485_SendOneByte(((Senddata)&0x00ff));
}

void RS485_Send3byte(unsigned long Senddata)
{
    RS485_SendOneByte(((Senddata>>16)&0xff));
    RS485_SendOneByte(((Senddata>>8)&0xff));
    RS485_SendOneByte(((Senddata)&0xff));
}

void RS485_SendLong(unsigned long Senddata)
{
    RS485_SendOneByte(((Senddata>>24)&0xff));
    RS485_SendOneByte(((Senddata>>16)&0xff));
    RS485_SendOneByte(((Senddata>>8)&0xff));
    RS485_SendOneByte(((Senddata)&0xff));
}


void RS485_SendSixbytes(unsigned long long Senddata)
{
    RS485_SendOneByte(((Senddata>>40)&0xff));
    RS485_SendOneByte(((Senddata>>32)&0xff));
    RS485_SendOneByte(((Senddata>>24)&0xff));
    RS485_SendOneByte(((Senddata>>16)&0xff));
    RS485_SendOneByte(((Senddata>>8)&0xff));
    RS485_SendOneByte(((Senddata)&0xff));
}

void RS485_SendFloat(float Senddata)
{
    long CalLong = 0;
    CalLong = (long)(Senddata*100.0);
    RS485_SendOneByte(((CalLong>>24)&0xff));
    RS485_SendOneByte(((CalLong>>16)&0xff));
    RS485_SendOneByte(((CalLong>>8)&0xff));
    RS485_SendOneByte(((CalLong)&0xff));
}

void RS485_Send_BackCmd(void)
{
    unsigned int Temp,CheckSum=0x00+0x16;
    RS485_Send_SimulateID();                //发送ID

    Temp = 6 + TxBufCnt;                    //数据长度加上2个ID，2个长度，2个校验和
    CheckSum +=  ((Temp>>8)&0x00FF)  + (Temp&0x00FF);
    RS485_SendUnsignedInt(Temp);            //发送长度

    for(Temp=0;Temp<TxBufCnt;Temp++)
    {
        RS485_SendOneByte(TxBuf[Temp]);     //发送内容
        CheckSum += TxBuf[Temp]&0x00FF;
    }
    RS485_SendUnsignedInt(CheckSum);        //发送校验和
    while(ScibRegs.SCICTL2.bit.TXEMPTY==0);//等待最后一个字节发送完毕，1:发送缓冲和发送移位寄存器为空，0:有数据没有发送完
  //  DELAY_MS(5);                            //防止没有发送完毕，就更改波特率
}

void RS485_Send_Arry(unsigned char *p,unsigned char num)
{
    unsigned char i=0;
    RS485B_TXMODE();//发送模式
    for(i=0;i<num;i++)
        RS485_SendOneByte(*(p++));
    while(ScibRegs.SCICTL2.bit.TXEMPTY==0);//防止没有最后一个字节没有发送完毕，就更改波特率
 //   DELAY_MS(5);                            //防止没有最后一个字节没有发送完毕，就更改波特率
    RS485B_RXMODE();
}
unsigned char RS485_RecieveOneByte(void)
{
   RS485B_RXMODE();
   unsigned char ReceivedChar;
   while(ScibRegs.SCIRXST.bit.RXRDY!=1) {}
   ReceivedChar=ScibRegs.SCIRXBUF.all;
   return ( ReceivedChar);
}


