/*
 * DS1629_Config.c
 *
 *  Created on: 2023年6月11日
 *      Author: xikai
 */

#include "DS1629_Config.h"




static void Init_DS1629_GPIO(void)                                          //DS1629端口初始化
{
    Set_GPIO_Out(DS1629_SCL_PIN);
    Set_GPIO_Out(DS1629_SDA_PIN);
    Enable_Pullup_Gpio(DS1629_SCL_PIN);
    Disable_Pullup_Gpio(DS1629_SDA_PIN);
    Set_GPIO_High(DS1629_SCL_PIN);
    Set_GPIO_High(DS1629_SDA_PIN);
}

void DS1629_Init(void)
{

    Init_DS1629_GPIO();
    IIC_WriteThreeByte_WithStartAndStop(DS1629_SCL_PIN,DS1629_SDA_PIN, SlaveAddress_Write, Access_Config, 0x05,DS1629_SCL_HOLD_TIME);        //将温度转换模块设置为待机模式
}



/*static void DS1629_WriteTimeReg(unsigned char RegVal,unsigned char SetVal)//写时间寄存器
{
    unsigned char TempVal;
    TempVal = Decimal2BCD(SetVal);
    IIC_WriteFourByte_WithStartAndStop(DS1629_SCL_PIN,DS1629_SDA_PIN,SlaveAddress_Write,Access_Clock,RegVal,TempVal);
    asm(" RPT #29 || NOP");
}

static void DS1629_WriteSecondReg(unsigned char RegVal,unsigned char SetVal)//写时间寄存器
{
    unsigned char TempVal;
    TempVal = Second_Decimal2BCD(SetVal);
    IIC_WriteFourByte_WithStartAndStop(DS1629_SCL_PIN,DS1629_SDA_PIN,SlaveAddress_Write,Access_Clock,RegVal,TempVal);
    asm(" RPT #29 || NOP");
}*/


//读出DS1629的实时时间
void DS1629_ReadTimeAll(void)  //读出所有时钟寄存器，放置于ReadTime数组中，读出数为十进制数。
{
    unsigned char Cnti;
    IIC_WriteThreeByte_WithStartAndStop(DS1629_SCL_PIN,DS1629_SDA_PIN,SlaveAddress_Write,Access_Clock,DS1629_Seconds,DS1629_SCL_HOLD_TIME); //发送读取内部寄存器命令
    //开始读取时钟数据
    IIC_Start(DS1629_SCL_PIN,DS1629_SDA_PIN,DS1629_SCL_HOLD_TIME);
    IIC_Write_Byte(DS1629_SCL_PIN,DS1629_SDA_PIN,SlaveAddress_Read,DS1629_SCL_HOLD_TIME);            //发送设备地址+读信号
    IIC_Check_ACK(DS1629_SCL_PIN,DS1629_SDA_PIN,DS1629_SCL_HOLD_TIME);
    for(Cnti=0;Cnti<7;Cnti++)
    {
        ReadTime[Cnti] = IIC_Read_Byte(DS1629_SCL_PIN,DS1629_SDA_PIN,DS1629_SCL_HOLD_TIME);          //读回该寄存器值
        if(Cnti<6)
            IIC_Master_ACK(DS1629_SCL_PIN,DS1629_SDA_PIN,DS1629_SCL_HOLD_TIME);
        else
            IIC_Master_NO_ACK(DS1629_SCL_PIN,DS1629_SDA_PIN,DS1629_SCL_HOLD_TIME);
         DELAY_US(1);    //延时1us
    }
    IIC_Stop(DS1629_SCL_PIN,DS1629_SDA_PIN,DS1629_SCL_HOLD_TIME);//发送停止信号

    ReadTime[0] &= 0x7f;//剔除无效位的干扰
    ReadTime[1] &= 0x7f;
    ReadTime[2] &= 0x3f;
    ReadTime[3] &= 0x07;
    ReadTime[4] &= 0x3f;
    ReadTime[5] &= 0x1f;
    ReadTime[6] &= 0xff;
    for(Cnti=0;Cnti<7;Cnti++)
    {
        ReadTime[Cnti] = BCD2Decimal(ReadTime[Cnti]);//转换为10进制数
    }
}

Uint16 DS1629_ReadTemperature(void)//读取温度
{
    IIC_WritetwoByte_WithStartAndStop(DS1629_SCL_PIN,DS1629_SDA_PIN,SlaveAddress_Write,Start_Convert_T,DS1629_SCL_HOLD_TIME);
    DELAY_US(20);
  //  IIC_WritetwoByte_WithStartAndStop(DS1629_SCL_PIN,DS1629_SDA_PIN,SlaveAddress_Write,Stop_Convert_T);
    IIC_WritetwoByte_WithStartAndStop(DS1629_SCL_PIN,DS1629_SDA_PIN,SlaveAddress_Write,Read_Temperature,DS1629_SCL_HOLD_TIME); //发送读取温度命令
    //开始读取温度数据
    IIC_Start(DS1629_SCL_PIN,DS1629_SDA_PIN,DS1629_SCL_HOLD_TIME);
    IIC_Write_Byte(DS1629_SCL_PIN,DS1629_SDA_PIN,SlaveAddress_Read,DS1629_SCL_HOLD_TIME);            //发送设备地址+读信号
    Uint16 Temp;
    Temp = IIC_Read_Byte(DS1629_SCL_PIN,DS1629_SDA_PIN,DS1629_SCL_HOLD_TIME);
//    IIC_Master_ACK(DS1629_SCL_PIN,DS1629_SDA_PIN);
//    Temp = Temp<<8 + IIC_Read_Byte(DS1629_SCL_PIN,DS1629_SDA_PIN);
    IIC_Master_NO_ACK(DS1629_SCL_PIN,DS1629_SDA_PIN,DS1629_SCL_HOLD_TIME);
    IIC_Stop(DS1629_SCL_PIN,DS1629_SDA_PIN,DS1629_SCL_HOLD_TIME);//发送停止信号f;
    return Temp;
}


void DS1629_SetTimeAll(unsigned char *SetVal)//  配置所有时钟寄存器。秒分时周日月年，十进制
{
    unsigned char Cnti=0;
    unsigned char TempVal[7];
    for(Cnti=0;Cnti<7;Cnti++)//BCD码转换
    {
        TempVal[Cnti] = Decimal2BCD(SetVal[Cnti]);

    }
    IIC_Start(DS1629_SCL_PIN,DS1629_SDA_PIN,DS1629_SCL_HOLD_TIME);
    IIC_Write_Byte(DS1629_SCL_PIN,DS1629_SDA_PIN,SlaveAddress_Write,DS1629_SCL_HOLD_TIME);            //发送设备地址+写信号
    IIC_Check_ACK(DS1629_SCL_PIN,DS1629_SDA_PIN,DS1629_SCL_HOLD_TIME);
    IIC_Write_Byte(DS1629_SCL_PIN,DS1629_SDA_PIN,Access_Clock,DS1629_SCL_HOLD_TIME);            //发送时钟配置寄存器
    IIC_Check_ACK(DS1629_SCL_PIN,DS1629_SDA_PIN,DS1629_SCL_HOLD_TIME);
    IIC_Write_Byte(DS1629_SCL_PIN,DS1629_SDA_PIN,0x00,DS1629_SCL_HOLD_TIME);            //发送时钟配置寄存起始位置
    IIC_Check_ACK(DS1629_SCL_PIN,DS1629_SDA_PIN,DS1629_SCL_HOLD_TIME);
    for(Cnti=0;Cnti<7;Cnti++)
    {
        IIC_Write_Byte(DS1629_SCL_PIN,DS1629_SDA_PIN,TempVal[Cnti],DS1629_SCL_HOLD_TIME);            //发送时钟各字节内容
        IIC_Check_ACK(DS1629_SCL_PIN,DS1629_SDA_PIN,DS1629_SCL_HOLD_TIME);

    }
    IIC_Stop(DS1629_SCL_PIN,DS1629_SDA_PIN,DS1629_SCL_HOLD_TIME);                            //发送停止信号
}
