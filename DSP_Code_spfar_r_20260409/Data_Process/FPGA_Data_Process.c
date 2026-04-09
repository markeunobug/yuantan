/*
 * FPGA_Buffer.c
 *
 *  Created on: 2023年7月2日
 *      Author: xikai
 */

#include <FPGA_Data_Process.h>

static float Normalize_Angle_Deg(float angle_deg)
{
    while(angle_deg >= 180.0f)
    {
        angle_deg -= 360.0f;
    }

    while(angle_deg < -180.0f)
    {
        angle_deg += 360.0f;
    }

    return angle_deg;
}

static float Calc_Phase_Deg(float q_val,float i_val)
{
    return atan2f(q_val,i_val) * 180.0f / PI;
}

static void Compensate_IQ_By_Ref(float *q_val,float *i_val,float ref_q,float ref_i)
{
    float amp = 0.0f;
    float phase_deg = 0.0f;
    float ref_phase_deg = 0.0f;
    float relative_phase_deg = 0.0f;
    float relative_phase_rad = 0.0f;

    amp = sqrtf((*q_val) * (*q_val) + (*i_val) * (*i_val));
    phase_deg = Calc_Phase_Deg(*q_val,*i_val);
    ref_phase_deg = Calc_Phase_Deg(ref_q,ref_i);
    relative_phase_deg = Normalize_Angle_Deg(phase_deg - ref_phase_deg);
    relative_phase_rad = relative_phase_deg * PI / 180.0f;

    *i_val = amp * sinf(relative_phase_rad);
    *q_val = amp * cosf(relative_phase_rad);
}

void Relative_Phase_Compensate(void)
{
    unsigned char sector_i = 0;
    float ref_q = 0.0f;
    float ref_i = 0.0f;

    ref_q = Average_R2A_Q;
    ref_i = Average_R2A_I;
    Compensate_IQ_By_Ref(&Average_R1A_Q,&Average_R1A_I,ref_q,ref_i);
    Compensate_IQ_By_Ref(&Average_R1B_Q,&Average_R1B_I,ref_q,ref_i);
    Compensate_IQ_By_Ref(&Average_R1C_Q,&Average_R1C_I,ref_q,ref_i);
    Compensate_IQ_By_Ref(&Average_R2A_Q,&Average_R2A_I,ref_q,ref_i);

    for(sector_i = 0;sector_i < 16;sector_i++)
    {
        ref_q = R2A_Q[sector_i];
        ref_i = R2A_I[sector_i];
        Compensate_IQ_By_Ref(&R1A_Q[sector_i],&R1A_I[sector_i],ref_q,ref_i);
        Compensate_IQ_By_Ref(&R1B_Q[sector_i],&R1B_I[sector_i],ref_q,ref_i);
        Compensate_IQ_By_Ref(&R1C_Q[sector_i],&R1C_I[sector_i],ref_q,ref_i);
        Compensate_IQ_By_Ref(&R2A_Q[sector_i],&R2A_I[sector_i],ref_q,ref_i);
    }
}

//处理FPGA读到的数据，按照角度进行累加

struct FPGA_data data;
void GET_Sector_Data(signed long long temp_res[],unsigned int temp_gama[],unsigned int num_res[],unsigned int num_gama[])
{
    struct FPGA_data data;
    unsigned char i=0;
    unsigned int time_out=0;
    ++num_res[Cur_sector];
    ++num_gama[Cur_sector];
//    GRprobe_sector = 0;
    for(i=0;i<MicroFrame_Num;i++)
    {
      time_out=0;
      data=FPGA_ReadData();
      temp_res[18*Cur_sector]+=data.R1A_Q;
      temp_res[18*Cur_sector+1]+=data.R1A_I;
      temp_res[18*Cur_sector+2]+=data.R1B_Q;
      temp_res[18*Cur_sector+3]+=data.R1B_I;
      temp_res[18*Cur_sector+4]+=data.R1C_Q;
      temp_res[18*Cur_sector+5]+=data.R1C_I;
      temp_res[18*Cur_sector+6]+=data.R2A_Q;
      temp_res[18*Cur_sector+7]+=data.R2A_I;
      temp_res[18*Cur_sector+8]+=data.R2B_Q;
      temp_res[18*Cur_sector+9]+=data.R2B_I;
      temp_res[18*Cur_sector+10]+=data.R2C_Q;
      temp_res[18*Cur_sector+11]+=data.R2C_I;
      temp_res[18*Cur_sector+12]+=data.R3U_Q;
      temp_res[18*Cur_sector+13]+=data.R3U_I;
      temp_res[18*Cur_sector+14]+=data.R3D_Q;
      temp_res[18*Cur_sector+15]+=data.R3D_I;
      temp_res[18*Cur_sector+16]+=data.TX_Q;
      temp_res[18*Cur_sector+17]+=data.TX_I;
      temp_gama[Cur_sector] += data.GAMMA;
      if(i==MicroFrame_Num/2)
          MS578H_Require_sector();//更新扇区角度信息
      while((GpioDataRegs.GPADAT.bit.GPIO31==1)&&(time_out<(5000/MicroFrame_Num)))
      {
          time_out++;
          DELAY_US(10);
       }

    }
 //   GR_sector[Cur_sector]+=GRprobe_sector;//扇区伽马累计
}

//对FPGA获取到的数据进行取平均
void SingleAverage_data_res(signed long long temp[],unsigned int num[])
{
    Average_R1A_Q = 0;
    Average_R1A_I = 0;
    Average_R1B_Q = 0;
    Average_R1B_I = 0;
    Average_R1C_Q = 0;
    Average_R1C_I = 0;
    Average_R2A_Q = 0;
    Average_R2A_I = 0;
    Average_R2B_Q = 0;
    Average_R2B_I = 0;
    Average_R2C_Q = 0;
    Average_R2C_I = 0;
    Average_R3U_Q = 0;
    Average_R3U_I = 0;
    Average_R3D_Q = 0;
    Average_R3D_I = 0;
    Average_TX_Q = 0;
    Average_TX_I = 0;
    Max_avg_R1A_Q=0;
    Max_avg_R1A_I=0;
    Max_avg_R2A_Q=0;
    Max_avg_R2A_I=0;
    unsigned char sector_i=0;
    for(sector_i=0;sector_i<16;++sector_i)
    {
       /*
        * 各扇区累加值除系数
        * 积累6000次（除以6000），20位ADC（除以2^20)，16位码表（除以2^16），ADC量程8192mV(乘以8192），码表幅度0.5（乘以2），公式带有0.5（乘以2），变为VPP（乘以2）
        */
        R1A_Q[sector_i]=(num[sector_i]==0)?0:temp[18*sector_i+0] /AD4020_Scale_Coefficient;
        R1A_I[sector_i]=(num[sector_i]==0)?0:temp[18*sector_i+1] /AD4020_Scale_Coefficient;
        R1B_Q[sector_i]=(num[sector_i]==0)?0:temp[18*sector_i+2] /AD4020_Scale_Coefficient;
        R1B_I[sector_i]=(num[sector_i]==0)?0:temp[18*sector_i+3] /AD4020_Scale_Coefficient;
        R1C_Q[sector_i]=(num[sector_i]==0)?0:temp[18*sector_i+4] /AD4020_Scale_Coefficient;
        R1C_I[sector_i]=(num[sector_i]==0)?0:temp[18*sector_i+5] /AD4020_Scale_Coefficient;
        R2A_Q[sector_i]=(num[sector_i]==0)?0:temp[18*sector_i+6] /AD4020_Scale_Coefficient;
        R2A_I[sector_i]=(num[sector_i]==0)?0:temp[18*sector_i+7] /AD4020_Scale_Coefficient;
        R2B_Q[sector_i]=(num[sector_i]==0)?0:temp[18*sector_i+8] /AD4020_Scale_Coefficient;
        R2B_I[sector_i]=(num[sector_i]==0)?0:temp[18*sector_i+9] /AD4020_Scale_Coefficient;
        R2C_Q[sector_i]=(num[sector_i]==0)?0:temp[18*sector_i+10]/AD4020_Scale_Coefficient;
        R2C_I[sector_i]=(num[sector_i]==0)?0:temp[18*sector_i+11]/AD4020_Scale_Coefficient;
        R3U_Q[sector_i]=(num[sector_i]==0)?0:temp[18*sector_i+12]/AD4020_Scale_Coefficient;
        R3U_I[sector_i]=(num[sector_i]==0)?0:temp[18*sector_i+13]/AD4020_Scale_Coefficient;
        R3D_Q[sector_i]=(num[sector_i]==0)?0:temp[18*sector_i+14]/AD4020_Scale_Coefficient;
        R3D_I[sector_i]=(num[sector_i]==0)?0:temp[18*sector_i+15]/AD4020_Scale_Coefficient;
        TX_Q[sector_i]=(num[sector_i]==0)?0:temp[18*sector_i+16] /AD4020_Scale_Coefficient;
        TX_I[sector_i]=(num[sector_i]==0)?0:temp[18*sector_i+17] /AD4020_Scale_Coefficient;
        /*
         * 各天线测量值80次累加
         */
        Average_R1A_Q += R1A_Q[sector_i];
        Average_R1A_I += R1A_I[sector_i];
        Average_R1B_Q += R1B_Q[sector_i];
        Average_R1B_I += R1B_I[sector_i];
        Average_R1C_Q += R1C_Q[sector_i];
        Average_R1C_I += R1C_I[sector_i];
        Average_R2A_Q += R2A_Q[sector_i];
        Average_R2A_I += R2A_I[sector_i];
        Average_R2B_Q += R2B_Q[sector_i];
        Average_R2B_I += R2B_I[sector_i];
        Average_R2C_Q += R2C_Q[sector_i];
        Average_R2C_I += R2C_I[sector_i];
        Average_R3U_Q += R3U_Q[sector_i];
        Average_R3U_I += R3U_I[sector_i];
        Average_R3D_Q += R3D_Q[sector_i];
        Average_R3D_I += R3D_I[sector_i];
        Average_TX_Q  += TX_Q[sector_i];
        Average_TX_I  += TX_I[sector_i];

//        GRprobe_total += GR_sector[sector_i];//累计伽马总值
        /*
         * 各扇区平均值
         */
        R1A_Q[sector_i] /= (MicroFrame_Num*num[sector_i]);
        R1A_I[sector_i] /= (MicroFrame_Num*num[sector_i]);
        R1B_Q[sector_i] /= (MicroFrame_Num*num[sector_i]);
        R1B_I[sector_i] /= (MicroFrame_Num*num[sector_i]);
        R1C_Q[sector_i] /= (MicroFrame_Num*num[sector_i]);
        R1C_I[sector_i] /= (MicroFrame_Num*num[sector_i]);
        R2A_Q[sector_i] /= (MicroFrame_Num*num[sector_i]);
        R2A_I[sector_i] /= (MicroFrame_Num*num[sector_i]);
        R2B_Q[sector_i] /= (MicroFrame_Num*num[sector_i]);
        R2B_I[sector_i] /= (MicroFrame_Num*num[sector_i]);
        R2C_Q[sector_i] /= (MicroFrame_Num*num[sector_i]);
        R2C_I[sector_i] /= (MicroFrame_Num*num[sector_i]);
        R3U_Q[sector_i] /= (MicroFrame_Num*num[sector_i]);
        R3U_I[sector_i] /= (MicroFrame_Num*num[sector_i]);
        R3D_Q[sector_i] /= (MicroFrame_Num*num[sector_i]);
        R3D_I[sector_i] /= (MicroFrame_Num*num[sector_i]);
        TX_Q[sector_i]  /= (MicroFrame_Num*num[sector_i]);
        TX_I[sector_i]  /= (MicroFrame_Num*num[sector_i]);
        /*
         * 1a天线和2a天线旋转一周极大值
         */
 //       GR_sector[sector_i]  = (num[sector_i]==0)?0:(GR_sector[sector_i]*20)/num[sector_i];//50ms累计一次公式

        Max_avg_R1A_Q=Max_avg_R1A_Q<abs(R1A_Q[sector_i])?abs(R1A_Q[sector_i]):Max_avg_R1A_Q;
        Max_avg_R1A_I=Max_avg_R1A_I<abs(R1A_I[sector_i])?abs(R1A_I[sector_i]):Max_avg_R1A_I;
        Max_avg_R2A_Q=Max_avg_R2A_Q<abs(R2A_Q[sector_i])?abs(R2A_Q[sector_i]):Max_avg_R2A_Q;
        Max_avg_R2A_I=Max_avg_R2A_I<abs(R2A_I[sector_i])?abs(R2A_I[sector_i]):Max_avg_R2A_I;
    }


    Average_R1A_Q /=(MicroFrame_Num*MicroFrame_Cnt);
    Average_R1A_I /=(MicroFrame_Num*MicroFrame_Cnt);
    Average_R1B_Q /=(MicroFrame_Num*MicroFrame_Cnt);
    Average_R1B_I /=(MicroFrame_Num*MicroFrame_Cnt);
    Average_R1C_Q /=(MicroFrame_Num*MicroFrame_Cnt);
    Average_R1C_I /=(MicroFrame_Num*MicroFrame_Cnt);
    Average_R2A_Q /=(MicroFrame_Num*MicroFrame_Cnt);
    Average_R2A_I /=(MicroFrame_Num*MicroFrame_Cnt);
    Average_R2B_Q /=(MicroFrame_Num*MicroFrame_Cnt);
    Average_R2B_I /=(MicroFrame_Num*MicroFrame_Cnt);
    Average_R2C_Q /=(MicroFrame_Num*MicroFrame_Cnt);
    Average_R2C_I /=(MicroFrame_Num*MicroFrame_Cnt);
    Average_R3U_Q /=(MicroFrame_Num*MicroFrame_Cnt);
    Average_R3U_I /=(MicroFrame_Num*MicroFrame_Cnt);
    Average_R3D_Q /=(MicroFrame_Num*MicroFrame_Cnt);
    Average_R3D_I /=(MicroFrame_Num*MicroFrame_Cnt);
    Average_TX_Q  /=(MicroFrame_Num*MicroFrame_Cnt);
    Average_TX_I  /=(MicroFrame_Num*MicroFrame_Cnt);
 //   GRprobe_total /=4;//累计4s
}

void SingleAverage_data_gama(unsigned int temp[],unsigned int num[])
{
    unsigned char sector_i=0;
    for(sector_i=0;sector_i<16;++sector_i)
    {
        GRprobe_total += temp[sector_i];//累计伽马总值
        GR_sector_reg[sector_i]  = (num[sector_i]==0)?0:(temp[sector_i]*20)/num[sector_i];//50ms累计一次公式
        Angle_sector_gama_reg[sector_i] = num[sector_i];
    }
    GRprobe_total_reg =GRprobe_total*2/45;//累计22.5s
}




