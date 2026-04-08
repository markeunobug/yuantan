/*
 * FLASH_Buffer.c
 *
 *  Created on: 2023年7月2日
 *      Author: xikai
 */

#include <Frame_Buffer_Process.h>

//存放一个unsigned char进入帧数据Buffer
void Send_Frame_UnsignedChar(unsigned char *Ulval,unsigned int Num)
{
    int cnti = 0;
    for(cnti = 0;cnti < Num;cnti++)
    {
        //用于计算校验位
        FlashSendSum += Ulval[cnti];
        //将数据存放至帧Buffer
        PageBuffer[pagecnt] = Ulval[cnti];
        pagecnt++;
        if(pagecnt==Frame_Length_Spfar)
        // if(pagecnt==Frame_Length[state_freq])
        {
            FLASH_SaveOneFrame();
        }
    }
}

static void Send_Frame_One_UnsignedChar(unsigned char Ulval)
{
    //用于计算校验位
    FlashSendSum += Ulval&0xFF;
    //将数据存放至帧Buffer
    PageBuffer[pagecnt] = Ulval&0xFF;
    pagecnt++;
    if(pagecnt==Frame_Length_Spfar)
    // if(pagecnt==Frame_Length[state_freq])
    {
        FLASH_SaveOneFrame();
    }
}


//存放unsigned long类型数据
void Send_Frame_UnsignedLong(unsigned long *Senddata,unsigned int Num)
{
    int cnti = 0;
    for(cnti=0;cnti< Num;cnti++)
    {
        Send_Frame_One_UnsignedChar(((Senddata[cnti]>>24)&0xff));
        Send_Frame_One_UnsignedChar(((Senddata[cnti]>>16)&0xff));
        Send_Frame_One_UnsignedChar(((Senddata[cnti]>>8)&0xff));
        Send_Frame_One_UnsignedChar(((Senddata[cnti])&0xff));
    }
}

//存放一个unsigned int类型数据
void Send_Frame_UnsignedInt(unsigned int *Senddata,unsigned int Num)
{
    int cnti = 0;
    for(cnti=0;cnti< Num;cnti++)
    {
        Send_Frame_One_UnsignedChar(((Senddata[cnti]>>8)&0xff));
        Send_Frame_One_UnsignedChar(((Senddata[cnti])&0xff));
    }
}

//存放float类型  数据，每个数据4个字节，乘以10^5之后再写入flash中
void Send_Frame_Float(float *Senddata,unsigned int Num)
{
    unsigned int cnti=0;
    long data=0;
    for(cnti=0;cnti<Num;cnti++)
    {
        data=Senddata[cnti]*100000.0;
        if(data < 0)                //负数不能直接发送补码，先转化为正数，再补符号位
        {
            data=-data;
            data=data| 0x80000000;
        }
        Send_Frame_UnsignedLong((unsigned long*)&data,1);
    }
}

//将时间转换为unsigned long
void Send_Frame_Time(unsigned char *Time)        //秒分时日周月年
{
     unsigned long Timedata;
     Timedata=Time[6];
     Timedata=Timedata<<4;         //月年占4位
     Timedata=Timedata+Time[5];
     Timedata=Timedata<<5;         //日月占5位
     Timedata=Timedata+Time[4];
     Timedata=Timedata<<5;         //时占5位
     Timedata=Timedata+Time[2];
     Timedata=Timedata<<6;         //分占6位
     Timedata=Timedata+Time[1];
     Timedata=Timedata<<6;         //秒占6位
     Timedata=Timedata+Time[0];
     Send_Frame_UnsignedLong(&Timedata,1);
}

//设置帧头
void Send_Frame_Start(void)
{
    //写入帧头和ID
    Send_Frame_UnsignedChar(DataFrameHeader, 2);
    //写入当前帧长度
    Send_Frame_UnsignedInt(&Frame_Length_Spfar, 1);
    // Send_Frame_UnsignedInt(Frame_Length+state_freq, 1);
    //写入频率字
    Send_Frame_One_UnsignedChar(state_freq+1);
    //写入时间
    Send_Frame_Time(ReadTime);
    //写入温度
    Send_Frame_One_UnsignedChar(Temperature);
    //写入x y震动
    Send_Frame_One_UnsignedChar(shockx);
    Send_Frame_One_UnsignedChar(shocky);
    //写入转速
    Send_Frame_One_UnsignedChar(Rpm);
    //写入方位
    Send_Frame_UnsignedInt(&Azi, 1);
    //写入井斜
    Send_Frame_UnsignedInt(&lnc, 1);
    //写入压力
    Send_Frame_UnsignedInt(&Pwd, 1);
}


//发送帧尾
void Send_Frame_End(void)
{
    //发送校验和
    Send_Frame_UnsignedInt(&FlashSendSum, 1);
}


void Send_Frame_WithFreq()
{
    //超远探版本，不需要区分工作模式
    //写入增益。使用原来声明字代替
    Send_Frame_One_UnsignedChar(Zoom_R1A);
    Send_Frame_One_UnsignedChar(Zoom_R1B);
    Send_Frame_One_UnsignedChar(Zoom_R1C);
    Send_Frame_One_UnsignedChar(Zoom_R2A);
    //写入天线电阻率平均值
    Send_Frame_Float(&Average_R1A_Q, 1);
    Send_Frame_Float(&Average_R1A_I, 1);
    Send_Frame_Float(&Average_R1B_Q, 1);
    Send_Frame_Float(&Average_R1B_I, 1);
    Send_Frame_Float(&Average_R1C_Q, 1);
    Send_Frame_Float(&Average_R1C_I, 1);
    Send_Frame_Float(&Average_R2A_Q, 1);
    Send_Frame_Float(&Average_R2A_I, 1);
    //写入每一个扇区的电阻率
    unsigned char sector_i = 0;
    for(sector_i = 0;sector_i<16;sector_i++)
    {
        //写入扇区扫描点数和伽马值
        Send_Frame_UnsignedInt(Angle_sector_res+sector_i, 1);
        Send_Frame_UnsignedInt(GR_sector_reg+sector_i, 1);
        //写入电阻率
        Send_Frame_Float(R1A_Q+sector_i, 1);
        Send_Frame_Float(R1A_I+sector_i, 1);
        Send_Frame_Float(R1B_Q+sector_i, 1);
        Send_Frame_Float(R1B_I+sector_i, 1);
        Send_Frame_Float(R1C_Q+sector_i, 1);
        Send_Frame_Float(R1C_I+sector_i, 1);
        Send_Frame_Float(R2A_Q+sector_i, 1);
        Send_Frame_Float(R2A_I+sector_i, 1);
    }

    /*
    // 旧远探
    switch(state_set_temp)//兼容远探上位机，暂时使用这个代替
    // switch(state_freq)
    {
        case 0:
        {
            //写入增益
            Send_Frame_One_UnsignedChar(Zoom_TX);
            Send_Frame_One_UnsignedChar(Zoom_R1A);
            Send_Frame_One_UnsignedChar(Zoom_R1B);
            Send_Frame_One_UnsignedChar(Zoom_R1C);
            //写入天线电阻率平均值
            Send_Frame_Float(&Average_TX_Q, 1);
            Send_Frame_Float(&Average_TX_I, 1);
            Send_Frame_Float(&Average_R1A_Q, 1);
            Send_Frame_Float(&Average_R1A_I, 1);
            Send_Frame_Float(&Average_R1B_Q, 1);
            Send_Frame_Float(&Average_R1B_I, 1);
            Send_Frame_Float(&Average_R1C_Q, 1);
            Send_Frame_Float(&Average_R1C_I, 1);
            //写入每一个扇区的电阻率
            unsigned char sector_i = 0;
            for(sector_i = 0;sector_i<16;sector_i++)
            {
                //写入扇区扫描点数和伽马值
                Send_Frame_UnsignedInt(Angle_sector+sector_i, 1);
                Send_Frame_UnsignedInt(GR_sector+sector_i, 1);
                //写入电阻率
                Send_Frame_Float(TX_Q+sector_i, 1);
                Send_Frame_Float(TX_I+sector_i, 1);
                Send_Frame_Float(R1A_Q+sector_i, 1);
                Send_Frame_Float(R1A_I+sector_i, 1);
                Send_Frame_Float(R1B_Q+sector_i, 1);
                Send_Frame_Float(R1B_I+sector_i, 1);
                Send_Frame_Float(R1C_Q+sector_i, 1);
                Send_Frame_Float(R1C_I+sector_i, 1);
            }
            break;
        }
        case 1:
        {

        }
        case 2:         //2 和 3操作一致
        {
            //写入增益
            Send_Frame_One_UnsignedChar(Zoom_TX);
            Send_Frame_One_UnsignedChar(Zoom_R1A);
            Send_Frame_One_UnsignedChar(Zoom_R1B);
            Send_Frame_One_UnsignedChar(Zoom_R1C);
            Send_Frame_One_UnsignedChar(Zoom_R2A);
            Send_Frame_One_UnsignedChar(Zoom_R2B);
            Send_Frame_One_UnsignedChar(Zoom_R2C);
            //写入天线电阻率平均值
            Send_Frame_Float(&Average_TX_Q, 1);
            Send_Frame_Float(&Average_TX_I, 1);
            Send_Frame_Float(&Average_R1A_Q, 1);
            Send_Frame_Float(&Average_R1A_I, 1);
            Send_Frame_Float(&Average_R1B_Q, 1);
            Send_Frame_Float(&Average_R1B_I, 1);
            Send_Frame_Float(&Average_R1C_Q, 1);
            Send_Frame_Float(&Average_R1C_I, 1);
            Send_Frame_Float(&Average_R2A_Q, 1);
            Send_Frame_Float(&Average_R2A_I, 1);
            Send_Frame_Float(&Average_R2B_Q, 1);
            Send_Frame_Float(&Average_R2B_I, 1);
            Send_Frame_Float(&Average_R2C_Q, 1);
            Send_Frame_Float(&Average_R2C_I, 1);
            //写入每一个扇区的电阻率
            unsigned char sector_i = 0;
            for(sector_i = 0;sector_i<16;sector_i++)
            {
                //写入扇区扫描点数和伽马值
                Send_Frame_UnsignedInt(Angle_sector+sector_i, 1);
                Send_Frame_UnsignedInt(GR_sector+sector_i, 1);
                //写入电阻率
                Send_Frame_Float(TX_Q+sector_i, 1);
                Send_Frame_Float(TX_I+sector_i, 1);
                Send_Frame_Float(R1A_Q+sector_i, 1);
                Send_Frame_Float(R1A_I+sector_i, 1);
                Send_Frame_Float(R1B_Q+sector_i, 1);
                Send_Frame_Float(R1B_I+sector_i, 1);
                Send_Frame_Float(R1C_Q+sector_i, 1);
                Send_Frame_Float(R1C_I+sector_i, 1);
                Send_Frame_Float(R2A_Q+sector_i, 1);
                Send_Frame_Float(R2A_I+sector_i, 1);
                Send_Frame_Float(R2B_Q+sector_i, 1);
                Send_Frame_Float(R2B_I+sector_i, 1);
                Send_Frame_Float(R2C_Q+sector_i, 1);
                Send_Frame_Float(R2C_I+sector_i, 1);
            }
            break;
        }
        case 3:
        {
            //写入增益
            Send_Frame_One_UnsignedChar(Zoom_TX);
            Send_Frame_One_UnsignedChar(Zoom_R2A);
            Send_Frame_One_UnsignedChar(Zoom_R2B);
            Send_Frame_One_UnsignedChar(Zoom_R2C);
            Send_Frame_One_UnsignedChar(Zoom_R3U);
            Send_Frame_One_UnsignedChar(Zoom_R3D);
            //写入天线电阻率平均值
            Send_Frame_Float(&Average_TX_Q, 1);
            Send_Frame_Float(&Average_TX_I, 1);
            Send_Frame_Float(&Average_R2A_Q, 1);
            Send_Frame_Float(&Average_R2A_I, 1);
            Send_Frame_Float(&Average_R2B_Q, 1);
            Send_Frame_Float(&Average_R2B_I, 1);
            Send_Frame_Float(&Average_R2C_Q, 1);
            Send_Frame_Float(&Average_R2C_I, 1);
            Send_Frame_Float(&Average_R3U_Q, 1);
            Send_Frame_Float(&Average_R3U_I, 1);
            Send_Frame_Float(&Average_R3D_Q, 1);
            Send_Frame_Float(&Average_R3D_I, 1);
            //写入每一个扇区的电阻率
            unsigned char sector_i = 0;
            for(sector_i = 0;sector_i<16;sector_i++)
            {
                //写入扇区扫描点数和伽马值
                Send_Frame_UnsignedInt(Angle_sector+sector_i, 1);
                Send_Frame_UnsignedInt(GR_sector+sector_i, 1);
                //写入电阻率
                Send_Frame_Float(TX_Q+sector_i, 1);
                Send_Frame_Float(TX_I+sector_i, 1);
                Send_Frame_Float(R2A_Q+sector_i, 1);
                Send_Frame_Float(R2A_I+sector_i, 1);
                Send_Frame_Float(R2B_Q+sector_i, 1);
                Send_Frame_Float(R2B_I+sector_i, 1);
                Send_Frame_Float(R2C_Q+sector_i, 1);
                Send_Frame_Float(R2C_I+sector_i, 1);
                Send_Frame_Float(R3U_Q+sector_i, 1);
                Send_Frame_Float(R3U_I+sector_i, 1);
                Send_Frame_Float(R3D_Q+sector_i, 1);
                Send_Frame_Float(R3D_I+sector_i, 1);
            }
            break;
        }
        case 4://故意这么使用，400k与2M ，后续更改
        {}
        case 5:
        {
            //写入增益
            Send_Frame_One_UnsignedChar(Zoom_TX);
            Send_Frame_One_UnsignedChar(Zoom_R3U);
            Send_Frame_One_UnsignedChar(Zoom_R3D);
            //写入天线电阻率平均值
            Send_Frame_Float(&Average_TX_Q, 1);
            Send_Frame_Float(&Average_TX_I, 1);
            Send_Frame_Float(&Average_R3U_Q, 1);
            Send_Frame_Float(&Average_R3U_I, 1);
            Send_Frame_Float(&Average_R3D_Q, 1);
            Send_Frame_Float(&Average_R3D_I, 1);
            //写入每一个扇区的电阻率
            unsigned char sector_i = 0;
            for(sector_i = 0;sector_i<16;sector_i++)
            {
                //写入扇区扫描点数和伽马值
                Send_Frame_UnsignedInt(Angle_sector+sector_i, 1);
                Send_Frame_UnsignedInt(GR_sector+sector_i, 1);
                //写入电阻率
                Send_Frame_Float(TX_Q+sector_i, 1);
                Send_Frame_Float(TX_I+sector_i, 1);
                Send_Frame_Float(R3U_Q+sector_i, 1);
                Send_Frame_Float(R3U_I+sector_i, 1);
                Send_Frame_Float(R3D_Q+sector_i, 1);
                Send_Frame_Float(R3D_I+sector_i, 1);
            }
            break;
        }
    }*/
}
