/*
 * Public_Var.h
 *
 *  Created on: 2023年6月12日
 *      Author: xikai
 */

#ifndef BASIC_CONFIG_PUBLIC_VAR_H_
#define BASIC_CONFIG_PUBLIC_VAR_H_

/******************************头文件引用**************************/


#include <ADC_Config.h>
#include <FPGA_Data_Process.h>
#include <Frame_Buffer_Process.h>
#include <Frame_Buffer_Process.h>
#include "Basic_Config.h"
#include "SoftWare_IIC.h"
#include "GPIO_Config.h"
#include "stdio.h"
#include "math.h"
#include "CMD_Process.h"
#include "FPGA_SDRAM_Config.h"
#include "Timer_Config.h"
#include "DS1629_Config.h"
#include "MCP23008_Config.h"
#include "EXT_FLASH_Config.h"
#include "F28335_FLASH.h"
#include "RS485_Config.h"
#include "Public_Var.h"
#include "Data_Process.h"
#include "Mode_Config.h"
#include "CMD_Process.h"
#include "Time_Process.h"
#include "GAMMA_Count.h"
#include "CAN_Broadcast.h"
#include "Auto_Zoom_Process.h"
#include "MS578H_UART_Config.h"

/*******************版本信息*******************************/
extern unsigned char Ins_ID;                                                        //仪器号
extern unsigned char DSP_Ver;                                                       //DSP程序版本号
extern unsigned char FPGA_Ver;                                                      //FPGA程序版本号
/***********************************************************/
extern unsigned int watch_paramter;
/******************工作状态********************************/
//Flag
extern volatile unsigned char Frame_Flag;                                           // 帧结束标志位
extern volatile unsigned char MicroFrame_Flag;                                      // 小帧结束标志位
//当前状态
extern volatile unsigned char state_main_last;                                      // 上一次的主工作状态
extern volatile unsigned char state_main;                                           // 当前主工作状态
extern volatile unsigned char SamplingFlag;                                         //正在采集标记，正在采集=1，停止采集=0
extern volatile unsigned long long SamplePoint_Log;                                 //已经采集点数，上电时从FLASH内查询
//预设置参数
extern unsigned long MicroFrame_Interval;                                           //小帧的间隔
extern unsigned int MicroFrame_Sum;                                                 //小帧的总数
extern unsigned char Frame_Interval;                                                //帧间隔，5s
extern unsigned char MicroFrame_Num;
extern unsigned int Angle_Diff;                                                     //角差
extern unsigned char Sample_gama;                                               //伽马每25秒记录一组数据


//延时启动相关配置
extern unsigned char DelayTimeFinishFlag;                                           //是否延时启动，默认不延时启动
extern unsigned int The_End_Time[7];                                                //目标时钟数据，秒分时周日月年，十进制，上电后从FLash读入
/***********************************************************/

/**********************串口交互*******************************/

#define  RxBufLength 480                                                            //RS485接收数组上限
#define  TxBufLenfgth 1000                                                          //RS485发送数组上限
extern unsigned char baudflag;                                                      //当前波特率标志位,1为9600 2为115200 3为468000
extern unsigned char setbaudflag;
extern volatile unsigned char  Framedata_rxflag;                                    //帧结束标志
extern unsigned char RxBufCnt;                                                      //当前接收字符数
extern unsigned char RxBuf[RxBufLength];                                            //RS485接收Buff
extern unsigned int TxBufCnt;                                                       //当前发送Buf字符数
extern unsigned char TxBuf[TxBufLenfgth];                                           //RS485发送Buff
extern unsigned CMD_Error;                                                          //如果此值为1，代表串口命令错误，不回复
/**********************************************************/

/******************交互与计算相关参数***********************************/

#define PI              3.1415926                                                   //PI
#define Samplenumber    250                                                         //方位计采样累计数
#define R1A_Threshold_Low 500                                                        //R1A低门槛调整电压 50mV
#define R1A_Threshold_High 2500                                                        //R1A高门槛调整电压 2500mV
#define R2A_Threshold_Low 500                                                        //R2A低门槛调整电压 50mV
#define R2A_Threshold_High 2500                                                        //R2A高门槛调整电压 2500mV
extern unsigned int GravityAngleMul10;
/**************************************************************/


/********************内部FLASH配置******************************/
#define F28335_Write_Addr   0x00330000                                              //写入FLASHB
#define PLLCR_VALUE         0x000A                                                  // SYSCLKOUT = (OSCLK*10)/2
/*************************************************************/


/********************外部FLASH配置参数****************************/

#define BlockStartAddr          0
//#define BlockSettingsAddr       6
#define BlockSettingsAddr       4095
#define PageSettingsAddr        127
//读写指针相关参数
extern unsigned int Write_BlockAddrCnt;                                               //外部FLASH读写Block指针
extern unsigned char Write_PageAddrCnt;                                               //外部FLASH读写Page指针
extern unsigned int Read_BlockAddrCnt;
extern unsigned char Read_PageAddrCnt;
extern unsigned long long ReadMemoryFrame;
extern unsigned long long ReadMemoryFrameEnd;
/*************************************************************/



/**********************与FPGA_SDRAM交互数据********************/

struct FPGA_data
{                                       // F1       F2      F3      F4      F5
    signed long long R1A_Q;             // 1        1       1
    signed long long R1A_I;             // 1        1       1
    signed long long R1B_Q;             // 1        1       1
    signed long long R1B_I;             // 1        1       1
    signed long long R1C_Q;             // 1        1       1
    signed long long R1C_I;             // 1        1       1
    signed long long R2A_Q;             //          1       1       1
    signed long long R2A_I;             //          1       1       1
    signed long long R2B_Q;             //          1       1       1
    signed long long R2B_I;             //          1       1       1
    signed long long R2C_Q;             //          1       1       1
    signed long long R2C_I;             //          1       1       1
    signed long long R3U_Q;             //                          1       1
    signed long long R3U_I;             //                          1       1
    signed long long R3D_Q;             //                          1       1
    signed long long R3D_I;             //                          1       1
    signed long long TX_Q;             // 1        1       1       1       1
    signed long long TX_I;             // 1        1       1       1       1
    unsigned long    GAMMA;
};

#define FPGA_Freq_Addr    0x00100000            //XINTF选择zone6起始地址0x1000000,终止地址0x2000000
#define FPGA_Amp_Addr     0x00100001
#define Data_addr         0x0010001E                                            //从FPGA内部SDRAM读取数据的地址

//extern float SampleData[288];                                                   //从FPGA读取到的数据的累加，一共16个扇区，每个扇区16种数据
extern signed long long Res_sector[288];                                           //从FPGA读取到的数据的累加，一共16个扇区，每个扇区9*2种数据

extern float AD4020_Scale_Coefficient;                                                 //归一化系数，换算为真实幅度
extern float ADS9218_Scale_Coefficient;                                                 //归一化系数，换算为真实幅度
extern const unsigned int CMD_SetFreq[5];                                                   //控制FPGA数据采集，写入的数据命令
extern  unsigned int CMD_SetAmp[5];                                                    //控制FPGA输出幅度
/*************************************************************/


/***********************控制MCP23008*************************/
extern unsigned char CMD_SetMCP23008[5];                                        //控制MCP23008输出
/************************************************************/

/**********************GAMMA计数*****************************/

#define ADC_REF    2048                                                       //DSP ADC 参考电压
extern volatile unsigned int GAMMA_Current_Count;                               //GAMMA实时计数值
/***********************************************************/


/***********************存储帧结构**********************************/
#define MAX_Buffer_LEN    1050
extern unsigned int  Frame_Length[5];                                                  //存储五个频率下的帧长度
extern unsigned char PageBuffer[MAX_Buffer_LEN];                                       //存储一帧数据,有冗余
extern unsigned char DataFrameHeader[2];                                               //帧头和ID 在帧内占2字节
extern unsigned int pagecnt;                                                           //存储当前帧Buffer写指针 在帧内占2字节
extern volatile unsigned char state_freq;                                                       //存放当前工作频率 在帧内占1字节
extern unsigned char ReadTime[7];                                                      //读出时钟数据，秒分时周日月年，十进制 在帧内占4字节
extern unsigned char Temperature;                                                      //存放当前温度 在帧内占1字节
extern unsigned char shockx;                                                           //x震动 在帧内占1字节
extern unsigned char shocky;                                                           //y震动 在帧内占1字节
extern unsigned char shockz;                                                           //y震动 在帧内占1字节
extern unsigned int  Temp_Lm135;
extern unsigned char Rpm;                                                              //转速 在帧内占1字节
extern unsigned int Azi;                                                               //方位 在帧内占2字节
extern unsigned int lnc;                                                              //井斜 在帧内占2字节
extern unsigned int Pwd;                                                              //压力 在帧内占1字节
extern unsigned int MicroFrame_Cnt;                                                    //小帧的数量 在帧内占2字节
extern unsigned char Auto_Zoom_R1A[3];                                                 //R1A默认增益为3档位
extern unsigned char Auto_Zoom_R1B[3];                                                 //R1B默认增益为3档位
extern unsigned char Auto_Zoom_R1C[3];                                                 //R1C默认增益为3档位
extern unsigned char Auto_Zoom_R2A[3];                                                 //R2A默认增益为3档位
extern unsigned char Auto_Zoom_R2B[3];                                                 //R2A默认增益为3档位
extern unsigned char Auto_Zoom_R2C[3];                                                 //R2A默认增益为3档位
extern unsigned char Auto_Zoom_R3U[2];                                                 //R3U默认增益为3档位
extern unsigned char Auto_Zoom_R3D[2];                                                 //R3D默认增益为3档位
extern unsigned char Zoom_R1A;                                                         //R1A增益 在帧内占1字节
extern unsigned char Zoom_R1B;                                                         //R1B增益 在帧内占1字节
extern unsigned char Zoom_R1C;                                                         //R1C增益 在帧内占1字节
extern unsigned char Zoom_R2A;                                                         //R2A增益 在帧内占1字节
extern unsigned char Zoom_R2B;                                                         //R2B增益 在帧内占1字节
extern unsigned char Zoom_R2C;                                                         //R2C增益 在帧内占1字节
extern unsigned char Zoom_R3U;                                                         //R3U增益
extern unsigned char Zoom_R3D;                                                         //R3D增益
extern unsigned char Zoom_TX;                                                         //R4A增益
extern float Max_avg_R1A_Q;                                                           //R1A天线电阻率实部平均值最大值
extern float Max_avg_R1A_I;                                                           //R1A天线电阻率虚部平均值最大值
extern float Max_avg_R2A_Q;                                                           //R2A天线电阻率实部平均值最大值
extern float Max_avg_R2A_I;                                                           //R2A天线电阻率虚部平均值最大值
/*************************************以下参数各个频率选择*********************************************/
extern float Average_R1A_Q;                                                           //R1A天线电阻率实部平均值
extern float Average_R1A_I;                                                           //R1A天线电阻率虚部平均值
extern float Average_R1B_Q;                                                           //R1B天线电阻率实部平均值
extern float Average_R1B_I;                                                           //R1B天线电阻率虚部平均值
extern float Average_R1C_Q;                                                           //R1C天线电阻率实部平均值
extern float Average_R1C_I;                                                           //R1C天线电阻率虚部平均值
extern float Average_R2A_Q;                                                           //R2A天线电阻率实部平均值
extern float Average_R2A_I;                                                           //R2A天线电阻率虚部平均值
extern float Average_R2B_Q;                                                           //R2B天线电阻率实部平均值
extern float Average_R2B_I;                                                           //R2B天线电阻率虚部平均值
extern float Average_R2C_Q;                                                           //R2C天线电阻率实部平均值
extern float Average_R2C_I;                                                           //R2C天线电阻率虚部平均值
extern float Average_R3U_Q;                                                           //R3U天线电阻率实部平均值
extern float Average_R3U_I;                                                           //R3U天线电阻率虚部平均值
extern float Average_R3D_Q;                                                           //R3D天线电阻率实部平均值
extern float Average_R3D_I;                                                           //R3D天线电阻率虚部平均值
extern float Average_TX_Q;                                                           //R4A天线电阻率实部平均值
extern float Average_TX_I;                                                           //R4A天线电阻率虚部平均值
/******************************************************************************************************/
extern unsigned int Angle_sector_res[16];                                                  //每一个扇区内数据累加的次数 在帧内占2个字节
extern unsigned int GR_sector[16];                                                    //每一个扇区内伽马信号的个数 在帧内占2个字节
extern unsigned int GR_sector_reg[16];
extern unsigned int Angle_sector_gama[16];
extern unsigned int Angle_sector_gama_reg[16];
extern unsigned int GRprobe_total;                                                      //采样总伽马4s累计平均值 在帧内占2字节
extern unsigned int GRprobe_total_reg;
extern unsigned int GRprobe_sector;                                                      //采样扇区伽马累计平均值 在帧内占2字节
extern volatile Uint16 Cur_sector;                                                     //当前所在sector
/*************************************以下参数各个频率选择*********************************************/
extern float R1A_Q[16];                                                               //R1A天线电阻率实部，16个扇区
extern float R1A_I[16];                                                               //R1A天线电阻率虚部，16个扇区
extern float R1B_Q[16];                                                               //R1B天线电阻率实部，16个扇区
extern float R1B_I[16];                                                               //R1B天线电阻率虚部，16个扇区
extern float R1C_Q[16];                                                               //R1C天线电阻率实部，16个扇区
extern float R1C_I[16];                                                               //R1C天线电阻率虚部，16个扇区
extern float R2A_Q[16];                                                               //R2A天线电阻率实部，16个扇区
extern float R2A_I[16];                                                               //R2A天线电阻率虚部，16个扇区
extern float R2B_Q[16];                                                               //R2B天线电阻率实部，16个扇区
extern float R2B_I[16];                                                               //R2B天线电阻率虚部，16个扇区
extern float R2C_Q[16];                                                               //R2C天线电阻率实部，16个扇区
extern float R2C_I[16];                                                               //R2C天线电阻率虚部，16个扇区
extern float R3U_Q[16];                                                               //R3U天线电阻率实部，16个扇区
extern float R3U_I[16];                                                               //R3U天线电阻率虚部，16个扇区
extern float R3D_Q[16];                                                               //R3D天线电阻率实部，16个扇区
extern float R3D_I[16];                                                               //R3D天线电阻率虚部，16个扇区
extern float TX_Q[16];                                                               //R3D天线电阻率实部，16个扇区
extern float TX_I[16];                                                               //R3D天线电阻率虚部，16个扇区
/******************************************************************************************************/
extern unsigned int  FlashSendSum;                                                    //存放帧数据校验和 在帧内占2字节
/****************************************************************/


#endif /* BASIC_CONFIG_PUBLIC_VAR_H_ */
