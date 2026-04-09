#include "Basic_Config.h"
#include "Mode_Freq.h"


/**
 * main.c
 */
int main(void)
{
    System_Init();              //系统时钟、GPIO、外设中断向量表PIE初始化
    // /*
    Timer_Init();               //初始化三个定时器
    ADC_init();                 //ADC初始化函数
    FPGA_SDRAM_Init();          //FPGA接口SDRAM,XINTF初始化
    Mode_Control_Init();        //控制发射频率 GPIO 2~4管脚
    DS1629_Init();              //DS1629初始化,10.14修改
    MCP23008_Init();            //MCP23008初始化,GPIO 0用作i2c scl, 1用作i2c sda
    EXT_Flash_PortInit();       //外置FLASH初始化
    RS485_Init();               //RS485初始化,scib,GPIO18和19
//    GAMMA_Count_EXTI_Init();    //GAMMA计数初始化
//    CAN_Init();                 //CAN接口初始化
    MS578H_Uart_Init();         //MS578H RS485初始化,scic,GPIO62和63

    // */

    /*
     * CPU级开启中断开启
     * f28335共有16个CPU级中断线
     * 2个不可屏蔽中断 reset 和 nmi
     * 14个可屏蔽中断 INT1~INT14
     * INT13单分配给定时器1，INT14单分配给定时器2
     * INT1~12中断线复用，每支线管理8个pie中断源
     * INT1管理WAKE TIME0 ADC XINT2 XINT1等
     * INT8管理SCIC I2C等
     * INT9管理ECAN1 ECAN0 ECAN1 SCIB SCIA 等
     */

// /*     
    IER |= M_INT13;             //开启定时器1的CPU中断
    IER |= M_INT14;             //开启定时器2的CPU中断
    IER |= M_INT1;              //开启CPU中断INT1
    IER |= M_INT9;              //开启CPU中断INT9
  //  IER |= M_INT8;
    //PIE级中断开启
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;   //开启定时器0的PIE中断,TINT0(TIMER0)
    PieCtrlRegs.PIEIER9.bit.INTx3 = 1;   //开启SCIB的接收中断,SCIRXINTB
 //   PieCtrlRegs.PIEIER8.bit.INTx5 = 1;   //开启SCIC的接收中断,SCIRXINTC(中断优先级需要调整)
   // PieCtrlRegs.PIEIER1.bit.INTx4 = 1;   //开始XINT1，GPIO外部中断,XINT1,用于伽马脉冲计数
    EINT;                               //使能全局总中断
    ERTM;                               //使能全局实时中断
    //使能全局中断
// */


    FPGA_Stop();                                //默认不发射
/*
    FLASH_ReadSettings();                       //读取存储器内上次参数设定

  //  DELAY_MS(500);
//  Flash_EraseAll();
    Flash_Read_SamplePoint();                   //上电时读取帧数，从外部FLASH
*/
    Delay_Process();

    LED_Init();
//    state_main=0;
    while(1)
    {
        LED_Toggle();
        // 主函数
        switch(state_main)
        {
            case 0:                     //循环测试各个频率
            {
                static unsigned char set_fre = 0;
                set_fre++;
                set_fre = set_fre%4;
                Mode_Freq(set_fre);
                break;
            }
            case 1:                     //单独测试频率0
            {
                Mode_Freq(0);
                break;
            }
            case 2:                     //单独测试频率1
            {
                Mode_Freq(1);
                break;
            }
            case 3:                     //单独测试频率2
            {
                Mode_Freq(2);
                break;
            }
            case 4:                     //单独测试频率3
            {
                Mode_Freq(3);
                break;
            }
            case 5:                     //单独测试频率4
            {
                Mode_Freq(3);
                // Mode_Freq(4);
                break;
            }
            default:
            {
                break;
            }
        }

        if(Framedata_rxflag)            //串口接收命令结束，对命令进行解码
        {
            Decode_Cmd();

            Framedata_rxflag = 0;

            Clear_BUF_Uchar(RxBuf,30);//这里清除是有意义的，在高速模式下460800，不清除会有误触发现象。
        }
    }
}
