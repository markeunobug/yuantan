/*
 * MCP23008_Config.c
 *
 *  Created on: 2023年6月11日
 *      Author: xikai
 */
#include "MCP23008_Config.h"

unsigned char SleepMode = 0x5A;

unsigned char Re_Ant_Add_Zoom=0;

#define MCP23008_SCL_SET GpioDataRegs.GPASET.bit.GPIO0     //SCL置1
#define MCP23008_SCL_CLR GpioDataRegs.GPACLEAR.bit.GPIO0   //SCL清零
#define MCP23008_SCL_DIR GpioCtrlRegs.GPADIR.bit.GPIO0     //SCL方向

#define MCP23008_SDA_DAT GpioDataRegs.GPADAT.bit.GPIO1      //SDA状态
#define MCP23008_SDA_SET GpioDataRegs.GPASET.bit.GPIO1      //SDA置1
#define MCP23008_SDA_CLR GpioDataRegs.GPACLEAR.bit.GPIO1   //SDA清零
#define MCP23008_SDA_DIR GpioCtrlRegs.GPADIR.bit.GPIO1     //SDA方向

void MCP_I2CStart(void)  //设置I2C起始条件
{
    Set_GPIO_Out(MCP23008_SCL_PIN);
    Set_GPIO_Out(MCP23008_SDA_PIN);
    MCP23008_SCL_SET=1;
 //   IIC_Delay(1);
/*
    MCP23008_SDA_SET=1;
    asm(" RPT #499 || NOP");//等待30个空指令
*/
    MCP23008_SDA_CLR=1;
    asm(" RPT #99 || NOP");
    MCP23008_SCL_CLR=1;
    asm(" RPT #99 || NOP");

}

void MCP_I2CStop(void)//设置I2C结束条件
{
    MCP23008_SDA_CLR=1;
    asm(" RPT #99 || NOP");
    MCP23008_SCL_SET=1;
    asm(" RPT #99 || NOP");
    MCP23008_SDA_SET=1;
    asm(" RPT #99 || NOP");
    MCP23008_SCL_CLR=1;////改
    asm(" RPT #99 || NOP");
}

void MCP23008_Write_Byte(unsigned char Data) //主机发送一个字节
{
    unsigned char Write_i,temp_flag=0;
    for(Write_i=8; Write_i>0;Write_i--)
    {
        if((Data&0x80)==0x80)    //8位，每次发送一位
        {
            MCP23008_SDA_SET=1;
            asm(" RPT #150 || NOP");
            MCP23008_SCL_SET=1;
            asm(" RPT #99 || NOP");
            MCP23008_SCL_CLR=1;
            asm(" RPT #99 || NOP");
        }
        else
        {
            MCP23008_SDA_CLR=1;
            asm(" RPT #99 || NOP");
            MCP23008_SCL_SET=1;
            asm(" RPT #99 || NOP");
            MCP23008_SCL_CLR=1;
            asm(" RPT #99 || NOP");
        }
        Data<<=1;
    }
/*  MCP23008_SDA_SET=1;//改
    asm(" RPT #99 || NOP");*/
    MCP23008_SDA_CLR=1;//改
    asm(" RPT #60 || NOP");
    MCP23008_SCL_SET=1;//改
    asm(" RPT #59 || NOP");//改
    MCP23008_SCL_CLR=1;//改
    //temp_flag=EEPROM_MCPCheck();改
    if(temp_flag==FALSE)
         {
           return;
         }
}

void MCP23008_Init(void)  //MCP23008初始化
{
    Enable_Pullup_Gpio(MCP23008_SCL_PIN);
    Disable_Pullup_Gpio(MCP23008_SDA_PIN);
    Set_GPIO_Out(MCP23008_SCL_PIN);
    Set_GPIO_Out(MCP23008_SDA_PIN);
    Set_GPIO_High(MCP23008_SCL_PIN);
    Set_GPIO_High(MCP23008_SDA_PIN);
}

void Single_Write_MCP23008(unsigned char Reg_Address,unsigned char Reg_Data)
{
    unsigned char sum=0;
    sum = Reg_Address + Reg_Data;
    IIC_WriteThreeByte_WithStartAndStop(MCP23008_SCL_PIN,MCP23008_SDA_PIN,Reg_Address,Reg_Data,sum,MCP23008_SCL_HOLD_TIME);
}

void MCP23008_Set_Amp()
{
    switch (state_freq)
    {
        case 0:
        {
            Zoom_R1A=Auto_Zoom_R1A[0];
            Zoom_R1B=Auto_Zoom_R1B[0];
            Zoom_R1C=Auto_Zoom_R1C[0];
            Single_Write_MCP23008(MCP23008_R2A_ADDR, SleepMode);
            DELAY_US(50);
            Single_Write_MCP23008(MCP23008_R2A_ADDR, SleepMode);
            DELAY_US(50);
            Single_Write_MCP23008(MCP23008_R2B_ADDR, SleepMode);
            DELAY_US(50);
            Single_Write_MCP23008(MCP23008_R2B_ADDR, SleepMode);
            DELAY_US(50);
            Single_Write_MCP23008(MCP23008_R2C_ADDR, SleepMode);
            DELAY_US(50);
            Single_Write_MCP23008(MCP23008_R2C_ADDR, SleepMode);
            DELAY_US(50);
            Single_Write_MCP23008(MCP23008_R3U_ADDR, SleepMode);
            DELAY_US(50);
            Single_Write_MCP23008(MCP23008_R3U_ADDR, SleepMode);
            DELAY_US(50);
            Single_Write_MCP23008(MCP23008_R3D_ADDR, SleepMode);
            DELAY_US(50);
            Single_Write_MCP23008(MCP23008_R3D_ADDR, SleepMode);
            DELAY_US(50);
            Re_Ant_Add_Zoom=0x10|Zoom_R1A;
            Single_Write_MCP23008(MCP23008_R1A_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);
            Single_Write_MCP23008(MCP23008_R1A_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);
            Re_Ant_Add_Zoom=0x10|Zoom_R1B;
            Single_Write_MCP23008(MCP23008_R1B_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);
            Single_Write_MCP23008(MCP23008_R1B_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);
            Re_Ant_Add_Zoom=0x10|Zoom_R1C;
            Single_Write_MCP23008(MCP23008_R1C_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);
            Single_Write_MCP23008(MCP23008_R1C_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);
            break;
        }
        case 1:
        {
            Zoom_R1A=Auto_Zoom_R1A[1];
            Zoom_R1B=Auto_Zoom_R1B[1];
            Zoom_R1C=Auto_Zoom_R1C[1];
            Zoom_R2A=Auto_Zoom_R2A[0];
            Zoom_R2B=Auto_Zoom_R2B[0];
            Zoom_R2C=Auto_Zoom_R2C[0];
            Single_Write_MCP23008(MCP23008_R3U_ADDR, SleepMode);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R3U_ADDR, SleepMode);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R3D_ADDR, SleepMode);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R3D_ADDR, SleepMode);
            DELAY_US(50);  //延时0.5ms
            Re_Ant_Add_Zoom=0x20|Zoom_R1A;
            Single_Write_MCP23008(MCP23008_R1A_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R1A_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Re_Ant_Add_Zoom=0x20|Zoom_R1B;
            Single_Write_MCP23008(MCP23008_R1B_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R1B_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Re_Ant_Add_Zoom=0x20|Zoom_R1C;
            Single_Write_MCP23008(MCP23008_R1C_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R1C_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Re_Ant_Add_Zoom=0x10|Zoom_R2A;
            Single_Write_MCP23008(MCP23008_R2A_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R2A_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Re_Ant_Add_Zoom=0x10|Zoom_R2B;
            Single_Write_MCP23008(MCP23008_R2B_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R2B_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Re_Ant_Add_Zoom=0x10|Zoom_R2C;
            Single_Write_MCP23008(MCP23008_R2C_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R2C_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            break;
        }
        case 2:
        {
            Zoom_R1A=Auto_Zoom_R1A[2];
            Zoom_R1B=Auto_Zoom_R1B[2];
            Zoom_R1C=Auto_Zoom_R1C[2];
            Zoom_R2A=Auto_Zoom_R2A[1];
            Zoom_R2B=Auto_Zoom_R2B[1];
            Zoom_R2C=Auto_Zoom_R2C[1];
            Single_Write_MCP23008(MCP23008_R3U_ADDR, SleepMode);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R3U_ADDR, SleepMode);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R3D_ADDR, SleepMode);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R3D_ADDR, SleepMode);
            DELAY_US(50);  //延时0.5ms
            Re_Ant_Add_Zoom=0x30|Zoom_R1A;
            Single_Write_MCP23008(MCP23008_R1A_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R1A_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Re_Ant_Add_Zoom=0x30|Zoom_R1B;
            Single_Write_MCP23008(MCP23008_R1B_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R1B_ADDR, Re_Ant_Add_Zoom);
            Re_Ant_Add_Zoom=0x30|Zoom_R1C;
            Single_Write_MCP23008(MCP23008_R1C_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R1C_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Re_Ant_Add_Zoom=0x20|Zoom_R2A;
            Single_Write_MCP23008(MCP23008_R2A_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R2A_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Re_Ant_Add_Zoom=0x20|Zoom_R2B;
            Single_Write_MCP23008(MCP23008_R2B_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R2B_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Re_Ant_Add_Zoom=0x20|Zoom_R2C;
            Single_Write_MCP23008(MCP23008_R2C_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R2C_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            break;
        }
        case 3:
        {
            Zoom_R2A=Auto_Zoom_R2A[2];
            Zoom_R2B=Auto_Zoom_R2B[2];
            Zoom_R2C=Auto_Zoom_R2C[2];
            Zoom_R3U=Auto_Zoom_R3U[0];
            Zoom_R3D=Auto_Zoom_R3D[0];
            Single_Write_MCP23008(MCP23008_R1A_ADDR, SleepMode);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R1A_ADDR, SleepMode);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R1B_ADDR, SleepMode);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R1B_ADDR, SleepMode);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R1C_ADDR, SleepMode);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R1C_ADDR, SleepMode);
            DELAY_US(50);  //延时0.5ms
            Re_Ant_Add_Zoom=0x30|Zoom_R2A;
            Single_Write_MCP23008(MCP23008_R2A_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R2A_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Re_Ant_Add_Zoom=0x30|Zoom_R2B;
            Single_Write_MCP23008(MCP23008_R2B_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R2B_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Re_Ant_Add_Zoom=0x30|Zoom_R2C;
            Single_Write_MCP23008(MCP23008_R2C_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R2C_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Re_Ant_Add_Zoom=0x20|Zoom_R3U;
            Single_Write_MCP23008(MCP23008_R3U_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R3U_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Re_Ant_Add_Zoom=0x20|Zoom_R3D;
            Single_Write_MCP23008(MCP23008_R3D_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R3D_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            break;
        }
        case 4:
        {
            Zoom_R3U=Auto_Zoom_R3U[1];
            Zoom_R3D=Auto_Zoom_R3D[1];
            Single_Write_MCP23008(MCP23008_R1A_ADDR, SleepMode);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R1A_ADDR, SleepMode);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R1B_ADDR, SleepMode);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R1B_ADDR, SleepMode);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R1C_ADDR, SleepMode);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R1C_ADDR, SleepMode);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R2A_ADDR, SleepMode);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R2A_ADDR, SleepMode);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R2B_ADDR, SleepMode);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R2B_ADDR, SleepMode);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R2C_ADDR, SleepMode);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R2C_ADDR, SleepMode);
            DELAY_US(50);  //延时0.5ms
            Re_Ant_Add_Zoom=0x30|Zoom_R3U;
            Single_Write_MCP23008(MCP23008_R3U_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R3U_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Re_Ant_Add_Zoom=0x30|Zoom_R3D;
            Single_Write_MCP23008(MCP23008_R3D_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            Single_Write_MCP23008(MCP23008_R3D_ADDR, Re_Ant_Add_Zoom);
            DELAY_US(50);  //延时0.5ms
            break;
        }
        default:
        {
            break;
        }
    }
}

