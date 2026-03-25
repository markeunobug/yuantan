#include "Mode_Freq.h"
//#include "FPGA_SDRAM_Config.h"
//float Test_Temp;
//float Test_gXYZ[3];

void Mode_Freq(unsigned char ID)
{
    //采集中标志位
    if(SamplingFlag == 1)
    {

        MS578H_Require_Angle();  //请求井斜方位等信息

        //配置Timer1，定时5S,单个频率工作时间为5s
        ConfigCpuTimer(&CpuTimer1, 150, Frame_Interval*1000000);
        //开始计时
        StartCpuTimer1();
        //帧采集初始化
        Clear_BUF_Uint64(Res_sector,288);           //清除SampleData

        Clear_BUF_Uchar(PageBuffer,1100);          //清除PageBuffer

        Clear_BUF_Uint16(Angle_sector_res,16);         //清除Angle_sector

        if(Sample_gama == 5)
        {
            Sample_gama = 0;

            Clear_BUF_Uint16(GR_sector,16);         //清除GR_sector

            Clear_BUF_Uint16(Angle_sector_gama,16);         //清除GR_sector

            GRprobe_total = 0;//清除GR_total
        }

        pagecnt = 0;
        FlashSendSum = 0;
        MicroFrame_Cnt = 0;
        state_freq = ID;
        //读取震动温度信号
        Read_ADC_Value();
        //读取实时时间
        DS1629_ReadTimeAll();
        //读取当前温度

        MCP23008_Set_Amp(); //调整滤波放大电路切换进入相应状态

        Transmit_Freq_Control(); //控制发射电路选择


        FPGA_Set_Freq();        //配置FPGA发射频率
    //    FPGA_DATA_write((unsigned int *)FPGA_Freq_Addr,CMD_SetFreq[state_freq]);

    //    FPGA_DATA_write((unsigned int *)FPGA_Freq_Addr,CMD_SetFreq[state_freq]);

   //     watch_paramter=*((unsigned int *)FPGA_Freq_Addr);

  //      RS485_SendOneByte(unsigned char(watch_paramter));

 //       FPGA_Set_Freq();        //配置FPGA发射频率

        FPGA_Set_AMP();         //配置FPGA控制DA输出幅度

      //  FPGA_Set_AMP();         //配置FPGA控制DA输出幅度

        Zoom_TX =  CMD_SetAmp[state_freq];

        DELAY_MS(20);                             //延时20ms
   //     MS578H_Require_sector();//扇区角度信息
 //       Can_Set_Freq_AMP(0,ID,0x13);
 //       Can_Set_Freq_AMP(1,ID,0x13);
  //      Can_Set_Freq_AMP(2,ID,0x13);
  //     Can_Set_Freq_AMP(3,ID,0x13);
  //      Can_Set_Freq_AMP(4,ID,0x13);
 //       Can_Set_Freq_AMP(5,ID,0x13);
 //       Can_Set_Freq_AMP(6,ID,0x13);
 //       Can_Set_Freq_AMP(7,ID,0x13);
 //       Can_Set_Freq_AMP(8,ID,0x13);

        //开始数据采集
        while(MicroFrame_Cnt < MicroFrame_Sum)
        {
            GET_Sector_Data(Res_sector,GR_sector,Angle_sector_res,Angle_sector_gama);
            //用于定时两次采集之前的时间
/*            if(MicroFrame_Cnt == 0)    //第一次启动采集，配置Timer2，定时小帧间隔
            {
                ConfigCpuTimer(&CpuTimer2, 150, MicroFrame_Interval);
            }
            else                       //之后循环重装Timer2，定时小帧间隔
            {
                ReloadCpuTimer2();
            }
            //清空当前GAMMA计数值Buffer
//            GAMMA_Current_Count = 0;
            //开始计时
            StartCpuTimer2();
            //读取当前数据，不区分频率，全部读取
   //         DELAY_MS(1);        //等待获取到sector
            GET_Sector_Data(SampleData,GR_sector,Angle_sector,Angle_sector_gama);
       //     get_angledata(SampleData,Angle_sector,Angle_sector_gama);
            //等待当前帧结束标志，标志位在定时器2中断内更新
            while(MicroFrame_Flag==0)
            {
                DELAY_US(1);                        //实测不加该语句，会出现死循环的情况
            }
            //复位标志位
            MicroFrame_Flag = 0;

            //结束定时器定时
            StopCpuTimer2();*/
            //帧数量增加
            MicroFrame_Cnt++;
        }

        //处理数据，取平均
        SingleAverage_data_res(Res_sector,Angle_sector_res);
        if(Sample_gama == 4)
        {
            SingleAverage_data_gama(GR_sector,Angle_sector_gama);
        }

        //向FLASH写入当前采样点获得的数据
        Send_Frame_Start();                         //存入各个频率相同的信息
        //写入扫描扇区总点数
        Send_Frame_UnsignedInt(&MicroFrame_Cnt, 1);
        //写入平均伽马
        Send_Frame_UnsignedInt(&GRprobe_total, 1);

        //写入每个频率下的特异化参数
        Send_Frame_WithFreq();
        //写入校验和
        Send_Frame_End();

 //       SamplePoint_Log++;                         //记录采集的点数
        Sample_gama ++;
        while(Frame_Flag==0)
        {
            DELAY_US(1);                        //等待5s到，该频率下测试结束
        }
        Frame_Flag = 0;                         //恢复标志位
        StopCpuTimer1();                        //停止该定时器定时
    }
}
