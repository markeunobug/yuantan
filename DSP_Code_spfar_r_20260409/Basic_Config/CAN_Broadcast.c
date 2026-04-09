/*
 * CAN_Broadcast.c
 *
 *  Created on: 2024年9月3日
 *      Author: zxk
 */

#include "CAN_Broadcast.h"

/*
 * ID_Number 接受滤波放大器的通道，R1A为0 发射驱动电路为8
 *
 */
void Can_Set_Freq_AMP(unsigned char ID_Number,unsigned char Freq_Word,unsigned char Amp_Word)
{
    switch(ID_Number)
    {
        case 0:
            ECanbMboxes.MBOX0.MDL.word.HI_WORD = CAN_Local_ID;            //来源ID
            ECanbMboxes.MBOX0.MDL.word.LOW_WORD = CAN_Set_CMD;            //命令
            ECanbMboxes.MBOX0.MDH.byte.BYTE4 = 0X00;                      //数据
            ECanbMboxes.MBOX0.MDH.byte.BYTE5 = 0X00;                      //数据
            ECanbMboxes.MBOX0.MDH.byte.BYTE6 = Freq_Word;                 //数据
            ECanbMboxes.MBOX0.MDH.byte.BYTE7 = Amp_Word;                  //数据
            break;
        case 1:
            ECanbMboxes.MBOX1.MDL.word.HI_WORD = CAN_Local_ID;            //来源ID
            ECanbMboxes.MBOX1.MDL.word.LOW_WORD = CAN_Set_CMD;            //命令
            ECanbMboxes.MBOX1.MDH.byte.BYTE4 = 0X00;                      //数据
            ECanbMboxes.MBOX1.MDH.byte.BYTE5 = 0X00;                      //数据
            ECanbMboxes.MBOX1.MDH.byte.BYTE6 = Freq_Word;                 //数据
            ECanbMboxes.MBOX1.MDH.byte.BYTE7 = Amp_Word;                  //数据
            break;
        case 2:
            ECanbMboxes.MBOX2.MDL.word.HI_WORD = CAN_Local_ID;            //来源ID
            ECanbMboxes.MBOX2.MDL.word.LOW_WORD = CAN_Set_CMD;            //命令
            ECanbMboxes.MBOX2.MDH.byte.BYTE4 = 0X00;                      //数据
            ECanbMboxes.MBOX2.MDH.byte.BYTE5 = 0X00;                      //数据
            ECanbMboxes.MBOX2.MDH.byte.BYTE6 = Freq_Word;                 //数据
            ECanbMboxes.MBOX2.MDH.byte.BYTE7 = Amp_Word;                  //数据
            break;
        case 3:
            ECanbMboxes.MBOX3.MDL.word.HI_WORD = CAN_Local_ID;            //来源ID
            ECanbMboxes.MBOX3.MDL.word.LOW_WORD = CAN_Set_CMD;            //命令
            ECanbMboxes.MBOX3.MDH.byte.BYTE4 = 0X00;                      //数据
            ECanbMboxes.MBOX3.MDH.byte.BYTE5 = 0X00;                      //数据
            ECanbMboxes.MBOX3.MDH.byte.BYTE6 = Freq_Word;                 //数据
            ECanbMboxes.MBOX3.MDH.byte.BYTE7 = Amp_Word;                  //数据
            break;
        case 4:
            ECanbMboxes.MBOX4.MDL.word.HI_WORD = CAN_Local_ID;            //来源ID
            ECanbMboxes.MBOX4.MDL.word.LOW_WORD = CAN_Set_CMD;            //命令
            ECanbMboxes.MBOX4.MDH.byte.BYTE4 = 0X00;                      //数据
            ECanbMboxes.MBOX4.MDH.byte.BYTE5 = 0X00;                      //数据
            ECanbMboxes.MBOX4.MDH.byte.BYTE6 = Freq_Word;                 //数据
            ECanbMboxes.MBOX4.MDH.byte.BYTE7 = Amp_Word;                  //数据
            break;
        case 5:
            ECanbMboxes.MBOX5.MDL.word.HI_WORD = CAN_Local_ID;            //来源ID
            ECanbMboxes.MBOX5.MDL.word.LOW_WORD = CAN_Set_CMD;            //命令
            ECanbMboxes.MBOX5.MDH.byte.BYTE4 = 0X00;                      //数据
            ECanbMboxes.MBOX5.MDH.byte.BYTE5 = 0X00;                      //数据
            ECanbMboxes.MBOX5.MDH.byte.BYTE6 = Freq_Word;                 //数据
            ECanbMboxes.MBOX5.MDH.byte.BYTE7 = Amp_Word;                  //数据
            break;
        case 6:
            ECanbMboxes.MBOX6.MDL.word.HI_WORD = CAN_Local_ID;            //来源ID
            ECanbMboxes.MBOX6.MDL.word.LOW_WORD = CAN_Set_CMD;            //命令
            ECanbMboxes.MBOX6.MDH.byte.BYTE4 = 0X00;                      //数据
            ECanbMboxes.MBOX6.MDH.byte.BYTE5 = 0X00;                      //数据
            ECanbMboxes.MBOX6.MDH.byte.BYTE6 = Freq_Word;                 //数据
            ECanbMboxes.MBOX6.MDH.byte.BYTE7 = Amp_Word;                  //数据
            break;
        case 7:
            ECanbMboxes.MBOX7.MDL.word.HI_WORD = CAN_Local_ID;            //来源ID
            ECanbMboxes.MBOX7.MDL.word.LOW_WORD = CAN_Set_CMD;            //命令
            ECanbMboxes.MBOX7.MDH.byte.BYTE4 = 0X00;                      //数据
            ECanbMboxes.MBOX7.MDH.byte.BYTE5 = 0X00;                      //数据
            ECanbMboxes.MBOX7.MDH.byte.BYTE6 = Freq_Word;                 //数据
            ECanbMboxes.MBOX7.MDH.byte.BYTE7 = Amp_Word;                  //数据
            break;
        case 8:
            ECanbMboxes.MBOX8.MDL.word.HI_WORD = CAN_Local_ID;            //来源ID
            ECanbMboxes.MBOX8.MDL.word.LOW_WORD = CAN_Set_CMD;            //命令
            ECanbMboxes.MBOX8.MDH.byte.BYTE4 = 0X00;                      //数据
            ECanbMboxes.MBOX8.MDH.byte.BYTE5 = 0X00;                      //数据
            ECanbMboxes.MBOX8.MDH.byte.BYTE6 = Freq_Word;                 //数据
            ECanbMboxes.MBOX8.MDH.byte.BYTE7 = Amp_Word;                  //数据
            break;
        default:
            break;
    }
        ECanbRegs.CANTRS.all = 0x00000001<<ID_Number;
        //不等待发送完成
//        while(ECanbRegs.CANTA.all == 0);
//        ECanbRegs.CANTA.all = 0x000000FFFF;
}



//---------------------------------------------------------------------------
// InitECan:
//---------------------------------------------------------------------------
// This function initializes to a known state.
//
void CAN_Init(void)
{
    struct ECAN_REGS EcanbShadow;

    /* Enable internal pull-up for the selected CAN pins */
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    EALLOW;

    //
    // Enable internal pull-up for the selected CAN pins
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    //GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;    //Enable pull-up for GPIO8 (CANTXB)
    //GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;   //Enable pull-up for GPIO12(CANTXB)
    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;   //Enable pull-up for GPIO16(CANTXB)
    //GpioCtrlRegs.GPAPUD.bit.GPIO20 = 0;   //Enable pull-up for GPIO20(CANTXB)

    //GpioCtrlRegs.GPAPUD.bit.GPIO10 = 0;   // Enable pull-up for GPIO10(CANRXB)
    //GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;  // Enable pull-up for GPIO13(CANRXB)
    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;  // Enable pull-up for GPIO17(CANRXB)
    //GpioCtrlRegs.GPAPUD.bit.GPIO21 = 0;  // Enable pull-up for GPIO21(CANRXB)

    //
    // Set qualification for selected CAN pins to asynch only
    // Inputs are synchronized to SYSCLKOUT by default.
    // This will select asynch (no qualification) for the selected pins.
    // Comment out other unwanted lines.
    //
    //GpioCtrlRegs.GPAQSEL1.bit.GPIO10 = 3; // Asynch qual for GPIO10 (CANRXB)
    //GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3; // Asynch qual for GPIO13 (CANRXB)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3; // Asynch qual for GPIO17 (CANRXB)
    //GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 3; // Asynch qual for GPIO21 (CANRXB)

    //
    // Configure eCAN-B pins using GPIO regs
    // This specifies which of the possible GPIO pins will be eCAN functional
    // pins.
    //
    //GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 2;   // Configure GPIO8 for CANTXB
    //GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 2;  // Configure GPIO12 for CANTXB
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 2;  // Configure GPIO16 for CANTXB
    //GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 3;  // Configure GPIO20 for CANTXB

    //GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 2;  // Configure GPIO10 for CANRXB
    //GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 2;  // Configure GPIO13 for CANRXB
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 2;  // Configure GPIO17 for CANRXB
    //GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 3;  // Configure GPIO21 for CANRXB

    EDIS;

    EALLOW;
    //配置eCAN的RX和TX分别为eCAN的接收和发送引脚
    EcanbShadow.CANTIOC.all = ECanbRegs.CANTIOC.all;
    EcanbShadow.CANTIOC.bit.TXFUNC = 1;
    ECanbRegs.CANTIOC.all = EcanbShadow.CANTIOC.all;

    EcanbShadow.CANRIOC.all = ECanbRegs.CANRIOC.all;
    EcanbShadow.CANRIOC.bit.RXFUNC = 1;
    ECanbRegs.CANRIOC.all = EcanbShadow.CANRIOC.all;
    EDIS;

    EALLOW;
    EcanbShadow.CANMC.all = ECanbRegs.CANMC.all;
    EcanbShadow.CANMC.bit.STM = 0;              //工作在正常模式
    EcanbShadow.CANMC.bit.SCB = 1;              //工作在ecan模式
    EcanbShadow.CANMC.bit.ABO = 1 ;             // 识别到总线自动恢复
    EcanbShadow.CANMC.bit.SUSP = 1 ;            //挂起模式下继续运行
    ECanbRegs.CANMC.all = EcanbShadow.CANMC.all;
    EDIS;

    //初始化所有主设备控制区域为0，MCF所有的位都初始化为0
    ECanbMboxes.MBOX0.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX1.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX2.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX3.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX4.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX5.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX6.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX7.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX8.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX9.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX10.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX11.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX12.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX13.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX14.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX15.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX16.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX17.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX18.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX19.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX20.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX21.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX22.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX23.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX24.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX25.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX26.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX27.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX28.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX29.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX30.MSGCTRL.all = 0x00000000;

    /* Clear all TAn bits */
    ECanbRegs.CANTA.all = 0xFFFFFFFF;

    /* Clear all RMPn bits */
    ECanbRegs.CANRMP.all = 0xFFFFFFFF;

    /* Clear all interrupt flag bits */
    ECanbRegs.CANGIF0.all = 0xFFFFFFFF;
    ECanbRegs.CANGIF1.all = 0xFFFFFFFF;

    /* Configure bit timing parameters */
    EALLOW;
    //ECanbRegs.CANMIM.all = 0xFFFFFFFF;        /*邮箱中断使能*/
    EcanbShadow.CANMC.all = ECanbRegs.CANMC.all;
    EcanbShadow.CANMC.bit.CCR = 1;
    ECanbRegs.CANMC.all = EcanbShadow.CANMC.all;
    EDIS;

    //CPU请求向CANBTC和CANGAM写配置信息，该位置1后必须等到CANED.CCE为1，才能
    //对CANBTC进行操作。
    do
    {
        EcanbShadow.CANES.all = ECanbRegs.CANES.all;
    }while(EcanbShadow.CANES.bit.CCE != 1);

    EALLOW;
    /*(BRPREG+1)=19 feeds a 15MHz CAN clock 250kbps*/
    EcanbShadow.CANBTC.bit.BRPREG = 19;
    /*150/10=15*/
    EcanbShadow.CANBTC.bit.TSEG2REG = 2;
    EcanbShadow.CANBTC.bit.TSEG1REG = 10;
    ECanbRegs.CANBTC.all = EcanbShadow.CANBTC.all;

    EcanbShadow.CANMC.all = ECanbRegs.CANMC.all;
    EcanbShadow.CANMC.bit.CCR = 0;
    ECanbRegs.CANMC.all = EcanbShadow.CANMC.all;
    EDIS;
    do
    {
     EcanbShadow.CANES.all = ECanbRegs.CANES.all;
    }while(EcanbShadow.CANES.bit.CCE != 0);     //等待CCE位清零

    //屏蔽所有邮箱
    ECanbRegs.CANME.all = 0;        //在写MSGID之前要完成该操作

    /*发送邮箱的ID号*/
    ECanbMboxes.MBOX0.MSGID.all = ((Uint32)(0x21))<<18;
    ECanbMboxes.MBOX1.MSGID.all = ((Uint32)(0x22))<<18;
    ECanbMboxes.MBOX2.MSGID.all = ((Uint32)(0x23))<<18;
    ECanbMboxes.MBOX3.MSGID.all = ((Uint32)(0x24))<<18;
    ECanbMboxes.MBOX4.MSGID.all = ((Uint32)(0x25))<<18;
    ECanbMboxes.MBOX5.MSGID.all = ((Uint32)(0x26))<<18;
    ECanbMboxes.MBOX6.MSGID.all = ((Uint32)(0x27))<<18;
    ECanbMboxes.MBOX7.MSGID.all = ((Uint32)(0x28))<<18;
    ECanbMboxes.MBOX8.MSGID.all = ((Uint32)(0x31))<<18;
    /*ECanbMboxes.MBOX1.MID.all = 0x9555AAA1;
    ECanbMboxes.MBOX2.MID.all = 0x9555AAA2;
    ECanbMboxes.MBOX3.MID.all = 0x9555AAA3;
    ECanbMboxes.MBOX4.MID.all = 0x9555AAA4;
    ECanbMboxes.MBOX5.MID.all = 0x9555AAA5;
    ECanbMboxes.MBOX6.MID.all = 0x9555AAA6;
    ECanbMboxes.MBOX7.MID.all = 0x9555AAA7;
    ECanbMboxes.MBOX8.MID.all = 0x9555AAA8;
    ECanbMboxes.MBOX9.MID.all = 0x9555AAA9;
    ECanbMboxes.MBOX10.MID.all = 0x9555AAAA;
    ECanbMboxes.MBOX11.MID.all = 0x9555AAAB;
    ECanbMboxes.MBOX12.MID.all = 0x9555AAAC;
    ECanbMboxes.MBOX13.MID.all = 0x9555AAAD;
    ECanbMboxes.MBOX14.MID.all = 0x9555AAAE;
    ECanbMboxes.MBOX15.MID.all = 0x9555AAAF;*/

    /*接收邮箱的ID号*/
//    ECanbMboxes.MBOX16.MSGID.all =  ((Uint32)(0x11))<<18;
    /*ECanbMboxes.MBOX17.MID.all = 0x9555AAA1;
    ECanbMboxes.MBOX18.MID.all = 0x9555AAA2;
    ECanbMboxes.MBOX19.MID.all = 0x9555AAA3;
    ECanbMboxes.MBOX20.MID.all = 0x9555AAA4;
    ECanbMboxes.MBOX21.MID.all = 0x9555AAA5;
    ECanbMboxes.MBOX22.MID.all = 0x9555AAA6;
    ECanbMboxes.MBOX23.MID.all = 0x9555AAA7;
    ECanbMboxes.MBOX24.MID.all = 0x9555AAA8;
    ECanbMboxes.MBOX25.MID.all = 0x9555AAA9;
    ECanbMboxes.MBOX26.MID.all = 0x9555AAAA;
    ECanbMboxes.MBOX27.MID.all = 0x9555AAAB;
    ECanbMboxes.MBOX28.MID.all = 0x9555AAAC;
    ECanbMboxes.MBOX29.MID.all = 0x9555AAAD;
    ECanbMboxes.MBOX30.MID.all = 0x9555AAAE;
    ECanbMboxes.MBOX31.MID.all = 0x9555AAAF;*/

    /*邮箱0为TX，16为RX*/
    EcanbShadow.CANMD.all = ECanbRegs.CANMD.all;
    EcanbShadow.CANMD.bit.MD0 =0;
    EcanbShadow.CANMD.bit.MD1 =0;
    EcanbShadow.CANMD.bit.MD2 =0;
    EcanbShadow.CANMD.bit.MD3 =0;
    EcanbShadow.CANMD.bit.MD4 =0;
    EcanbShadow.CANMD.bit.MD5 =0;
    EcanbShadow.CANMD.bit.MD6 =0;
    EcanbShadow.CANMD.bit.MD7 =0;
    EcanbShadow.CANMD.bit.MD8 =0;
//    EcanbShadow.CANMD.bit.MD16 =1;
    ECanbRegs.CANMD.all = EcanbShadow.CANMD.all;


    /*数据长度 8个BYTE*/
    ECanbMboxes.MBOX0.MSGCTRL.bit.DLC = 8;
    ECanbMboxes.MBOX1.MSGCTRL.bit.DLC = 8;
    ECanbMboxes.MBOX2.MSGCTRL.bit.DLC = 8;
    ECanbMboxes.MBOX3.MSGCTRL.bit.DLC = 8;
    ECanbMboxes.MBOX4.MSGCTRL.bit.DLC = 8;
    ECanbMboxes.MBOX5.MSGCTRL.bit.DLC = 8;
    ECanbMboxes.MBOX6.MSGCTRL.bit.DLC = 8;
    ECanbMboxes.MBOX7.MSGCTRL.bit.DLC = 8;
    ECanbMboxes.MBOX8.MSGCTRL.bit.DLC = 8;
    /*ECanbMboxes.MBOX1.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX2.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX3.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX4.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX5.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX6.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX7.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX8.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX9.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX10.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX11.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX12.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX13.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX14.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX15.MCF.bit.DLC = 8;*/
 //   ECanbMboxes.MBOX16.MSGCTRL.bit.DLC = 8;
    /*ECanbMboxes.MBOX17.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX18.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX19.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX20.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX21.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX22.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX23.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX24.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX25.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX26.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX27.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX28.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX29.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX30.MCF.bit.DLC = 8;
    ECanbMboxes.MBOX31.MCF.bit.DLC = 8;*/

    //设置发送优先级  2009.3.15 Add
    ECanbMboxes.MBOX0.MSGCTRL.bit.TPL = 0;
    ECanbMboxes.MBOX1.MSGCTRL.bit.TPL = 0;
    ECanbMboxes.MBOX2.MSGCTRL.bit.TPL = 0;
    ECanbMboxes.MBOX3.MSGCTRL.bit.TPL = 0;
    ECanbMboxes.MBOX4.MSGCTRL.bit.TPL = 0;
    ECanbMboxes.MBOX5.MSGCTRL.bit.TPL = 0;
    ECanbMboxes.MBOX6.MSGCTRL.bit.TPL = 0;
    ECanbMboxes.MBOX7.MSGCTRL.bit.TPL = 0;
    ECanbMboxes.MBOX8.MSGCTRL.bit.TPL = 0;


    /*没有远方应答帧被请求*/
    ECanbMboxes.MBOX0.MSGCTRL.bit.RTR = 0;
    /*ECanbMboxes.MBOX1.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX2.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX3.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX4.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX5.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX6.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX7.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX8.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX9.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX10.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX11.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX12.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX13.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX14.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX15.MCF.bit.RTR = 0;*/
//    ECanbMboxes.MBOX16.MSGCTRL.bit.RTR = 0;
    /*ECanbMboxes.MBOX17.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX18.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX19.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX20.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX21.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX22.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX23.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX24.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX25.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX26.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX27.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX28.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX29.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX30.MCF.bit.RTR = 0;
    ECanbMboxes.MBOX31.MCF.bit.RTR = 0;*/

    //向邮箱RAM区写数据
//    ECanbMboxes.MBOX0.MDL.all = 0x01234567;
//    ECanbMboxes.MBOX0.MDH.all = 0x89ABCDEF;

    /*ECanbMboxes.MBOX1.MDRL.all = 0x9555AAA1;
    ECanbMboxes.MBOX1.MDRH.all = 0x89ABCDEF;

    ECanbMboxes.MBOX2.MDRL.all = 0x9555AAA2;
    ECanbMboxes.MBOX2.MDRH.all = 0x89ABCDEF;

    ECanbMboxes.MBOX3.MDRL.all = 0x9555AAA3;
    ECanbMboxes.MBOX3.MDRH.all = 0x89ABCDEF;

    ECanbMboxes.MBOX4.MDRL.all = 0x9555AAA4;
    ECanbMboxes.MBOX4.MDRH.all = 0x89ABCDEF;

    ECanbMboxes.MBOX5.MDRL.all = 0x9555AAA5;
    ECanbMboxes.MBOX5.MDRH.all = 0x89ABCDEF;

    ECanbMboxes.MBOX6.MDRL.all = 0x9555AAA6;
    ECanbMboxes.MBOX6.MDRH.all = 0x89ABCDEF;

    ECanbMboxes.MBOX7.MDRL.all = 0x9555AAA7;
    ECanbMboxes.MBOX7.MDRH.all = 0x89ABCDEF;

    ECanbMboxes.MBOX8.MDRL.all = 0x9555AAA8;
    ECanbMboxes.MBOX8.MDRH.all = 0x89ABCDEF;

    ECanbMboxes.MBOX9.MDRL.all = 0x9555AAA9;
    ECanbMboxes.MBOX9.MDRH.all = 0x89ABCDEF;

    ECanbMboxes.MBOX10.MDRL.all = 0x9555AAAA;
    ECanbMboxes.MBOX10.MDRH.all = 0x89ABCDEF;

    ECanbMboxes.MBOX11.MDRL.all = 0x9555AAAB;
    ECanbMboxes.MBOX11.MDRH.all = 0x89ABCDEF;

    ECanbMboxes.MBOX12.MDRL.all = 0x9555AAAC;
    ECanbMboxes.MBOX12.MDRH.all = 0x89ABCDEF;

    ECanbMboxes.MBOX13.MDRL.all = 0x9555AAAD;
    ECanbMboxes.MBOX13.MDRH.all = 0x89ABCDEF;

    ECanbMboxes.MBOX14.MDRL.all = 0x9555AAAE;
    ECanbMboxes.MBOX14.MDRH.all = 0x89ABCDEF;

    ECanbMboxes.MBOX15.MDRL.all = 0x9555AAAF;
    ECanbMboxes.MBOX15.MDRH.all = 0x89ABCDEF;*/


    //邮箱使能Mailbox0

    EcanbShadow.CANME.all = ECanbRegs.CANME.all;
    EcanbShadow.CANME.bit.ME0 =1;
    EcanbShadow.CANME.bit.ME1 =1;
    EcanbShadow.CANME.bit.ME2 =1;
    EcanbShadow.CANME.bit.ME3 =1;
    EcanbShadow.CANME.bit.ME4 =1;
    EcanbShadow.CANME.bit.ME5 =1;
    EcanbShadow.CANME.bit.ME6 =1;
    EcanbShadow.CANME.bit.ME7 =1;
    EcanbShadow.CANME.bit.ME8 =1;
  //  EcanbShadow.CANME.bit.ME16 =1;
    ECanbRegs.CANME.all = EcanbShadow.CANME.all;

//    //邮箱中断使能
//    EALLOW;
//    ECanbRegs.CANMIM.all = 0x00010000;   //开启接收邮箱的中断
//    //邮箱中断将产生在ECAN0INT
//    ECanbRegs.CANMIL.all = 0;
//    ECanbRegs.CANGIF0.all = 0xFFFFFFFF;
//    //ECAN0INT中断请求线被使能
//    ECanbRegs.CANGIM.bit.I0EN = 1;
//    EDIS;


}

