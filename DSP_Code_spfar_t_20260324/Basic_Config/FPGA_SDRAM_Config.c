/*
 * FPGA_SDRAM_Config.c
 *
 *  Created on: 2026年1月30日
 *      Author: xikai
 */

#include "FPGA_SDRAM_Config.h"

void FPGA_SDRAM_Init(void)
{
    InitXintf();                                    //初始化Xif,修改原有库函数，仅使用ZONE6，不使用USEREDY信号
    EALLOW;
    GpioCtrlRegs.GPADIR.bit.GPIO31=0;               //GPIO31 = Input，fpga准备完成信号，采样完成引脚初始化
    EDIS;
}

//写一个Uint16到FPGA接口
static void FPGA_DATA_write(unsigned int *addr_write, unsigned int writedata)
{
    *(addr_write)=writedata;
}

//从FPGA接口读取Uint16
static unsigned int FPGA_DATA_read_uint16(unsigned int *addr_read)
{
    unsigned int readdata;
    readdata=*(addr_read);
     return (readdata);
}

static signed long long FPGA_DATA_read(unsigned int *addr_read)
{
     signed long long Temp;
     Temp = FPGA_DATA_read_uint16((unsigned int *)addr_read);
     Temp = Temp<<16;
     Temp = Temp + FPGA_DATA_read_uint16((unsigned int *)(addr_read + 1));
     Temp = Temp<<16;
     Temp = Temp + FPGA_DATA_read_uint16((unsigned int *)(addr_read + 2));
     Temp = Temp<<16;
     Temp = Temp + FPGA_DATA_read_uint16((unsigned int *)(addr_read + 3));
     return Temp;
}


// 设置当前发射频率，用于FPGA进行数字锁相放大
void FPGA_Set_Freq()
{

    FPGA_DATA_write((unsigned int *)FPGA_Freq_Addr,CMD_SetFreq[state_freq]);
    DELAY_US(20);

}

//struct FPGA_data Test_Read_data;
void FPGA_Stop()
{
    FPGA_CLOSE_AMP();
    Single_Write_MCP23008(MCP23008_R1A_ADDR, SleepMode);
    DELAY_US(50);
    Single_Write_MCP23008(MCP23008_R1A_ADDR, SleepMode);
    DELAY_US(50);
    Single_Write_MCP23008(MCP23008_R1B_ADDR, SleepMode);
    DELAY_US(50);
    Single_Write_MCP23008(MCP23008_R1B_ADDR, SleepMode);
    DELAY_US(50);
    Single_Write_MCP23008(MCP23008_R1C_ADDR, SleepMode);
    DELAY_US(50);
    Single_Write_MCP23008(MCP23008_R1C_ADDR, SleepMode);
    DELAY_US(50);
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
    Set_GPIO_Low(TRAN_ANT_PIN0);
    Set_GPIO_Low(TRAN_ANT_PIN1);
    Set_GPIO_Low(TRAN_ANT_PIN2);
    DELAY_US(100);
}
void FPGA_CLOSE_AMP()
{
    FPGA_DATA_write((unsigned int *)FPGA_Amp_Addr_low,0);
    FPGA_DATA_write((unsigned int *)FPGA_Amp_Addr_high,0);
    DELAY_US(20);
}

//控制FPGA输出信号幅度
void FPGA_Set_AMP()
{

    FPGA_DATA_write((unsigned int *)FPGA_Amp_Addr_low,CMD_SetAmp[(state_freq % 4)]);    //低频的幅度
    FPGA_DATA_write((unsigned int *)FPGA_Amp_Addr_high,CMD_SetAmp[(state_freq / 4)+4]); //高频的幅度
    DELAY_US(20);
}

//从FPGA内部读取数据输出，五个频率下的接口统一，不同频率选择使用不同的数据,需要fpga缩小io高电平持续时间
void FPGA_ReadData(struct FPGA_data *FPGA_Read_data)
{
 //   struct FPGA_data FPGA_Read_data;
    int time_out=0;
    while((Get_GPIO_Value(FPGA_DATA_READY)==0)&&(time_out<(5000/MicroFrame_Num)))
    {
      time_out++;
      DELAY_US(10);
    }

    FPGA_Read_data->R1A_I = FPGA_DATA_read((unsigned int *)Data_addr);

    FPGA_Read_data->R1A_Q = FPGA_DATA_read((unsigned int *)(Data_addr + 4));

    FPGA_Read_data->R1B_I = FPGA_DATA_read((unsigned int *)(Data_addr + 8));

    FPGA_Read_data->R1B_Q = FPGA_DATA_read((unsigned int *)(Data_addr + 12));

    FPGA_Read_data->R1C_I = FPGA_DATA_read((unsigned int *)(Data_addr + 16));

    FPGA_Read_data->R1C_Q = FPGA_DATA_read((unsigned int *)(Data_addr + 20));

    FPGA_Read_data->R2A_I = FPGA_DATA_read((unsigned int *)(Data_addr + 24));

    FPGA_Read_data->R2A_Q = FPGA_DATA_read((unsigned int *)(Data_addr + 28));

    FPGA_Read_data->R2B_I = FPGA_DATA_read((unsigned int *)(Data_addr + 32));

    FPGA_Read_data->R2B_Q = FPGA_DATA_read((unsigned int *)(Data_addr + 36));

    FPGA_Read_data->R2C_I = FPGA_DATA_read((unsigned int *)(Data_addr + 40));

    FPGA_Read_data->R2C_Q = FPGA_DATA_read((unsigned int *)(Data_addr + 44));

    FPGA_Read_data->R3U_I = FPGA_DATA_read((unsigned int *)(Data_addr + 48));

    FPGA_Read_data->R3U_Q = FPGA_DATA_read((unsigned int *)(Data_addr + 52));

    FPGA_Read_data->R3D_I = FPGA_DATA_read((unsigned int *)(Data_addr + 56));

    FPGA_Read_data->R3D_Q = FPGA_DATA_read((unsigned int *)(Data_addr + 60));

    FPGA_Read_data->TX_I = FPGA_DATA_read((unsigned int *)(Data_addr + 64));

    FPGA_Read_data->TX_Q = FPGA_DATA_read((unsigned int *)(Data_addr + 68));

    Uint32 Temp = FPGA_DATA_read_uint16((unsigned int *)(Data_addr + 72));
    Temp = Temp<<16;
    Temp += FPGA_DATA_read_uint16((unsigned int *)(Data_addr + 73));

    FPGA_Read_data->GAMMA = Temp;


}

