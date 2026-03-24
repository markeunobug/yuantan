
#include "ADC_Config.h"


void ADC_init() //顺序采集 四个通道
{
    //ADC基本初始化
    InitAdc();

    AdcRegs.ADCTRL1.bit.ACQ_PS=0x2;         //采样窗口6个ADCCLK
    AdcRegs.ADCTRL3.bit.ADCCLKPS=0xf;       // ADCCLK=HSPCLK/6=12.5MHz，最高25MHz

    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 0x1; //允许向CPU发出中断请求
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;     //清除SEQ1中断标志位
    AdcRegs.ADCTRL1.bit.SEQ_CASC=0x01;      //级联模式
    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 0x1;     //复位ADC模块
    AdcRegs.ADCTRL1.bit.CONT_RUN = 1;       //连续采样
    AdcRegs.ADCTRL3.bit.SMODE_SEL=0x0;      //顺序采样模式
    AdcRegs.ADCTRL1.bit.SEQ_OVRD = 1;       //在序列发生器结束时发生回绕

    AdcRegs.ADCMAXCONV.all=0x0003;          //五个通道顺序采样，AD_gZ、AD_gY、AD_gX、TEMP、TEMP_MAG
    AdcRegs.ADCCHSELSEQ1.bit.CONV00=0x0;    //ADCINA0
    AdcRegs.ADCCHSELSEQ1.bit.CONV01=0x1;    //ADCINA1
    AdcRegs.ADCCHSELSEQ1.bit.CONV02=0x2;    //ADCINA2
    AdcRegs.ADCCHSELSEQ1.bit.CONV03=0x8;    //ADCINB0

    //软件触发开启AD采样
    AdcRegs.ADCTRL2.bit.SOC_SEQ1=1;
}

void Read_ADC_Value_gXYZ(float *ADC_Value)
{
    //等待ADC采集完成标志
    while (AdcRegs.ADCST.bit.INT_SEQ1== 0);
    //清空标志位
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
    //读取ADC转换结果
    ADC_Value[0] = ((AdcRegs.ADCRESULT0)>>4)*ADC_REF/4096;
    ADC_Value[1] = ((AdcRegs.ADCRESULT1)>>4)*ADC_REF/4096;
    ADC_Value[2] = ((AdcRegs.ADCRESULT2)>>4)*ADC_REF/4096;
}


float getAD_Temperature(void)
{
    //等待ADC采集完成标志
    while (AdcRegs.ADCST.bit.INT_SEQ1== 0);
    //清空标志位
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
    //读取ADC转换结果
    float ADC_Value = ((AdcRegs.ADCRESULT3)>>4)*ADC_REF/4096;
    return ADC_Value;
}




