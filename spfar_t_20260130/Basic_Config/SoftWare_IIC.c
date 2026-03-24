/*
 * SoftWare_IIC.c
 *
 *  Created on: 2024年10月16日
 *  Author: CHENHUI
 */

#include "SoftWare_IIC.h"

void IIC_Hold_Time(Uint16 hold_time)

{
    unsigned int i=0;
    for(i=0;i<hold_time;i++);
}
/*
 * 设置I2C起始条件
 * SCL高电平期间，SDA产生下降沿
 */
void IIC_Start(Uint32 SCL_GPIO,Uint32 SDA_GPIO,Uint16 hold_time)  //设置I2C起始条件
{
    Set_GPIO_Out(SCL_GPIO);//SCL输出状态

    Set_GPIO_Out(SDA_GPIO);//SDA输出状态

    Set_GPIO_High(SCL_GPIO);//SCL上拉高电平

    Set_GPIO_High(SDA_GPIO);//SDA上拉高电平

    IIC_Hold_Time(hold_time);//hold_time=50,620个时钟周期，scl约4*2=8us

    Set_GPIO_Low(SDA_GPIO);//SCL拉高期间，SDA由高往低跳变，产生起始条件

    IIC_Hold_Time(hold_time);//保持一下

    Set_GPIO_Low(SCL_GPIO);//SCL拉低，为后续位发送创造条件

    IIC_Hold_Time(hold_time);//保持一下

}
/*
 * 设置I2C结束条件
 *  SCL高电平期间，SDA产生上升沿
 */
void IIC_Stop(Uint32 SCL_GPIO,Uint32 SDA_GPIO,Uint16 hold_time)//设置I2C结束条件
{
    Set_GPIO_Out(SDA_GPIO);//SDA 调整为输出状态

    Set_GPIO_Low(SCL_GPIO);//确保SCL为低，可以更改SDA值，避免误触发

    Set_GPIO_Low(SDA_GPIO);//SDA 调整为低，为产生上升沿做好准备

    IIC_Hold_Time(hold_time);//保持一下

    Set_GPIO_High(SCL_GPIO);//SCL 跳变为高，为高电平期间产生SDA上升沿创造条件

    IIC_Hold_Time(hold_time);//保持一下

    Set_GPIO_High(SDA_GPIO);//SDA跳变为高，产生上升沿，产生结束条件

    IIC_Hold_Time(hold_time);//保持一下
}
/*
 * 主机产生应答信号条件
 * 第9个SCL高电平期之前，SDA拉低，保证第9个SCL高电平期间，从机检测SDA为低
 */

void IIC_Master_ACK(Uint32 SCL_GPIO,Uint32 SDA_GPIO,Uint16 hold_time)//主机产生应答信号条件
{
    Set_GPIO_Out(SDA_GPIO);//SDA改为输出状态

    Set_GPIO_Low(SDA_GPIO);//此时SCL依旧为低，SDA调整为低

    IIC_Hold_Time(hold_time);//保持一下

    Set_GPIO_High(SCL_GPIO);//第9个SCL拉高，此时SDA为低，产生应答条件

    IIC_Hold_Time(hold_time);//保持一下

    Set_GPIO_Low(SCL_GPIO);//SCL拉低

 //   Set_GPIO_In(SDA_GPIO);//SDA改为输入状态，希望从机继续发送后续字节,其实可以不用，在读字节程序中已有相关设计

  //  Set_GPIO_High(SDA_GPIO);//释放sda

 //   IIC_Hold_Time(hold_time);
 }

/*
 * 主机产生应答信号条件
 * 第9个SCL高电平期之前，SDA拉低，保证第9个SCL高电平期间，从机检测SDA为低
 */
void IIC_Master_NO_ACK(Uint32 SCL_GPIO,Uint32 SDA_GPIO,Uint16 hold_time)
{
    Set_GPIO_Out(SDA_GPIO);//SDA改为输出状态

    Set_GPIO_High(SDA_GPIO);//此时SCL依旧为低，SDA调整为高

    IIC_Hold_Time(hold_time);//保持一下

    Set_GPIO_High(SCL_GPIO);//第9个SCL拉高，此时SDA为高，产生非应答条件

    IIC_Hold_Time(hold_time);//保持一下

    Set_GPIO_Low(SCL_GPIO);
  //  IIC_Hold_Time(hold_time);
   // Set_GPIO_Low(SDA_GPIO);
}
/*
 * 应答检测
 * 应答信号来自于从机，在scl高电平期间，从机将sda拉至低电平即为应答信号
 */

unsigned char IIC_Check_ACK(Uint32 SCL_GPIO,Uint32 SDA_GPIO,Uint16 hold_time) //检查从机是否有应答
{
    unsigned int errtime=255;                               //因故障接收方无ACK，超时值为255。*20ns=~5us?

    unsigned char AckFlag;

    Set_GPIO_In(SDA_GPIO);//SDA改为输入状态

  //  Set_GPIO_High(SDA_GPIO);//此时SCL为低电平，dsp释放sda总线

    IIC_Hold_Time(hold_time);//保持一下
  //  DELAY_US(1);//稍微延迟2us

    Set_GPIO_High(SCL_GPIO);//SCL拉高，为高电平期间检测SDA创造条件


    DELAY_US(1);//稍微延迟2us

    AckFlag=Get_GPIO_Value(SDA_GPIO);

    while(AckFlag)                          //SDA出现为0时,则为有应答
    {
        AckFlag=Get_GPIO_Value(SDA_GPIO);

        errtime--;

        if (!errtime)                       //errtime是否为0
        {
            Set_GPIO_Out(SDA_GPIO);         //SDA口设为输出

            Set_GPIO_Low(SCL_GPIO);

            return FALSE;                   //返回为0(失败)
        }
    }
        Set_GPIO_Out(SDA_GPIO);             //SDA口设为输出

        Set_GPIO_Low(SCL_GPIO);

        return TRUE;           //返回为1
}

/*
 * 主机按位发送1个字节，共计8位，由高到低
 */
void IIC_Write_Byte(Uint32 SCL_GPIO,Uint32 SDA_GPIO,Uint8 Data,Uint16 hold_time) //主机发送一个字节
{
    unsigned char Write_i;

    for(Write_i=8; Write_i>0;Write_i--)
    {
        if((Data&0x80)==0x80)    //发1
        {
            Set_GPIO_High(SDA_GPIO);//此时SCL为低，SDA拉高

            IIC_Hold_Time(hold_time);//保持一下

            Set_GPIO_High(SCL_GPIO);//SCL拉高

            IIC_Hold_Time(hold_time);//保持一下

            Set_GPIO_Low(SCL_GPIO);//SCL拉低，为后续位发送创造条件

            IIC_Hold_Time(hold_time);
        }
        else                    //发0
        {
            Set_GPIO_Low(SDA_GPIO);//此时SCL为低，SDA拉低

            IIC_Hold_Time(hold_time);

            Set_GPIO_High(SCL_GPIO);

            IIC_Hold_Time(hold_time);

            Set_GPIO_Low(SCL_GPIO);

            IIC_Hold_Time(hold_time);
        }
        Data<<=1;//左移
    }
}
/*
 * 主机按位接收1个字节，共计8位，由高到低
 */
unsigned char IIC_Read_Byte(Uint32 SCL_GPIO,Uint32 SDA_GPIO,Uint16 hold_time) //主机接收一个字节
{
    unsigned char Read_i,temp=0;

//    Set_GPIO_High(SDA_GPIO);
    Set_GPIO_In(SDA_GPIO);//SDA调整为输入方向

//    DELAY_US(1);//稍微延迟2us

    for(Read_i=0;Read_i<8;Read_i++)
    {
        Set_GPIO_Low(SCL_GPIO);//确保 SCL 为低

        IIC_Hold_Time(hold_time);//等待从机更改SDA数据

        Set_GPIO_High(SCL_GPIO);//调整 SCL为高，为读SDA总线数据做好准备

        IIC_Hold_Time(hold_time);//保持一下

        if(Get_GPIO_Value(SDA_GPIO)==1)//读1，进行移位，读0，保持
        {
            temp|=(0x01<<(7-Read_i));
        }

        IIC_Hold_Time(hold_time);
    }
    Set_GPIO_Low(SCL_GPIO);
    Set_GPIO_Out(SDA_GPIO);
    return (temp);
}
void IIC_WritetwoByte_WithStartAndStop(Uint32 SCL_GPIO,Uint32 SDA_GPIO,Uint8 Device_Address,Uint8 Reg_Address,Uint16 hold_time)
{
    IIC_Start(SCL_GPIO,SDA_GPIO,hold_time);                                       //起始信号
    IIC_Write_Byte(SCL_GPIO,SDA_GPIO,Device_Address,hold_time);                   //发送设备地址+写信号
    IIC_Check_ACK(SCL_GPIO,SDA_GPIO,hold_time);
    IIC_Write_Byte(SCL_GPIO,SDA_GPIO,Reg_Address,hold_time);                      //内部寄存器地址
    IIC_Check_ACK(SCL_GPIO,SDA_GPIO,hold_time);
    IIC_Stop(SCL_GPIO,SDA_GPIO,hold_time);                                        //发送停止
}
void IIC_WriteThreeByte_WithStartAndStop(Uint32 SCL_GPIO,Uint32 SDA_GPIO,Uint8 Device_Address,Uint8 Reg_Address,Uint8 Reg_Data,Uint16 hold_time)
{
    IIC_Start(SCL_GPIO,SDA_GPIO,hold_time);                                       //起始信号
    IIC_Write_Byte(SCL_GPIO,SDA_GPIO,Device_Address,hold_time);                   //发送设备地址+写信号
    IIC_Check_ACK(SCL_GPIO,SDA_GPIO,hold_time);
    IIC_Write_Byte(SCL_GPIO,SDA_GPIO,Reg_Address,hold_time);                      //内部寄存器地址
    IIC_Check_ACK(SCL_GPIO,SDA_GPIO,hold_time);
    IIC_Write_Byte(SCL_GPIO,SDA_GPIO,Reg_Data,hold_time);                         //内部寄存器赋值数据
    IIC_Check_ACK(SCL_GPIO,SDA_GPIO,hold_time);
    IIC_Stop(SCL_GPIO,SDA_GPIO,hold_time);                                        //发送停止
}

void IIC_WriteFourByte_WithStartAndStop(Uint32 SCL_GPIO,Uint32 SDA_GPIO,Uint8 Device_Address,Uint8 command,Uint8 Reg_Address,Uint8 Reg_Data,Uint16 hold_time)
{
    IIC_Start(SCL_GPIO,SDA_GPIO,hold_time);                            //起始信号
    IIC_Write_Byte(SCL_GPIO,SDA_GPIO,Device_Address,hold_time);       //发送设备地址+写信号
    IIC_Check_ACK(SCL_GPIO,SDA_GPIO,hold_time);
    IIC_Write_Byte(SCL_GPIO,SDA_GPIO,command,hold_time);
    IIC_Check_ACK(SCL_GPIO,SDA_GPIO,hold_time);
    IIC_Write_Byte(SCL_GPIO,SDA_GPIO,Reg_Address,hold_time);          //内部寄存器地址
    IIC_Check_ACK(SCL_GPIO,SDA_GPIO,hold_time);
    IIC_Write_Byte(SCL_GPIO,SDA_GPIO,Reg_Data,hold_time);             //内部寄存器赋值数据
    IIC_Check_ACK(SCL_GPIO,SDA_GPIO,hold_time);
    IIC_Stop(SCL_GPIO,SDA_GPIO,hold_time);                            //发送停止信号
}
