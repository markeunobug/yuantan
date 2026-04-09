/*
 * Cmd_Logging.c
 *
 *  Created on: 2023年10月17日
 *      Author: xikai
 */
#include "Cmd_Logging.h"

unsigned long Start_Read_Frame=0;       //开始读的帧数
unsigned long Read_Frame=0;             //当前读的帧
unsigned long End_Read_Frame=0;         //结束读的帧数

void Set_TxBuf_SpecRes(unsigned char F)
{
    /********超远探版本********/
    TxBufCnt = 588;//超远探少2个字节
    TxBuf[0] = 0x05;
    TxBuf[1] = 0x00;
    TxBuf[2] = 0x01;
    TxBuf[3] = 0x00;
    TxBuf[4] = 0x01;
    TxBuf[5] = 0x00;
    TxBuf[6] = PageBuffer[19];//Cnth
    TxBuf[7] = PageBuffer[20];//Cntl

    TxBuf[8] = Zoom_R1A; //zoom_TX，借用于低频发射模式关键字
    TxBuf[9] = Zoom_R1B;//Zoom_R1A,借用于高频发射模式关键字
    TxBuf[10] = 0;//PageBuffer[25]，暂时不用
    TxBuf[11] = 0;//PageBuffer[26]，暂时不用

    int cnti  = 0,cntj = 0;
    /*   for(cnti=0;cnti<24;cnti++)
           TxBuf[12+cnti]=0x00;
    for(cnti=25;cnti<32;cnti++)
               TxBuf[12+cnti]=0x11;平均*/
   for(cnti=0;cnti<32;cnti++)
        TxBuf[12+cnti]=PageBuffer[27+cnti];
    for(cnti=0,cntj=0;cnti<34*16;cnti++,cntj++)
    {
        //跳过伽马信息
        if(cnti%34==2)
            cntj+=2;
        TxBuf[44+cnti] = PageBuffer[59+cntj];
    }
    /*************************/
 
    /********远探版本********/
    /*
    switch(F)
    {
        case 0x00:
        {
            switch(state_set_temp)//兼容远探上位机，暂时使用这个代替
            // switch(state_freq)
            {
                case 0:
                {
                    TxBufCnt = 588;
                    TxBuf[0] = 0x05;
                    TxBuf[1] = 0x00;
                    TxBuf[2] = 0x01;
                    TxBuf[3] = 0x00;
                    TxBuf[4] = 0x01;
                    TxBuf[5] = 0x00;
                    TxBuf[6] = PageBuffer[19];
                    TxBuf[7] = PageBuffer[20];

                    TxBuf[8] = PageBuffer[23];
                    TxBuf[9] = PageBuffer[24];
                    TxBuf[10] = PageBuffer[25];
                    TxBuf[11] = PageBuffer[26];

                    int cnti  = 0,cntj = 0;
                    for(cnti=0;cnti<32;cnti++)
                        TxBuf[12+cnti]=PageBuffer[27+cnti];  //平均
                    for(cnti=0,cntj=0;cnti<34*16;cnti++,cntj++)
                    {
                        //跳过伽马信息
                        if(cnti%34==2)
                            cntj+=2;
                        TxBuf[44+cnti] = PageBuffer[59+cntj];
                    }
                    break;
                }
                case 1:
                {
                   TxBufCnt = 999;
                   TxBuf[0] = 0x05;
                   TxBuf[1] = 0x00;
                   TxBuf[2] = 0x01;
                   TxBuf[3] = 0x00;
                   TxBuf[4] = 0x02;
                   TxBuf[5] = 0x00;
                   TxBuf[6] = PageBuffer[19];
                   TxBuf[7] = PageBuffer[20];
                   TxBuf[8] = PageBuffer[23];
                   TxBuf[9] = PageBuffer[24];
                   TxBuf[10] = PageBuffer[25];
                   TxBuf[11] = PageBuffer[26];
                   TxBuf[12] = PageBuffer[27];
                   TxBuf[13] = PageBuffer[28];
                   TxBuf[14] = PageBuffer[29];

                   int cnti = 0,cntj = 0;
                   for(cnti=0;cnti<56;cnti++)
                       TxBuf[15+cnti]=PageBuffer[30+cnti];  //增益和平均
                   for(cnti=0,cntj=0;cnti<58*16;cnti++,cntj++)
                   {
                       //跳过伽马信息
                       if(cnti%58==2)
                           cntj+=2;
                       TxBuf[71+cnti] = PageBuffer[86+cntj];
                   }
                  break;
               }
               case 2:
               {
                    TxBufCnt = 999;
                    TxBuf[0] = 0x05;
                    TxBuf[1] = 0x00;
                    TxBuf[2] = 0x01;
                    TxBuf[3] = 0x00;
                    TxBuf[4] = 0x03;
                    TxBuf[5] = 0x00;
                    TxBuf[6] = PageBuffer[19];
                    TxBuf[7] = PageBuffer[20];
                    TxBuf[8] = PageBuffer[23];
                    TxBuf[9] = PageBuffer[24];
                    TxBuf[10] = PageBuffer[25];
                    TxBuf[11] = PageBuffer[26];
                    TxBuf[12] = PageBuffer[27];
                    TxBuf[13] = PageBuffer[28];
                    TxBuf[14] = PageBuffer[29];

                    int cnti = 0,cntj = 0;
                    for(cnti=0;cnti<56;cnti++)
                        TxBuf[15+cnti]=PageBuffer[30+cnti];  //增益和平均
                    for(cnti=0,cntj=0;cnti<58*16;cnti++,cntj++)
                    {
                        //跳过伽马信息
                        if(cnti%58==2)
                            cntj+=2;
                        TxBuf[71+cnti] = PageBuffer[86+cntj];
                    }
                 break;
               }
               case 3:
               {

                  TxBufCnt = 862;
                  TxBuf[0] = 0x05;
                  TxBuf[1] = 0x00;
                  TxBuf[2] = 0x01;
                  TxBuf[3] = 0x00;
                  TxBuf[4] = 0x04;
                  TxBuf[5] = 0x00;
                  TxBuf[6] = PageBuffer[19];
                  TxBuf[7] = PageBuffer[20];
                  TxBuf[8] = PageBuffer[23];
                  TxBuf[9] = PageBuffer[24];
                  TxBuf[10] = PageBuffer[25];
                  TxBuf[11] = PageBuffer[26];
                  TxBuf[12] = PageBuffer[27];
                  TxBuf[13] = PageBuffer[28];

                  int cnti = 0,cntj = 0;
                  for(cnti=0;cnti<48;cnti++)
                      TxBuf[14+cnti]=PageBuffer[29+cnti];  //增益和平均
                  for(cnti=0,cntj=0;cnti<50*16;cnti++,cntj++)
                  {
                      //跳过伽马信息
                      if(cnti%50==2)
                          cntj+=2;
                      TxBuf[62+cnti] = PageBuffer[77+cntj];
                  }
                    break;
                }
                case 4://故意这么使用，400k与2M ，后续更改
                {}
                case 5:
                {
                TxBufCnt = 451;
                TxBuf[0] = 0x05;
                TxBuf[1] = 0x00;
                TxBuf[2] = 0x01;
                TxBuf[3] = 0x00;
                TxBuf[4] = 0x05;
                TxBuf[5] = 0x00;
                TxBuf[6] = PageBuffer[19];
                TxBuf[7] = PageBuffer[20];
                TxBuf[8] = PageBuffer[23];
                TxBuf[9] = PageBuffer[24];
                TxBuf[10] = PageBuffer[25];
                int cnti = 0,cntj = 0;
                for(cnti=0;cnti<24;cnti++)
                    TxBuf[11+cnti]=PageBuffer[26+cnti];  //增益和平均
                for(cnti=0,cntj=0;cnti<26*16;cnti++,cntj++)
                {
                    //跳过伽马信息
                    if(cnti%26==2)
                        cntj+=2;
                    TxBuf[35+cnti] = PageBuffer[50+cntj];
                }
                break;
                }
                default:
                {
                  //CMD_Error = 1;
                break;
                }
            }
            break;
        }
        //F1电阻率信息
        case 0x01:
        {
            if(state_main==1)
            {
                TxBufCnt = 588;
                TxBuf[0] = 0x05;
                TxBuf[1] = 0x00;
                TxBuf[2] = 0x01;
                TxBuf[3] = 0x00;
                TxBuf[4] = 0x01;
                TxBuf[5] = 0x00;
                TxBuf[6] = PageBuffer[19];
                TxBuf[7] = PageBuffer[20];

                TxBuf[8] = PageBuffer[23];
                TxBuf[9] = PageBuffer[24];
                TxBuf[10] = PageBuffer[25];
                TxBuf[11] = PageBuffer[26];

                int cnti  = 0,cntj = 0;
                for(cnti=0;cnti<32;cnti++)
                    TxBuf[12+cnti]=PageBuffer[27+cnti];  //平均
                for(cnti=0,cntj=0;cnti<34*16;cnti++,cntj++)
                {
                    //跳过伽马信息
                    if(cnti%34==2)
                        cntj+=2;
                    TxBuf[44+cnti] = PageBuffer[59+cntj];
                }
            }
            else
            {
                CMD_Error = 1;
            }

            break;
        }
        //F2电阻率信息
        case 0x02:
        {
            if(state_main==2)
            {
                TxBufCnt = 999;
                TxBuf[0] = 0x05;
                TxBuf[1] = 0x00;
                TxBuf[2] = 0x01;
                TxBuf[3] = 0x00;
                TxBuf[4] = 0x02;
                TxBuf[5] = 0x00;
                TxBuf[6] = PageBuffer[19];
                TxBuf[7] = PageBuffer[20];
                TxBuf[8] = PageBuffer[23];
                TxBuf[9] = PageBuffer[24];
                TxBuf[10] = PageBuffer[25];
                TxBuf[11] = PageBuffer[26];
                TxBuf[12] = PageBuffer[27];
                TxBuf[13] = PageBuffer[28];
                TxBuf[14] = PageBuffer[29];

                int cnti = 0,cntj = 0;
                for(cnti=0;cnti<56;cnti++)
                    TxBuf[15+cnti]=PageBuffer[30+cnti];  //增益和平均
                for(cnti=0,cntj=0;cnti<58*16;cnti++,cntj++)
                {
                    //跳过伽马信息
                    if(cnti%58==2)
                        cntj+=2;
                    TxBuf[71+cnti] = PageBuffer[86+cntj];
                }
            }
            else
            {
                CMD_Error = 1;
            }
            break;
        }
        //F3电阻率信息
        case 0x03:
        {
            if(state_main==3)
            {
                TxBufCnt = 999;
                TxBuf[0] = 0x05;
                TxBuf[1] = 0x00;
                TxBuf[2] = 0x01;
                TxBuf[3] = 0x00;
                TxBuf[4] = 0x03;
                TxBuf[5] = 0x00;
                TxBuf[6] = PageBuffer[19];
                TxBuf[7] = PageBuffer[20];
                TxBuf[8] = PageBuffer[23];
                TxBuf[9] = PageBuffer[24];
                TxBuf[10] = PageBuffer[25];
                TxBuf[11] = PageBuffer[26];
                TxBuf[12] = PageBuffer[27];
                TxBuf[13] = PageBuffer[28];
                TxBuf[14] = PageBuffer[29];

                int cnti = 0,cntj = 0;
                for(cnti=0;cnti<56;cnti++)
                    TxBuf[15+cnti]=PageBuffer[30+cnti];  //增益和平均
                for(cnti=0,cntj=0;cnti<58*16;cnti++,cntj++)
                {
                    //跳过伽马信息
                    if(cnti%58==2)
                        cntj+=2;
                    TxBuf[71+cnti] = PageBuffer[86+cntj];
                }
            }
            else
            {
                CMD_Error = 1;
            }
            break;
        }
        //F4电阻率信息
        case 0x04:
        {
            if(state_main==4)
            {
                TxBufCnt = 862;
                TxBuf[0] = 0x05;
                TxBuf[1] = 0x00;
                TxBuf[2] = 0x01;
                TxBuf[3] = 0x00;
                TxBuf[4] = 0x04;
                TxBuf[5] = 0x00;
                TxBuf[6] = PageBuffer[19];
                TxBuf[7] = PageBuffer[20];
                TxBuf[8] = PageBuffer[23];
                TxBuf[9] = PageBuffer[24];
                TxBuf[10] = PageBuffer[25];
                TxBuf[11] = PageBuffer[26];
                TxBuf[12] = PageBuffer[27];
                TxBuf[13] = PageBuffer[28];

                int cnti = 0,cntj = 0;
                for(cnti=0;cnti<48;cnti++)
                    TxBuf[14+cnti]=PageBuffer[29+cnti];  //增益和平均
                for(cnti=0,cntj=0;cnti<50*16;cnti++,cntj++)
                {
                    //跳过伽马信息
                    if(cnti%50==2)
                        cntj+=2;
                    TxBuf[62+cnti] = PageBuffer[77+cntj];
                }
            }
            else
            {
                CMD_Error = 1;
            }
            break;
        }
        //F5电阻率信息
        case 0x05:
        {
            if(state_main==5)
            {
                TxBufCnt = 451;
                TxBuf[0] = 0x05;
                TxBuf[1] = 0x00;
                TxBuf[2] = 0x01;
                TxBuf[3] = 0x00;
                TxBuf[4] = 0x05;
                TxBuf[5] = 0x00;
                TxBuf[6] = PageBuffer[19];
                TxBuf[7] = PageBuffer[20];
                TxBuf[8] = PageBuffer[23];
                TxBuf[9] = PageBuffer[24];
                TxBuf[10] = PageBuffer[25];


                int cnti = 0,cntj = 0;
                for(cnti=0;cnti<24;cnti++)
                    TxBuf[11+cnti]=PageBuffer[26+cnti];  //增益和平均
                for(cnti=0,cntj=0;cnti<26*16;cnti++,cntj++)
                {
                    //跳过伽马信息
                    if(cnti%26==2)
                        cntj+=2;
                    TxBuf[35+cnti] = PageBuffer[50+cntj];
                }
            }
            else
            {
                CMD_Error = 1;
            }
            break;
        }
        default:
        {
            break;
        }
    }
    */
}

void Set_TxBuf_GRprobe()
{
    //相同的回应信息
    TxBufCnt = 72;
    TxBuf[0] = 0x05;
    TxBuf[1] = 0x00;
    TxBuf[2] = 0x02;
    TxBuf[3] = 0x00;
    TxBuf[4] = (MicroFrame_Cnt>>8)&0xFF;
    TxBuf[5] = (MicroFrame_Cnt)&0xFF;
    TxBuf[6] = (GRprobe_total>>8)&0xFF;
    TxBuf[7] = (GRprobe_total)&0xFF;
    int cnti=0;
    for(cnti=0;cnti<16;cnti++)
    {
        TxBuf[8+4*cnti] = (Angle_sector_gama_reg[cnti]>>8)&0xFF;
        TxBuf[9+4*cnti] = (Angle_sector_gama_reg[cnti])&0xFF;
        TxBuf[10+4*cnti] = (GR_sector_reg[cnti]>>8)&0xFF;
        TxBuf[11+4*cnti] = (GR_sector_reg[cnti])&0xFF;
    }
}

void Set_TxBuf_Assist_Mes(unsigned char ID)
{
    switch(ID)
    {
        case 0x00:
        {
            TxBufCnt = 8;
            TxBuf[0] = 0x05;
            TxBuf[1] = 0x00;
            TxBuf[2] = 0x03;
            TxBuf[3] = 0x00;
            TxBuf[4] = 0x00;
            TxBuf[5] = 0x00;
            TxBuf[6] = (Azi >> 8)&0xFF;
            TxBuf[7] = (Azi) & 0xFF;
            break;
        }
        case 0x01:
        {
            TxBufCnt = 10;
            TxBuf[0] = 0x05;
            TxBuf[1] = 0x00;
            TxBuf[2] = 0x03;
            TxBuf[3] = 0x00;
            TxBuf[4] = 0x01;
            TxBuf[5] = 0x00;
            TxBuf[6] = (lnc >> 8)&0xFF;
            TxBuf[7] = (lnc) & 0xFF;
            TxBuf[8] = (Azi >> 8)&0xFF;
            TxBuf[9] = (Azi) & 0xFF;
            break;
        }
        case 0x02:
        {
            TxBufCnt = 10;
            TxBuf[0] = 0x05;
            TxBuf[1] = 0x00;
            TxBuf[2] = 0x03;
            TxBuf[3] = 0x00;
            TxBuf[4] = 0x02;
            TxBuf[5] = 0x00;
            TxBuf[6] = shockx;
            TxBuf[7] = shocky;
            TxBuf[8] = Temperature;
            TxBuf[9] = Rpm;
            break;
        }
        case 0x03:
        {
            TxBufCnt = 8;
            TxBuf[0] = 0x05;
            TxBuf[1] = 0x00;
            TxBuf[2] = 0x03;
            TxBuf[3] = 0x00;
            TxBuf[4] = 0x03;
            TxBuf[5] = 0x00;
            TxBuf[6] = (Pwd >> 8)&0xFF;
            TxBuf[7] = (Pwd) & 0xFF;
            break;
        }
        default:
        {
            break;
        }
    }
}

void Set_Mem_Manage(unsigned char ID)
{
    switch(ID)
    {
        //帧数查询
        case 0x00:
        {
            TxBufCnt = 10;
            TxBuf[0] = 0x05;
            TxBuf[1] = 0x00;
            TxBuf[2] = 0x04;
            TxBuf[3] = 0x00;
            TxBuf[4] = 0x00;
            TxBuf[5] = 0x00;
            Flash_Read_SamplePoint();
            TxBuf[6] = (SamplePoint_Log >> 24)&0xFF;
            TxBuf[7] = (SamplePoint_Log >> 18)&0xFF;
            TxBuf[8] = (SamplePoint_Log >> 8)&0xFF;
            TxBuf[9] = (SamplePoint_Log)&0xFF;

            break;
        }
        //设置读取帧
        case 0x01:
        {

            Start_Read_Frame = RxBuf[8];
            Start_Read_Frame = (Start_Read_Frame<<8) + RxBuf[9];
            Start_Read_Frame = (Start_Read_Frame<<8) + RxBuf[10];
            Start_Read_Frame = (Start_Read_Frame<<8) + RxBuf[11];
            End_Read_Frame = RxBuf[12];
            End_Read_Frame = (End_Read_Frame<<8) + RxBuf[13];
            End_Read_Frame = (End_Read_Frame<<8) + RxBuf[14];
            End_Read_Frame = (End_Read_Frame<<8) + RxBuf[15];
            Read_Frame = Start_Read_Frame;
            if(End_Read_Frame>SamplePoint_Log)
            {
                CMD_Error = 1;
                End_Read_Frame = SamplePoint_Log;
            }
            TxBufCnt = 6;
            TxBuf[0] = 0x05;
            TxBuf[1] = 0x00;
            TxBuf[2] = 0x04;
            TxBuf[3] = 0x00;
            TxBuf[4] = 0x01;
            TxBuf[5] = 0x00;

            break;
        }
        //读取帧
        case 0x02:
        {
            Read_BlockAddrCnt = Read_Frame/128;
            Read_PageAddrCnt = Read_Frame%128;
            Read_Frame++;
            if(Read_Frame<=End_Read_Frame)
            {
                FlashReadOneFrame();                                //结果存储在PageBuffer
                // TxBufCnt = Frame_Length[PageBuffer[4] - 1] + 6 - 6; //加上6个应答标志，剔除6个帧头ID校验和
                TxBufCnt = Frame_Length_Spfar;
                TxBuf[0] = 0x05;
                TxBuf[1] = 0x00;
                TxBuf[2] = 0x04;
                TxBuf[3] = 0x00;
                TxBuf[4] = 0x02;
                TxBuf[5] = 0x00;
                MemCopy((Uint16*)(PageBuffer+4), (Uint16*)(PageBuffer + TxBufCnt), (Uint16*)(TxBuf + 6) );
            }
            else
            {
                CMD_Error = 1;
            }
            break;
        }
        case 0x03:
        {
            Flash_EraseAll();
            SamplePoint_Log = 0;        //擦除后没有数据
            TxBufCnt = 6;
            TxBuf[0] = 0x05;
            TxBuf[1] = 0x00;
            TxBuf[2] = 0x04;
            TxBuf[3] = 0x00;
            TxBuf[4] = 0x03;
            TxBuf[5] = 0x00;
            break;
        }
    }
}

void  Decode_Cmd_Logging()
{
    //检索命令第二字节
    switch(RxBuf[5])
    {
        //要电阻率参数
        case 0x01:
        {
            //根据频率发送电阻率信息
            Set_TxBuf_SpecRes(RxBuf[7]);
            break;
        }
        //要伽马参数
        case 0x02:
        {
            Set_TxBuf_GRprobe();
            break;
        }
        //要辅助参数
        case 0x03:
        {
            Set_TxBuf_Assist_Mes(RxBuf[7]);
            break;
        }
        //存储管理
        case 0x04:
        {
            Set_Mem_Manage(RxBuf[7]);
            break;
        }
        //NWD要数
        case 0x05:
        {

            break;
        }
        default:
        {
            return;
        }
    }
}

