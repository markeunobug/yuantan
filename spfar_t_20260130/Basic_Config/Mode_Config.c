/*
 * Mode_Config.c
 *
 *  Created on: 2023年6月12日
 *      Author: xikai
 */

#include "Mode_Config.h"


void Mode_Control_Init(void)
{
    //发射频率控制引脚初始化

    Enable_Pullup_Gpio(TRAN_ANT_PIN0);
    Enable_Pullup_Gpio(TRAN_ANT_PIN1);
    Enable_Pullup_Gpio(TRAN_ANT_PIN2);//上拉发射控制天线1 2 3，提高驱动能力
    Set_GPIO_Out(TRAN_ANT_PIN0);
    Set_GPIO_Out(TRAN_ANT_PIN1);
    Set_GPIO_Out(TRAN_ANT_PIN2);       //发射控制天线1 2 3配置为输出方向
    Set_GPIO_Low(TRAN_ANT_PIN0);
    Set_GPIO_Low(TRAN_ANT_PIN1);
    Set_GPIO_Low(TRAN_ANT_PIN2);        //发射控制天线1 2 3默认输出低

    /*
     * 内部上拉功能验证
     */
    /*Set_GPIO_High(TRAN_ANT_PIN1);
    Set_GPIO_Low(TRAN_ANT_PIN1);
    Disable_Pullup_Gpio(TRAN_ANT_PIN1);
    Set_GPIO_High(TRAN_ANT_PIN1);*/



}

//发射电路调谐控制，输入参数为当前频率
void Transmit_Freq_Control()
{
    if(state_high==0)
        Set_GPIO_Low(TRAN_ANT_PIN0);   //400k
    else
        Set_GPIO_High(TRAN_ANT_PIN0); //2000k

    switch (state_low)
    {
         case 0://00:1K
         {
             Set_GPIO_Low(TRAN_ANT_PIN1);
             Set_GPIO_Low(TRAN_ANT_PIN2);
             break;
         }
         case 1://01:4K
         {
             Set_GPIO_High(TRAN_ANT_PIN1);
             Set_GPIO_Low(TRAN_ANT_PIN2);
             break;
         }
         case 2://10:20K
         {
           Set_GPIO_Low(TRAN_ANT_PIN1);
           Set_GPIO_High(TRAN_ANT_PIN2);
           break;
         }
         case 3://11:50K
         {
           Set_GPIO_High(TRAN_ANT_PIN1);
           Set_GPIO_High(TRAN_ANT_PIN2);
           break;
         }
         default:
         {
            Set_GPIO_High(TRAN_ANT_PIN1);
            Set_GPIO_High(TRAN_ANT_PIN2);
            break;
         }

    }


    /*switch (state_freq)
    {
        case 0://001:4K
        {
            Set_GPIO_High(TRAN_ANT_PIN0);
            Set_GPIO_Low(TRAN_ANT_PIN1);
            Set_GPIO_Low(TRAN_ANT_PIN2);
            break;
        }
        case 1://010:10K
        {
            Set_GPIO_Low(TRAN_ANT_PIN0);
            Set_GPIO_High(TRAN_ANT_PIN1);
            Set_GPIO_Low(TRAN_ANT_PIN2);
            break;
        }
        case 2://011:40K
        {
            Set_GPIO_High(TRAN_ANT_PIN0);
            Set_GPIO_High(TRAN_ANT_PIN1);
            Set_GPIO_Low(TRAN_ANT_PIN2);
            break;
        }
        case 3://100:100K
        {
            Set_GPIO_Low(TRAN_ANT_PIN0);
            Set_GPIO_Low(TRAN_ANT_PIN1);
            Set_GPIO_High(TRAN_ANT_PIN2);
            break;
        }
        case 4://101:400K
        {
            Set_GPIO_High(TRAN_ANT_PIN0);
            Set_GPIO_Low(TRAN_ANT_PIN1);
            Set_GPIO_High(TRAN_ANT_PIN2);
            break;
        }
        default://000:不发射
        {
            Set_GPIO_Low(TRAN_ANT_PIN0);
            Set_GPIO_Low(TRAN_ANT_PIN1);
            Set_GPIO_Low(TRAN_ANT_PIN2);
            break;
        }
    }*/

}


