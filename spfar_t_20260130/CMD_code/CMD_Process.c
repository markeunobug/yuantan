/*
 * Process_Data.c
 *
 *  Created on: 2023年6月11日
 *      Author: xikai
 */

#include "CMD_Process.h"



//计算校验和，正确输出1
static unsigned char Verify_CMD(void)
{
    if( RxBuf[0]==0x16 && RxBuf[1]==0x00 )
    {
        //命令长度
        unsigned int CMD_Length  = RxBuf[2];
        CMD_Length = CMD_Length << 8;       //高字节
        CMD_Length += RxBuf[3];             //低字节
        //校验和
        unsigned int CheckSum = RxBuf[CMD_Length - 2];
        CheckSum = CheckSum << 8;
        CheckSum += RxBuf[CMD_Length - 1];
        //计算校验和
        unsigned int cnti = 0,temp = 0;
        for(cnti = 0;cnti < CMD_Length-2;cnti++)
        {
            temp += RxBuf[cnti];
        }
        if(temp==CheckSum)
            return 1;
        else
            return 0;
    }
    else
    {
        return 0;
    }
}



void Decode_Cmd(void)
{
    TxBufCnt = 0;
    //前两个字节为虚拟ID，接下来两个字节为命令长度
    if(Verify_CMD()==1)
    {
        //第五个字节为命令首字节
        switch(RxBuf[4])
        {
            case 0x01:
            {
                Decode_Cmd_Set();
                break;
            }
            case 0x02:
            {
                Decode_Cmd_Inquire();
                break;
            }
            case 0x03:
            {
                Decode_Cmd_Scale();
                break;
            }
            case 0x04:
            {
                Decode_Cmd_SelfTest();
                break;
            }
            case 0x05:
            {
                Decode_Cmd_Logging();
                break;
            }
            default:
            {
                return ;
            }
        }
        if(CMD_Error==1)
        {
            CMD_Error = 0;
            TxBufCnt = 2;
            TxBuf[0] = 0xFA;
            TxBuf[1] = 0x00;
        }

        RS485_Send_BackCmd();

        if(setbaudflag!=0)
        {
            setbaudflag=0;
            RS485_SetBsp();             //回复之后才可以更改波特率

        }

        RS485B_RXMODE();             //进入接收模式
    }
    else
    {
        return ;
    }
}


