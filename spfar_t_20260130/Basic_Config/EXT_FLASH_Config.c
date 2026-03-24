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
    // 加足够 NOP：目标 40~100 ns 以上
    asm(" NOP"); asm(" NOP"); asm(" NOP"); asm(" NOP");
    asm(" NOP"); asm(" NOP"); asm(" NOP"); asm(" NOP");  // 8 个 ≈ 53 ns
    asm(" NOP"); asm(" NOP"); asm(" NOP"); asm(" NOP");  // 再加 8 个 ≈ 106 ns 总
    asm(" NOP"); asm(" NOP"); asm(" NOP"); asm(" NOP");

    Flash_WE_SET=1;
    asm(" NOP");
    // 高电平也加一点
    asm(" NOP"); asm(" NOP"); asm(" NOP"); asm(" NOP");
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
    // 检查这次写是否成功
    Flash_PageWriteLOGdata(RealBlockAddr,PageAddr,ColumnAddr,Length);
    if(!Flash_ReadStatus())// 如果失败了
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
    for(tempBlockAddr=BlockStartAddr;tempBlockAddr < 3800;tempBlockAddr++)
    {
        Flash_EraseBlockGUI(tempBlockAddr);
    }
}


//存储一帧数据
void FLASH_SaveOneFrame(void)
{
    Flash_ResetOperation();
    Flash_WritePageGUI_LOGdata(Write_BlockAddrCnt,Write_PageAddrCnt,0,Frame_Length_Spfar);
        // Flash_WritePageGUI_LOGdata(Write_BlockAddrCnt,Write_PageAddrCnt,0,Frame_Length[state_freq]);
    Write_PageAddrCnt++;
    if(Write_PageAddrCnt == 128)
    {
        Write_PageAddrCnt = 0;
        Write_BlockAddrCnt ++;
        if(Write_BlockAddrCnt == 3800)
        {
            Write_BlockAddrCnt = BlockStartAddr;
        }
    }
    asm(" NOP");
}


void  FlashReadOneFrame()    //读取一帧数据
{
    unsigned int cnti;

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
    CMD_SetAmp[5]=PageBuffer[13];//2M频率
    Angle_Diff=PageBuffer[14];
    DelayTimeFinishFlag=PageBuffer[15];
}

void Flash_Read_SamplePoint(void)
{
    unsigned int BlockAddr_cnti=0,PageAddr_cnti=0;
    for(PageAddr_cnti=0;;)
    {
        Flash_ResetOperation();
        Flash_ReadPageGUI(BlockAddr_cnti,PageAddr_cnti,0,10);
        //读出的block数据开头不是，即未存入数据
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
    SamplePoint_Log = (long)BlockAddr_cnti*128+PageAddr_cnti;//注意强制类型转换长度
    Write_BlockAddrCnt = BlockAddr_cnti;
    Write_PageAddrCnt = PageAddr_cnti;
}







//以下为测试代码
// 在 main() 或某个测试函数中调用这个测试
/*void Test_NAND_ReadID(void)
{
    unsigned char id[8] = {0};
    int i;

    // 初始状态，确保控制信号安全
    Flash_CE_SET  = 1;   // 先不选中
    Flash_RE_SET  = 1;
    Flash_WE_SET  = 1;
    Flash_CLE_CLR = 1;
    Flash_ALE_CLR = 1;

    // 数据总线先设为输出（写命令/地址时需要）
    Flash_DataPort_OUT();

    // 选中芯片
    Flash_CE_CLR = 1;          // CE = 0
    DELAY_US(2);               // 稍微稳定一下（可换成几个 NOP）

    // 发送 Read ID 命令： 0x90
    Flash_CLE_SET = 1;
    Flash_WriteOneByte(0x90);
    Flash_CLE_CLR = 1;

    // 发送地址周期：通常为 0x00
    Flash_ALE_SET = 1;
    Flash_WriteOneByte(0x00);
    Flash_ALE_CLR = 1;

    // 切换到输入，准备读取 ID
    Flash_DataPort_IN();

    // 连续读取 5~8 个字节 ID（大部分芯片前 2~5 个字节最重要）
    for(i = 0; i < 8; i++)
    {
        Flash_RE_CLR = 1;
        asm(" NOP"); asm(" NOP"); asm(" NOP"); asm(" NOP");  // 保证 RE 低电平足够时间
        asm(" NOP"); asm(" NOP");
        id[i] = Flash_ReadOneByte();   // 使用你封装的读函数
        // 注意：Flash_ReadOneByte 内部已经包含了 RE 拉高动作
    }

    // 读完后恢复数据总线为输出（非常重要，避免后续误操作）
    Flash_DataPort_OUT();

    // 释放片选
    Flash_CE_SET = 1;

    // ──────────────── 以下是结果观察方式 ────────────────
    // 方法1：如果有串口，直接打印（需你自己实现 UART 输出）
    // printf("NAND ID: %02X %02X %02X %02X %02X %02X %02X %02X\n",
    //        id[0], id[1], id[2], id[3], id[4], id[5], id[6], id[7]);

    // 方法2：在 CCS 调试时直接看数组内容（推荐）
    // 打断点在此处，观察 id[0] ~ id[7] 的值

    // 方法3：如果有 LED 或其他输出方式，可以简单编码显示
    // 例如：如果 id[0] != 0xFF && id[0] != 0x00 则点亮某个灯表示成功
}


*/
/*
void Test_Save_Read_Settings(void)
{
    // 1. 准备明显的数据
    // memset(PageBuffer, 0xFF, MAX_Buffer_LEN);  // 先清成 FF
    PageBuffer[0] = 0xAA;                      // 明显标记
    PageBuffer[1] = 0x55;
    PageBuffer[2] = 0x5A;
    PageBuffer[3] = 0xA5;

    // 2. 先擦除（确保干净）
    Flash_EraseBlockGUI(BlockSettingsAddr);
    Flash_CheckBusy();   // 额外等一下

    // 3. 写 4 字节测试数据
    Flash_WritePageGUI_LOGdata(BlockSettingsAddr, PageSettingsAddr, 0, 4);

    // 4. 强制等编程完成
    // DELAY_US(1000);      // 或 Flash_CheckBusy() 多次
    Flash_CheckBusy();

    PageBuffer[0] = 0xFF;                      // 明显标记
    PageBuffer[1] = 0xFF;
    PageBuffer[2] = 0xFF;
    PageBuffer[3] = 0xFF;
    // 5. 立即读回
    Flash_ReadPageGUI(BlockSettingsAddr, PageSettingsAddr, 0, 16);

    // 6. 在调试器看 PageBuffer 前 4 个字节
    // 期望：0xAA 0x55 0x5A 0xA5
    // 如果还是 FF → 写入失败
}


// 测试 NAND Flash 所有控制引脚：WE, ALE, CLE, RB, RE, CE
// 功能：先全部输出低电平（0），保持 0.5s → 全部输出高电平（1），保持 0.5s → 循环或结束
// 使用方法：调用一次，用万用表测对应引脚电压变化
void Test_FlashControlPins_AllLow_High(void)
{
    EALLOW;

    // === 1. 配置所有引脚为 GPIO 模式 + 输出方向 ===

    // GPIO24 ~ GPIO27 (A组)
    GpioCtrlRegs.GPAMUX2.all &= ~0x000000FF;     // GPAMUX2 bit0~7 清零 → GPIO24~27 为 GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO24 = 1;         // 输出
    GpioCtrlRegs.GPADIR.bit.GPIO25 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO26 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO27 = 1;

    // GPIO32 ~ GPIO33 (B组)
    GpioCtrlRegs.GPBMUX1.all &= ~0x0000000F;     // GPBMUX1 bit0~3 清零 → GPIO32~33 为 GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;          // 输出
    GpioCtrlRegs.GPBDIR.bit.GPIO33 = 1;

    // 可选：禁用内部上拉（避免与外部电阻冲突）
    GpioCtrlRegs.GPAPUD.bit.GPIO24 = 1;         // 禁用上拉
    GpioCtrlRegs.GPAPUD.bit.GPIO25 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO26 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO27 = 1;
    GpioCtrlRegs.GPBPUD.bit.GPIO32 = 1;
    GpioCtrlRegs.GPBPUD.bit.GPIO33 = 1;

    GpioCtrlRegs.GPBDIR.all |= (0xFFUL << 18);  // bit18~25 对应 GPIO50~57

    Flash_WriteDataPort(0x00);          // 使用你已有的函数，输出 0
    // 或者直接写：
    // GpioDataRegs.GPBDAT.all &= ~(0xFFUL << 18);  // 清 bit18~25 → 全 0

    // 第二阶段：全部输出高电平 (0xFF)
    Flash_WriteDataPort(0xFF);          // 输出 255 → 全 1

    EDIS;

    // === 2. 第一阶段：全部拉低 ===
    Flash_WE_CLR = 1;    // WE# = 0
    Flash_ALE_CLR = 1;   // ALE = 0
    Flash_CLE_CLR = 1;   // CLE = 0
    Flash_RB_CLR = 1;    // RB 输出 0（注意：RB 正常是输入，这里强制输出测试）
    Flash_RE_CLR = 1;    // RE# = 0
    Flash_CE_CLR = 1;    // CE# = 0

    // DELAY_US(500000);    // 保持 0.5 秒（足够你用万用表测量）

    // === 3. 第二阶段：全部拉高 ===
    Flash_WE_SET = 1;    // WE# = 1
    Flash_ALE_SET = 1;   // ALE = 1
    Flash_CLE_SET = 1;   // CLE = 1
    Flash_RB_SET = 1;    // RB 输出 1
    Flash_RE_SET = 1;    // RE# = 1
    Flash_CE_SET = 1;    // CE# = 1

    // DELAY_US(500000);    // 再保持 0.5 秒

    // === 4. 可选：循环测试（方便观察反复跳变）===
    // 取消下面注释即可循环

    while(1)
    {
        // 低
        Flash_WE_CLR=1; Flash_ALE_CLR=1; Flash_CLE_CLR=1;
        Flash_RB_CLR=1; Flash_RE_CLR=1; Flash_CE_CLR=1;
        DELAY_US(300000);

        // 高
        Flash_WE_SET=1; Flash_ALE_SET=1; Flash_CLE_SET=1;
        Flash_RB_SET=1; Flash_RE_SET=1; Flash_CE_SET=1;
        DELAY_US(300000);
    }


    // 测试结束，恢复安全状态（所有控制信号高，芯片不选中）
    Flash_WE_SET=1; Flash_RE_SET=1; Flash_CE_SET=1;
    Flash_ALE_CLR=1; Flash_CLE_CLR=1;
//}

*/
