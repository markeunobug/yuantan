/*
 * EXT_FLASH_Config.c
 *
 *  Created on: 2023年6月11日
 *      Author: xikai
 */

#include "EXT_FLASH_Config.h"




#define BadBlockTableLength 90
static unsigned int BadBlockTable[90]={0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF};
static unsigned int BadBlockTableCnt = 0;



static void Set_FlashWE_OUT()
{
    EALLOW;
    Flash_WE_DIR=OUT;
    EDIS;
}


static void Set_FlashALE_OUT()
{
    EALLOW;
    Flash_ALE_DIR=OUT;
    EDIS;
}

static void Set_FlashCLE_OUT()
{
    EALLOW;
    Flash_CLE_DIR=OUT;
    EDIS;
}

static void Set_FlashRB_IN()
{
    EALLOW;
    Flash_RB_DIR=IN;
    EDIS;
}

static void Set_FlashRE_OUT()
{
    EALLOW;
    Flash_RE_DIR=OUT;
    EDIS;
}

static void Set_FlashCE_OUT()
{
    EALLOW;
    Flash_CE_DIR=OUT;
    EDIS;
}

static void Flash_DataPort_OUT()
{
     EALLOW;
     GpioCtrlRegs.GPBDIR.bit.GPIO50 = OUT;      // FLASH_DATA0(GPIO50) is output
     GpioCtrlRegs.GPBDIR.bit.GPIO51 = OUT;      // FLASH_DATA1(GPIO51) is output
     GpioCtrlRegs.GPBDIR.bit.GPIO52 = OUT;      // FLASH_DATA2(GPIO52) is output
     GpioCtrlRegs.GPBDIR.bit.GPIO53 = OUT;      // FLASH_DATA3(GPIO53) is output
     GpioCtrlRegs.GPBDIR.bit.GPIO54 = OUT;       // FLASH_DATA4(GPIO54) is output
     GpioCtrlRegs.GPBDIR.bit.GPIO55 = OUT;       // FLASH_DATA5(GPIO55) is output
     GpioCtrlRegs.GPBDIR.bit.GPIO56 = OUT;       // FLASH_DATA6(GPIO56) is output
     GpioCtrlRegs.GPBDIR.bit.GPIO57 = OUT;       // FLASH_DATA7(GPIO57) is output
    EDIS;
}

static void Flash_DataPort_IN()
{
     EALLOW;
     GpioCtrlRegs.GPBDIR.bit.GPIO50 = IN;      // FLASH_DATA0(GPIO50) is input
     GpioCtrlRegs.GPBDIR.bit.GPIO51 = IN;      // FLASH_DATA1(GPIO51) is input
     GpioCtrlRegs.GPBDIR.bit.GPIO52 = IN;      // FLASH_DATA2(GPIO52) is input
     GpioCtrlRegs.GPBDIR.bit.GPIO53 = IN;      // FLASH_DATA3(GPIO53) is input
     GpioCtrlRegs.GPBDIR.bit.GPIO54 = IN;       // FLASH_DATA4(GPIO54) is input
     GpioCtrlRegs.GPBDIR.bit.GPIO55 = IN;       // FLASH_DATA5(GPIO55) is input
     GpioCtrlRegs.GPBDIR.bit.GPIO56 = IN;       // FLASH_DATA6(GPIO56) is input
     GpioCtrlRegs.GPBDIR.bit.GPIO57 = IN;       // FLASH_DATA7(GPIO57) is input
     EDIS;
}

//nand flash判忙函数
static void Flash_CheckBusy(void)
{
    unsigned int i=200,j=65535;
    while( (i>10) && (Flash_RB_DAT==0))//此处防阻塞，150M下，最多0.1s跳出//
    {
        j--;
        if(j<3)
        {
            i--;
            j = 65535;
        }
    }
}


//设置数据接口为输出一个byte
static void Flash_WriteDataPort(unsigned char Value)
{
    Uint32 Temp = Value;
    GpioDataRegs.GPBSET.all = ( Temp&(0x000000FF) )<<18;
    GpioDataRegs.GPBCLEAR.all = ( (~Temp) &(0x000000FF) )<<18;
}

//从数据接口读入一个byte
static unsigned char Flash_ReadDataPort()
{
    unsigned char ReadValue;
    ReadValue = (GpioDataRegs.GPBDAT.all >> 18)& 0x000000FF;
    return ReadValue;
}

//写入nand flash一个byte
static void Flash_WriteOneByte(unsigned char Dat)
{
    Flash_WriteDataPort(Dat);
    Flash_WE_CLR=1;
    asm(" NOP");
    Flash_WE_SET=1;
    asm(" NOP");
}

//从nand flash读入一个byte
static unsigned char Flash_ReadOneByte(void)
{
    unsigned char Temp;
    Flash_RE_CLR=1;
    asm(" NOP");asm(" NOP");
    Temp = Flash_ReadDataPort();
    Flash_RE_SET=1;
    return Temp;
}

//写入nand flash一个地址，5个byte
static void Flash_SendAddr(unsigned int BlockAddr,unsigned char PageAddr,unsigned int ColumnAddr)
{
    Flash_CheckBusy();
    Flash_ALE_SET=1;
    Flash_WriteOneByte( (ColumnAddr) & 0xff );
    Flash_WriteOneByte( (ColumnAddr>>8) & 0x0f );
    Flash_WriteOneByte( ((PageAddr) & 0x7f) | ((BlockAddr<<7) & 0x80));
    Flash_WriteOneByte( (BlockAddr>>1) & 0xff );
    Flash_WriteOneByte( (BlockAddr>>9) & 0x07 );
    Flash_ALE_CLR=1;
}

//nand flash写入一页的数据
static void Flash_PageWriteOperation(unsigned int BlockAddr,unsigned char PageAddr,unsigned int ColumnAddr,unsigned int Length,unsigned char *DataArray)
{
    unsigned int cnt;
    Flash_CE_CLR=1;
    Flash_CheckBusy();
    Flash_CLE_SET=1;
    Flash_WriteOneByte(0x80);
    Flash_CLE_CLR=1;
    Flash_SendAddr(BlockAddr,PageAddr,ColumnAddr);
    for(cnt=0;cnt<Length;cnt++)
    {
        Flash_WriteOneByte(DataArray[cnt]);
    }
    Flash_CLE_SET=1;
    Flash_WriteOneByte(0x10);
    Flash_CLE_CLR=1;
    Flash_CE_SET=1;
}

// nand flash读出一页的数据
static void Flash_PageReadOperation(unsigned int BlockAddr,unsigned char PageAddr,unsigned int ColumnAddr,unsigned int Length)
{
    unsigned int cnt;
    Flash_CE_CLR=1;
    Flash_CheckBusy();
    Flash_CLE_SET=1;
    Flash_WriteOneByte(0x00);
    Flash_CLE_CLR=1;
    Flash_SendAddr(BlockAddr,PageAddr,ColumnAddr);
    Flash_CLE_SET=1;
    Flash_WriteOneByte(0x30);
    Flash_CLE_CLR=1;
    Flash_CheckBusy();
    Flash_DataPort_IN();
    for(cnt=0;cnt<Length;cnt++)
    {
        PageBuffer[cnt] = Flash_ReadOneByte();
    }
    Flash_DataPort_OUT();
    Flash_CE_SET=1;
}

//nand flash读状态
static unsigned char Flash_ReadStatus(void)
{
    unsigned char StatusDat=0xff;
    unsigned char SucceedFlag=0;
    Flash_CheckBusy();
    Flash_CE_CLR=1;
    Flash_CLE_SET=1;
    Flash_WriteOneByte(0x70);
    Flash_CLE_CLR=1;
    Flash_DataPort_IN();
    StatusDat = Flash_ReadOneByte();
    if((StatusDat&0x01) == 0)
    {
        SucceedFlag = 1;
    }
    else
    {
        SucceedFlag = 0;
    }
    Flash_DataPort_OUT();
    Flash_CE_SET=1;
    return SucceedFlag;
}

//nand flash块地址重新定位函数。坏块管理的隔离层
static unsigned int Flash_BlockAddrMap(unsigned int AddrDef)
{
    unsigned int AddrReal=0;
    unsigned int Cnti=0;
    unsigned int AddrTemp=0;
    if(AddrDef < BadBlockTable[0])
    {
        AddrReal = AddrDef;
    }
    else if((BadBlockTableCnt > 0) && (AddrDef > BadBlockTable[BadBlockTableCnt - 1]))
    {
        AddrReal = AddrDef + BadBlockTableCnt;
    }
    else
    {
        while(AddrDef != AddrTemp)
        {
            AddrTemp ++;
            AddrReal ++;
            if(AddrReal >= BadBlockTable[0] && AddrReal < BadBlockTable[BadBlockTableCnt - 1])
            {
                for(Cnti=0;Cnti<BadBlockTableLength;Cnti++)
                {
                    if(AddrReal == BadBlockTable[Cnti])
                    {
                        AddrReal ++;
                    }
                }
            }

        }
    }
    return AddrReal;
}

//nand flash操作GUI函数。
static void Flash_WriteBadBlockTable(void)
{
    unsigned char ReadDat[192]={0};
    unsigned char ReadCnt=0;
    for(ReadCnt=0;ReadCnt<90;ReadCnt++)
    {
        ReadDat[2*ReadCnt] = (BadBlockTable[ReadCnt]>>8)&0xff;
        ReadDat[2*ReadCnt+1] = (BadBlockTable[ReadCnt])&0xff;
    }
    ReadDat[191] = BadBlockTableCnt;
    Flash_PageWriteOperation(1,0,0,192,ReadDat);
    Flash_PageWriteOperation(2,0,0,192,ReadDat);
    Flash_PageWriteOperation(3,0,0,192,ReadDat);
}


// nand flash向坏块表插入新的坏块标记
static void Flash_UpdateBadBlockTable(unsigned int BadAddr)
{
    unsigned int cnti=0;
    unsigned int savei=0;
    unsigned char sameflag = 0;

    for(cnti=0;cnti<BadBlockTableLength;cnti++)
    {
        if(BadAddr == BadBlockTable[cnti])
        {
            sameflag = 1;
            break;
        }
    }
    if(!sameflag)
    {
        if(BadAddr < BadBlockTable[0])
            savei = 0;
        else if((BadBlockTableCnt > 0) && (BadAddr > BadBlockTable[BadBlockTableCnt - 1]))
            savei = BadBlockTableCnt;
        else
        {
            for(cnti=0;cnti<BadBlockTableLength;cnti++)
            {
                if(BadAddr > BadBlockTable[cnti] && BadAddr < BadBlockTable[cnti+1])
                {
                    savei = cnti + 1;
                    break;
                }
            }
        }

        for(cnti=BadBlockTableLength-1;cnti>savei;cnti--)
        {
            BadBlockTable[cnti] = BadBlockTable[cnti-1];
        }
        BadBlockTable[savei] = BadAddr;
        BadBlockTableCnt ++;
        Flash_WriteBadBlockTable();
    }
}

// nand flash块擦除函数
static void Flash_EraseOperation(unsigned int BlockAddr)
{
    Flash_CE_CLR=1;
    Flash_CheckBusy();
    Flash_CLE_SET=1;
    Flash_WriteOneByte(0x60);
    Flash_CLE_CLR=1;
    Flash_ALE_SET=1;
    Flash_WriteOneByte( (BlockAddr<<7) & 0x80 );
    Flash_WriteOneByte( (BlockAddr>>1) & 0xff );
    Flash_WriteOneByte( (BlockAddr>>9) & 0x07 );
    Flash_ALE_CLR=1;
    Flash_CLE_SET=1;
    Flash_WriteOneByte(0xd0);
    Flash_CLE_CLR=1;
    Flash_CE_SET=1;
}

//nand flash操作GUI函数。块擦除
static void Flash_EraseBlockGUI(unsigned int BlockAddr)
{
    unsigned int RealBlockAddr;
    RealBlockAddr = Flash_BlockAddrMap(BlockAddr);
    Flash_EraseOperation(RealBlockAddr);
    if(!Flash_ReadStatus())
    {
        Flash_UpdateBadBlockTable(RealBlockAddr);
    }
}

//nand flash操作GUI函数。从一个地址读出一页的数据。
void Flash_ReadPageGUI(unsigned int BlockAddr,unsigned char PageAddr,unsigned int ColumnAddr,unsigned int Length)
{
    unsigned int RealBlockAddr;
    RealBlockAddr = Flash_BlockAddrMap(BlockAddr);
    Flash_PageReadOperation(RealBlockAddr,PageAddr,ColumnAddr,Length);
}


void Flash_ResetOperation(void)
{
    Flash_CE_CLR=1;

    Flash_CheckBusy();

    Flash_CLE_SET=1;
    Flash_WriteOneByte(0xff);
    Flash_CLE_CLR=1;

    Flash_CE_SET=1;
}


static void Flash_PageWriteLOGdata(unsigned int BlockAddr,unsigned char PageAddr,unsigned int ColumnAddr,unsigned int Length)
{
    unsigned int cnt;
    Flash_CE_CLR=1;
    Flash_CheckBusy();
    Flash_CLE_SET=1;
    Flash_WriteOneByte(0x80);
    Flash_CLE_CLR=1;
    Flash_SendAddr(BlockAddr,PageAddr,ColumnAddr);
    for(cnt=0;cnt<Length;cnt++)
    {
        Flash_WriteOneByte(PageBuffer[cnt]);
    }
    Flash_CLE_SET=1;
    Flash_WriteOneByte(0x10);
    Flash_CLE_CLR=1;
    Flash_CE_SET=1;
}

void Flash_WritePageGUI_LOGdata(unsigned int BlockAddr,unsigned char PageAddr,unsigned int ColumnAddr,unsigned int Length)
{
    unsigned int RealBlockAddr;
    RealBlockAddr = Flash_BlockAddrMap(BlockAddr);
    Flash_PageWriteLOGdata(RealBlockAddr,PageAddr,ColumnAddr,Length);
    if(!Flash_ReadStatus())
    {
        Flash_UpdateBadBlockTable(RealBlockAddr);
        RealBlockAddr = Flash_BlockAddrMap(BlockAddr);
        Flash_PageWriteLOGdata(RealBlockAddr,PageAddr,ColumnAddr,Length);
    }
}

void EXT_Flash_PortInit()
{
    Flash_DataPort_OUT();//8个io口方向输出
    Set_FlashCLE_OUT();//CLE方向输出
    Set_FlashALE_OUT();//ALE方向输出
    Set_FlashCE_OUT();//CE方向输出
    Set_FlashRE_OUT();//RE方向输出
    Set_FlashWE_OUT();//WE方向输出
    Set_FlashRB_IN();//READY&BUSY方向输入

    Flash_CLE_CLR=1;//CLE输出0
    Flash_ALE_CLR=1;//ALE输出0
    Flash_CE_SET=1;//CE输出0，未选中
    Flash_RE_SET=1;//RE读拉高，避免误触发上升沿
    Flash_WE_SET=1;//WE读拉高，避免误触发上升沿
}


void Flash_EraseAll(void)
{
    unsigned int tempBlockAddr;
    for(tempBlockAddr=BlockStartAddr;tempBlockAddr < BlockSettingsAddr;tempBlockAddr++)
    {
        Flash_EraseBlockGUI(tempBlockAddr);
    }
}



void FLASH_SaveOneFrame(void)
{
    Flash_ResetOperation();
    Flash_WritePageGUI_LOGdata(Write_BlockAddrCnt,Write_PageAddrCnt,0,Frame_Length[state_freq]);
    Write_PageAddrCnt++;
    if(Write_PageAddrCnt == 128)
    {
        Write_PageAddrCnt = 0;
        Write_BlockAddrCnt ++;
        if(Write_BlockAddrCnt == BlockSettingsAddr)
        {
            Write_BlockAddrCnt = BlockStartAddr;
        }
    }
    asm(" NOP");
}


void  FlashReadOneFrame()    //读取一帧数据
{
    unsigned int cnti;
//    Read_BlockAddrCnt=BlockStartAddr;
    for(cnti=0;cnti<MAX_Buffer_LEN;cnti++)
        PageBuffer[cnti]=0xff;

    Flash_ResetOperation();
    Flash_ReadPageGUI(Read_BlockAddrCnt,Read_PageAddrCnt,0,MAX_Buffer_LEN);
    Read_PageAddrCnt++;
}

void FLASH_SaveSettings(void)
{
    //擦除内容
    Flash_EraseBlockGUI(BlockSettingsAddr);
    //将数据存放于PageBuffer
    PageBuffer[0] = state_main;         //工作状态
    PageBuffer[1] = SamplingFlag;       //是否工作
    PageBuffer[2] = The_End_Time[0];
    PageBuffer[3] = The_End_Time[1];
    PageBuffer[4] = The_End_Time[2];
    PageBuffer[5] = The_End_Time[3];
    PageBuffer[6] = The_End_Time[4];
    PageBuffer[7] = The_End_Time[5];
    PageBuffer[8] = The_End_Time[6];
    PageBuffer[9] = CMD_SetAmp[0];
    PageBuffer[10] = CMD_SetAmp[1];
    PageBuffer[11] = CMD_SetAmp[2];
    PageBuffer[12] = CMD_SetAmp[3];
    PageBuffer[13] = CMD_SetAmp[4];
    PageBuffer[14] = Angle_Diff;
    PageBuffer[15] = DelayTimeFinishFlag;

    //写入数据
    Flash_WritePageGUI_LOGdata(BlockSettingsAddr,PageSettingsAddr,0,16);
}


void FLASH_ReadSettings(void)
{
    //读出数据
    Flash_ReadPageGUI(BlockSettingsAddr, PageSettingsAddr, 0, 16);
    //将数据存放于PageBuffer
    state_main=PageBuffer[0];         //工作状态
    SamplingFlag=PageBuffer[1];       //是否工作
    The_End_Time[0]=PageBuffer[2];
    The_End_Time[1]=PageBuffer[3];
    The_End_Time[2]=PageBuffer[4];
    The_End_Time[3]=PageBuffer[5];
    The_End_Time[4]=PageBuffer[6];
    The_End_Time[5]=PageBuffer[7];
    The_End_Time[6]=PageBuffer[8];

    CMD_SetAmp[0]=PageBuffer[9];
    CMD_SetAmp[1]=PageBuffer[10];
    CMD_SetAmp[2]=PageBuffer[11];
    CMD_SetAmp[3]=PageBuffer[12];
    CMD_SetAmp[4]=PageBuffer[13];
    Angle_Diff=PageBuffer[14];
    DelayTimeFinishFlag=PageBuffer[15];
}

void Flash_Read_SamplePoint(void)
{
    unsigned int BlockAddr_cnti=BlockStartAddr,PageAddr_cnti=0;
    for(PageAddr_cnti=0;;)
    {
        Flash_ResetOperation();
        Flash_ReadPageGUI(BlockAddr_cnti,PageAddr_cnti,0,10);
        if( (PageBuffer[0]==0x00) && (PageBuffer[1]==0x16) )
        {
            PageAddr_cnti++;
            if(PageAddr_cnti==128)
            {
                BlockAddr_cnti++;
                PageAddr_cnti=0;
            }
        }
        else
        {
            break;
        }
    }
    SamplePoint_Log = (long)(BlockAddr_cnti-BlockStartAddr)*128+PageAddr_cnti;//注意强制类型转换长度
    Write_BlockAddrCnt = BlockAddr_cnti;
    Write_PageAddrCnt = PageAddr_cnti;
}

















