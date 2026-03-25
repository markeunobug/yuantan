/*
 * Cmd_Set.c
 *
 *  Created on: 2023年10月17日
 *      Author: xikai
 */

#include "Cmd_Set.h"


void Decode_Cmd_Set()
{
    //检索命令第二字节
    switch(RxBuf[5])
    {
        //停止设置
        case 0x00:
        {
            //FPGA停止发送
            SamplingFlag = 0;       //停止
            FPGA_Stop();
            TxBuf[2] = 0x00;
            break;
        }
        //启动设置
        case 0x01:
        {
            SamplingFlag = 1;       //采集标志置1

            TxBuf[2] = 0x01;
            break;
        }
        //状态设置
        case 0x02:
        {
            switch(RxBuf[7])
            {
                case 0x00:
                {
                    state_main = 0;//MF
                    break;
                }
                case 0x01:
                {
                    state_main = 1;//1K
                    break;
                }
                case 0x02:
                {
                    state_main = 2;//4K
                    break;
                }
                case 0x03:
                {
                    state_main = 3;//16K
                    break;
                }
                case 0x04:
                {
                    state_main = 4;//64K
                    break;
                }
                case 0x05:
                {
                    state_main = 5;//400K
                    break;
                }
                case 0x06:
                {
                    state_main = 6;//2M
                    break;
                }
                default:
                {
                    break;
                }
            }
            TxBuf[2] = 0x02;
            break;
        }
        //时钟设置
        case 0x03:
        {
            //设置当前时间
            unsigned long Timedata=RxBuf[6];
            Timedata=Timedata<<8;
            Timedata=Timedata+RxBuf[7];
            Timedata=Timedata<<8;
            Timedata=Timedata+RxBuf[8];
            Timedata=Timedata<<8;
            Timedata=Timedata+RxBuf[9];
            unsigned char WriteTime[7];
            WriteTime[0] =  Timedata&0x0000003f;            //秒：第0-5位。
            WriteTime[1] = (Timedata&0x00000fc0)>>6;        //分：第6-11位；
            WriteTime[2] = (Timedata&0x0001f000)>>12;       //时：第12-16位；
            WriteTime[4] = (Timedata&0x003e0000)>>17;       //日：第17-21位；
            WriteTime[5] = (Timedata&0x03c00000)>>22;       //月：第22-25位
            WriteTime[6] = (Timedata&0xfc000000)>>26;       //年-2000：第26-31位；
            DS1629_SetTimeAll(WriteTime);

            TxBuf[2] = 0x03;
            break;
        }
        //启动工作时间设置
        case 0x04:
        {
            //设置启动时间
            unsigned long Timedata=RxBuf[6];
            Timedata=Timedata<<8;
            Timedata=Timedata+RxBuf[7];
            Timedata=Timedata<<8;
            Timedata=Timedata+RxBuf[8];
            Timedata=Timedata<<8;
            Timedata=Timedata+RxBuf[9];

            The_End_Time[0] =  Timedata&0x0000003f;            //秒：第0-5位。
            The_End_Time[1] = (Timedata&0x00000fc0)>>6;        //分：第6-11位；
            The_End_Time[2] = (Timedata&0x0001f000)>>12;       //时：第12-16位；
            The_End_Time[4] = (Timedata&0x003e0000)>>17;       //日：第17-21位；
            The_End_Time[5] = (Timedata&0x03c00000)>>22;       //月：第22-25位
            The_End_Time[6] = (Timedata&0xfc000000)>>26;       //年-2000：第26-31位；

            TxBuf[2] = 0x04;
            break;
        }
        //采样间隔设置
        case 0x05:
        {
            if(SamplingFlag==0)                                    //仅在停止状态设置有效
            {
                Frame_Interval = RxBuf[7];
            }

            TxBuf[2] = 0x05;
            break;
        }
        //角差设置
        case 0x06:
        {
            // Angle_Diff = RxBuf[6];
            // Angle_Diff = (Angle_Diff << 8) + RxBuf[7];

            //暂时借用设置2M
            state_main = 6;//2M

            TxBuf[2] = 0x06;
            break;
        }
        //波特率设置
        case 0x07:
        {
            if(SamplingFlag==0)                                    //仅在停止状态设置有效
            {
                baudflag = RxBuf[7];
                setbaudflag = 0x01;
            }
            else
            {
                CMD_Error = 1;
            }

            TxBuf[2] = 0x07;
            break;
        }
        //幅度控制字设置
        case 0x08:
        {
            if(SamplingFlag==0)                                    //仅在停止状态设置有效
            {
                CMD_SetAmp[0] = RxBuf[6];
                CMD_SetAmp[0] = (CMD_SetAmp[0] << 8) + RxBuf[7];
                CMD_SetAmp[1] = RxBuf[8];
                CMD_SetAmp[1] = (CMD_SetAmp[1] << 8) + RxBuf[9];
                CMD_SetAmp[2] = RxBuf[10];
                CMD_SetAmp[2] = (CMD_SetAmp[2] << 8) + RxBuf[11];
                CMD_SetAmp[3] = RxBuf[12];
                CMD_SetAmp[3] = (CMD_SetAmp[3] << 8) + RxBuf[13];
                //AMPH_F5 AMPL_F5被400K和2M复用，即同时配置。
                CMD_SetAmp[4] = RxBuf[14];
                CMD_SetAmp[4] = (CMD_SetAmp[4] << 8) + RxBuf[15];
                CMD_SetAmp[5] = CMD_SetAmp[4];
            }
            else
            {
                CMD_Error = 1;
            }

            TxBuf[2]=0x08;
            break;
        }
        default:
        {
            return;
        }
    }

    //幅值回传数据
    TxBufCnt = 4;
    TxBuf[0] = 0x01;
    TxBuf[1] = 0x00;
    TxBuf[3] = 0x00;

    FLASH_SaveSettings();                                   //保存设置
}

