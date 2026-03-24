/*
 * F28335_FLASH.c
 *
 *  Created on: 2023年6月11日
 *      Author: xikai
 */

#include "F28335_FLASH.h"

FLASH_ST FlashStatus;

//F28335内部Flashc初始化，用于API库读写
void F28335Flash_Init()
{
     //确保不工作在limp模式
     if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 1)
       {
             //确保主频150MHz
           if (SysCtrlRegs.PLLCR.bit.DIV != PLLCR_VALUE)
           {
              EALLOW;
              // Before setting PLLCR turn off missing clock detect
              SysCtrlRegs.PLLSTS.bit.MCLKOFF = 1;
              SysCtrlRegs.PLLCR.bit.DIV = PLLCR_VALUE;
              EDIS;
              // Wait for PLL to lock.
              // The watchdog should be disabled before this loop, or fed within
              // the loop.
              //确保关闭看门狗
              EALLOW;
              SysCtrlRegs.WDCR= 0x0068;
              EDIS;
              // Wait for the PLL lock bit to be set.
              while(SysCtrlRegs.PLLSTS.bit.PLLLOCKS != 1) { }
              EALLOW;
              SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;
              EDIS;
           }
       }
       // If the PLL is in limp mode, shut the system down
       else
       {
          asm("        ESTOP0");
       }

  // Initalize Flash_CPUScaleFactor.
     Flash_CPUScaleFactor = SCALE_FACTOR;
 // Initalize Flash_CallbackPtr.
     Flash_CallbackPtr = NULL;
}

//F28335写入数据
void F28335Flash_Program(unsigned int *DataBuffer,unsigned long datalength)
{
     Uint16 Program_Status;       // Pointer to a location in flash
     Uint16 *Flash_ptr;
     Flash_ptr = (Uint16 *)F28335_Write_Addr;
     // Call the program API function
     Program_Status = Flash_Program(Flash_ptr,DataBuffer,datalength,&FlashStatus);
     if(Program_Status != STATUS_SUCCESS)
    {
         asm("        ESTOP0");
    }
}

//F28335擦除扇区
void F28335Flash_Eraser()
{
     Uint16  Erase_Status;
     Erase_Status = Flash_Erase(SECTORB,&FlashStatus);     //擦除B扇区
       if( Erase_Status != STATUS_SUCCESS)
       {
           asm("    ESTOP0");
       }
}

//从内部FLASH读取数据
void F28335Flash_Read(unsigned int *DataBuffer,unsigned long datalength)
{
    Uint16 *Flash_star;
    Uint16 *Flash_end;

    Flash_star = (Uint16 *)F28335_Write_Addr;
    Flash_end = (Uint16 *)(F28335_Write_Addr+datalength);

    MemCopy(Flash_star,Flash_end,DataBuffer);
}



