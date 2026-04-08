#include "Mode_Freq.h"

float Test_Temp;
float Test_gXYZ[3];
void Mode_Freq(unsigned char state_low_ID, unsigned char state_high_ID)
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
        Clear_BUF_Uint16(GR_sector,16);         //清除GR_sector
        GRprobe_total = 0;//清除GR_total
        pagecnt = 0;
        FlashSendSum = 0;
        MicroFrame_Cnt = 0;
        
        //将state_low与state_high映射到对应的状态state_freq上
        state_freq = state_low_ID + ((state_high_ID)<<2);//映射到 CMD_SetFreq[state_freq] 1-8 中. 4*2 = 8种状态
        Zoom_R1A =state_high_ID;
        Zoom_R1B =state_low_ID;

        //读取震动信号
        Read_ADC_Value_gXYZ(Test_gXYZ);
        //读取实时时间
        DS1629_ReadTimeAll();
        //读取当前温度
        Test_Temp = getAD_Temperature();

 //       MCP23008_Set_Amp(); //调整滤波放大电路切换进入相应状态

        Transmit_Freq_Control(); //控制发射电路选择

        FPGA_Set_Freq();        //配置FPGA发射频率

        FPGA_Set_AMP();         //配置FPGA控制DA输出幅度

        DELAY_MS(20);                             //延时20ms

        //开始数据采集
        while(MicroFrame_Cnt < MicroFrame_Sum)
        {
            MS578H_Require_sector();//扇区角度信息
            GET_Sector_Data(Res_sector,GR_sector,Angle_sector_res,Angle_sector_gama);
            //帧数量增加
            MicroFrame_Cnt++;
        }

        //处理数据，取平均
        SingleAverage_data_res(Res_sector,Angle_sector_res);
        SingleAverage_data_gama(GR_sector,Angle_sector_gama);

        //向flash存入数据
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

     //  SamplePoint_Log++;                         //记录采集的点数

        while(Frame_Flag==0)
        {
            DELAY_US(1);                        //等待5s到，该频率下测试结束
        }
        Frame_Flag = 0;                         //恢复标志位
        StopCpuTimer1();                        //停止该定时器定时
    }
}
