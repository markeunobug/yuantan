/*
 * Public_Var.c
 *
 *  Created on: 2023年6月12日
 *      Author: xikai
 */

#include "Public_Var.h"
/*******************版本信息*******************************/
unsigned char Ins_ID=1;                                                     //仪器号
unsigned char DSP_Ver = 0x11;                                                  //DSP程序版本号
unsigned char FPGA_Ver = 0x11;                                                 //FPGA程序版本号
/***********************************************************/
unsigned int watch_paramter=0;                                                  //局部变量观察数据入口
/******************工作状态********************************/
//Flag
volatile unsigned char Frame_Flag=0;                                        // 帧结束标志位
volatile unsigned char MicroFrame_Flag=0;                                   // 小帧结束标志位
//当前状态
volatile unsigned char state_main = 0;                                     // 当前主工作状态
volatile unsigned char state_low = 0;                                      // 当前低频工作状态
volatile unsigned char state_high = 0;                                     // 当前高频工作状态
volatile unsigned char SamplingFlag = 0;                                    //正在采集标记，正在采集=1，停止采集=0
volatile unsigned long long SamplePoint_Log = 0;                            //已经采集点数，上电时从FLASH内查询

volatile unsigned char state_set_temp = 0;  //暂时变量，用于兼容旧上位机使用。使用新上位机删除这个
//预设置参数
unsigned long MicroFrame_Interval=50000;                                     //小帧的间隔,50ms，拉大间隔，提高2路485冲撞几率
unsigned int  MicroFrame_Sum = 80;                                          //小帧的总数
unsigned char Frame_Interval = 5;                                            //帧间隔
unsigned char MicroFrame_Num = 5;                                           //小间隔电阻率采样点数
unsigned int Angle_Diff = 0;                                                //角差/2

//延时启动相关配置
unsigned char DelayTimeFinishFlag = 0;                                      //是否延时启动，默认不延时启动
unsigned int The_End_Time[7] = {0,0,0,0,0,0,0};                             //目标时钟数据，秒分时周日月年，十进制，上电后从FLash读入
/***********************************************************/

/**********************串口交互*******************************/
unsigned char baudflag = 0x01;                                              //当前波特率标志位
unsigned char setbaudflag = 0x00;                                            //更改波特率标志位

volatile unsigned char  Framedata_rxflag = 0;                               //帧结束标志
unsigned char RxBufCnt = 0;                                                 //当前接收字符数
unsigned char RxBuf[RxBufLength]={0};                                       //RS485接收Buff
unsigned int TxBufCnt = 0;                                                  //当前发送Buf字符数，仅代表数据
unsigned char TxBuf[TxBufLenfgth] = {0};                                    //RS485发送Buff
unsigned CMD_Error = 0;                                                     //如果此值为1，代表串口命令错误，不回复
/**********************************************************/



/**************************************************************/


/********************外部FLASH配置参数****************************/
//读写指针相关参数
unsigned int Write_BlockAddrCnt=0;                                              //外部FLASH写Block指针
unsigned char Write_PageAddrCnt=0;                                              //外部FLASH写Page指针
unsigned int Read_BlockAddrCnt=0;
unsigned char Read_PageAddrCnt=0;
unsigned long long ReadMemoryFrame=0;
unsigned long long ReadMemoryFrameEnd=0;
/*************************************************************/


/**********************与FPGA_SDRAM交互数据********************/
//float SampleData[288]={0};                                                    //从FPGA读取到的数据的累加，一共16个扇区，每个扇区9*2种数据
signed long long Res_sector[288]={0};                                           //从FPGA读取到的数据的累加，一共16个扇区，每个扇区9*2种数据
float AD4020_Scale_Coefficient = 2621440000;                                    //归一化系数，换算为真实幅度
Uint16 CMD_SetFreq[8] = {0x0011,0x0012,0x0014,0x0018,0x0021,0x0022,0x0024,0x0028};//控制FPGA数据采集，写入的数据命令,分别对应F1F5、 F2F5 、 F3F5 、 F4F5 、 F1F6 、 F2F6 、 F3F6 、 F4F6
Uint16 CMD_SetAmp[6]  = {8,4,4,4,4,4};            //控制FPGA输出幅度
/*************************************************************/


/**********************GAMMA计数*****************************/
volatile unsigned int GAMMA_Current_Count = 0;                                  //GAMMA实时计数值
/************************************************************/


/***********************存储帧结构**********************************/
unsigned int  Frame_Length[5] = {637,1048,1048,911,500};                        //存储五个频率下的帧长度
unsigned int  Frame_Length_Spfar = 637;//超远探固定帧数637
unsigned char PageBuffer[MAX_Buffer_LEN];                                       //存储一帧数据,有冗余

unsigned char DataFrameHeader[2]={0x00,0x16};                           //帧头和ID 在帧内占2字节
unsigned int pagecnt;                                                           //存储当前帧Buffer写指针 在帧内占2字节,实际写入为定值
unsigned char state_freq=0;                                                     //存放当前工作频率 在帧内占1字节
unsigned char ReadTime[7];                                                      //读出时钟数据，秒分时周日月年，十进制 在帧内占4字节
unsigned char Temperature;                                                      //存放当前温度 在帧内占1字节
unsigned char shockx;                                                           //x震动 在帧内占1字节
unsigned char shocky;                                                           //y震动 在帧内占1字节
unsigned char Rpm;                                                              //转速 在帧内占1字节
unsigned int Azi;                                                               //方位 在帧内占2字节
unsigned int lnc;                                                               //井斜 在帧内占2字节
unsigned int Pwd;                                                               //压力 在帧内占2字节
unsigned int MicroFrame_Cnt;                                                    //小帧的个数 在帧内占2字节
unsigned int GRprobe_total;                                                     //采样总伽马4s累计平均值 在帧内占2字节
unsigned int GRprobe_sector;                                                    //采样扇区伽马累计平均值 在帧内占2字节
/*************************************以下参数各个频率选择*********************************************/
unsigned char Auto_Zoom_R1A[3]={0x02,0x02,0x02};                   //R1A 4k、10k、40k档位0x01,0x02,0x01
unsigned char Auto_Zoom_R1B[3]={0x02,0x02,0x02};                   //R1B 4k、10k、40k档位0x03,0x02,0x01
unsigned char Auto_Zoom_R1C[3]={0x02,0x02,0x02};                   //R1C 4k、10k、40k档位0x03,0x02,0x01
unsigned char Auto_Zoom_R2A[3]={0x02,0x02,0x02};                   //R2A 10k、40k、100k档位0x01,0x01,0x01
unsigned char Auto_Zoom_R2B[3]={0x02,0x02,0x02};                   //R2B 10k、40k、100k档位0x01,0x01,0x01
unsigned char Auto_Zoom_R2C[3]={0x02,0x02,0x02};                   //R2C 10k、40k、100k档位0x01,0x01,0x01
unsigned char Auto_Zoom_R3U[2]={0x01,0x01};                            //R3U 100k、400k档位0x02,0x03
unsigned char Auto_Zoom_R3D[2]={0x01,0x01};                            //R3D 100k、400k档位0x02,0x03
unsigned char Zoom_R1A=1;                                                      //R1A增益
unsigned char Zoom_R1B=1;                                                      //R1B增益默认增益为3档位
unsigned char Zoom_R1C=1;                                                      //R1C增益默认增益为3档位
unsigned char Zoom_R2A=1;                                                      //R2A增益
unsigned char Zoom_R2B=1;                                                      //R2B增益
unsigned char Zoom_R2C=1;                                                      //R2C增益
unsigned char Zoom_R3U=1;                                                      //R3U增益
unsigned char Zoom_R3D=1;                                                      //R3D增益
unsigned char Zoom_TX=1;                                                       //R4A增益
float Average_R1A_Q;                                                           //R1A天线电阻率实部平均值
float Average_R1A_I;                                                           //R1A天线电阻率虚部平均值
float Average_R1B_Q;                                                           //R1B天线电阻率实部平均值
float Average_R1B_I;                                                           //R1B天线电阻率虚部平均值
float Average_R1C_Q;                                                           //R1C天线电阻率实部平均值
float Average_R1C_I;                                                           //R1C天线电阻率虚部平均值
float Average_R2A_Q;                                                           //R2A天线电阻率实部平均值
float Average_R2A_I;                                                           //R2A天线电阻率虚部平均值
float Average_R2B_Q;                                                           //R2B天线电阻率实部平均值
float Average_R2B_I;                                                           //R2B天线电阻率虚部平均值
float Average_R2C_Q;                                                           //R2C天线电阻率实部平均值
float Average_R2C_I;                                                           //R2C天线电阻率虚部平均值
float Average_R3U_Q;                                                           //R3U天线电阻率实部平均值
float Average_R3U_I;                                                           //R3U天线电阻率虚部平均值
float Average_R3D_Q;                                                           //R3D天线电阻率实部平均值
float Average_R3D_I;                                                           //R3D天线电阻率虚部平均值
float Average_TX_Q;                                                            //TX天线电阻率实部平均值
float Average_TX_I;                                                            //TX天线电阻率虚部平均值
float Max_avg_R1A_Q;                                                           //R1A天线电阻率实部平均值最大值
float Max_avg_R1A_I;                                                           //R1A天线电阻率虚部平均值最大值
float Max_avg_R2A_Q;                                                           //R2A天线电阻率实部平均值最大值
float Max_avg_R2A_I;                                                           //R2A天线电阻率虚部平均值最大值
/******************************************************************************************************/
unsigned int Angle_sector_res[16]={0};                                                  //每一个扇区内数据累加的次数 在帧内占2个字节
unsigned int Angle_sector_gama[16]={0};                                             //伽马扇区累计次数
unsigned int Angle_sector_gama_reg[16]={0};                                             //伽马扇区累计次数
unsigned int GR_sector[16]={0};                                                    //每一个扇区内伽马信号的个数 在帧内占2个字节
unsigned int GR_sector_reg[16]={0};                                                    //每一个扇区内伽马寄存
unsigned int GRprobe_total=0;                                                      //采样总伽马4s累计平均值 在帧内占2字节
unsigned int GRprobe_total_reg=0;
unsigned int GRprobe_sector=0;                                                      //采样扇区伽马累计平均值 在帧内占2字节
volatile Uint16 Cur_sector=0;                                                   //当前所在sector
/*************************************以下参数各个频率选择*********************************************/
float R1A_Q[16];                                                               //R1A天线电阻率实部，16个扇区
float R1A_I[16];                                                               //R1A天线电阻率虚部，16个扇区
float R1B_Q[16];                                                               //R1B天线电阻率实部，16个扇区
float R1B_I[16];                                                               //R1B天线电阻率虚部，16个扇区
float R1C_Q[16];                                                               //R1C天线电阻率实部，16个扇区
float R1C_I[16];                                                               //R1C天线电阻率虚部，16个扇区
float R2A_Q[16];                                                               //R2A天线电阻率实部，16个扇区
float R2A_I[16];                                                               //R2A天线电阻率虚部，16个扇区
float R2B_Q[16];                                                               //R2B天线电阻率实部，16个扇区
float R2B_I[16];                                                               //R2B天线电阻率虚部，16个扇区
float R2C_Q[16];                                                               //R2C天线电阻率实部，16个扇区
float R2C_I[16];                                                               //R2C天线电阻率虚部，16个扇区
float R3U_Q[16];                                                               //R3U天线电阻率实部，16个扇区
float R3U_I[16];                                                               //R3U天线电阻率虚部，16个扇区
float R3D_Q[16];                                                               //R3D天线电阻率实部，16个扇区
float R3D_I[16];                                                               //R3D天线电阻率虚部，16个扇区
float TX_Q[16];                                                               //TX天线电阻率实部，16个扇区
float TX_I[16];                                                               //TX天线电阻率虚部，16个扇区
/******************************************************************************************************/
unsigned int  FlashSendSum;                                                    //存放帧数据校验和 在帧内占2字节
/****************************************************************/
