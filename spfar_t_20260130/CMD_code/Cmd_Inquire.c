/*
 * Cmd_Inquire.c
 *
 *  Created on: 2023年10月17日
 *      Author: xikai
 */

#include "Cmd_Inquire.h"


void Decode_Cmd_Inquire()
{
    //检索命令第二字节
    switch(RxBuf[5])
    {
        //仪器状态查询
        case 0x00:
        {
            Flash_Read_SamplePoint();
            TxBufCnt = 4;
            TxBuf[0] = 0x02;
            TxBuf[1] = 0x00;
            TxBuf[2] = SamplingFlag;
            TxBuf[3] = 0x00;
            break;
        }
        //状态机查询
        case 0x02:
        {
            TxBufCnt = 4;
            TxBuf[0] = 0x02;
            TxBuf[1] = 0x00;
            TxBuf[2] = 0x02;
            TxBuf[3] = state_main;
            break;
        }
        //时钟查询
        case 0x03:
        {
            TxBufCnt = 8;
            TxBuf[0] = 0x02;
            TxBuf[1] = 0x00;
            TxBuf[2] = 0x03;
            TxBuf[3] = 0x00;

            DS1629_ReadTimeAll();           //读出当前时间
            unsigned long Timedata=ReadTime[6];
            Timedata=Timedata<<4;
            Timedata=Timedata+ReadTime[5];
            Timedata=Timedata<<5;
            Timedata=Timedata+ReadTime[4];
            Timedata=Timedata<<5;
            Timedata=Timedata+ReadTime[2];
            Timedata=Timedata<<6;
            Timedata=Timedata+ReadTime[1];
            Timedata=Timedata<<6;
            Timedata=Timedata+ReadTime[0];

            TxBuf[4] = (Timedata>>24)&0xFF;
            TxBuf[5] = (Timedata>>16)&0xFF;
            TxBuf[6] = (Timedata>>8)&0xFF;
            TxBuf[7] = Timedata&0xFF;

            break;
        }
        //启动工作时间查询
        case 0x04:
        {
            TxBufCnt = 8;
            TxBuf[0] = 0x02;
            TxBuf[1] = 0x00;
            TxBuf[2] = 0x04;
            TxBuf[3] = 0x00;

            unsigned long Timedata=The_End_Time[6];
            Timedata=Timedata<<4;
            Timedata=Timedata+The_End_Time[5];
            Timedata=Timedata<<5;
            Timedata=Timedata+The_End_Time[4];
            Timedata=Timedata<<5;
            Timedata=Timedata+The_End_Time[2];
            Timedata=Timedata<<6;
            Timedata=Timedata+The_End_Time[1];
            Timedata=Timedata<<6;
            Timedata=Timedata+The_End_Time[0];

            TxBuf[4] = (Timedata>>24)&0xFF;
            TxBuf[5] = (Timedata>>16)&0xFF;
            TxBuf[6] = (Timedata>>8)&0xFF;
            TxBuf[7] = Timedata&0xFF;
            break;
        }
        //采样间隔查询
        case 0x05:
        {
            TxBufCnt = 6;
            TxBuf[0] = 0x02;
            TxBuf[1] = 0x00;
            TxBuf[2] = 0x05;
            TxBuf[3] = 0x00;
            TxBuf[4] = 0x00;
            TxBuf[5] = Frame_Interval;
            break;
        }
        //角差查询
        case 0x06:
        {
            TxBufCnt = 6;
            TxBuf[0] = 0x02;
            TxBuf[1] = 0x00;
            TxBuf[2] = 0x06;
            TxBuf[3] = 0x00;
            TxBuf[4] = (Angle_Diff>>8)&0xFF;
            TxBuf[5] = Angle_Diff&0xFF;
            break;
        }
        //发射幅度控制字查询
        case 0x08:
        {
            TxBufCnt = 14;
            TxBuf[0] = 0x02;
            TxBuf[1] = 0x00;
            TxBuf[2] = 0x08;
            TxBuf[3] = 0x00;
            TxBuf[4] = (CMD_SetAmp[0] >> 8)&0xFF;
            TxBuf[5] = CMD_SetAmp[0]&0xFF;
            TxBuf[6] = (CMD_SetAmp[1] >> 8)&0xFF;
            TxBuf[7] = CMD_SetAmp[1]&0xFF;
            TxBuf[8] = (CMD_SetAmp[2] >> 8)&0xFF;
            TxBuf[9] = CMD_SetAmp[2]&0xFF;
            TxBuf[10] = (CMD_SetAmp[3] >> 8)&0xFF;
            TxBuf[11] = CMD_SetAmp[3]&0xFF;
            TxBuf[12] = (CMD_SetAmp[4] >> 8)&0xFF;
            TxBuf[13] = CMD_SetAmp[4]&0xFF;
            break;
        }
        //仪器号查询
        case 0x09:
        {
            TxBufCnt = 6;
            TxBuf[0] = 0x02;
            TxBuf[1] = 0x00;
            TxBuf[2] = 0x09;
            TxBuf[3] = 0x00;
            TxBuf[4] = 0x00;
            TxBuf[5] = Ins_ID;
            break;
        }
        //幅度控制字设置
        case 0x0A:
        {
            TxBufCnt = 6;
            TxBuf[0] = 0x02;
            TxBuf[1] = 0x00;
            TxBuf[2] = 0x0A;
            TxBuf[3] = 0x00;
            TxBuf[4] = DSP_Ver;
            TxBuf[5] = FPGA_Ver;
            break;
        }
        default:
        {
            return;
        }
    }
}

