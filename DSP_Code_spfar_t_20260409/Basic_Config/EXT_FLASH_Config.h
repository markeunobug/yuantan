/*
 * EXT_FLASH_Config.h
 *
 *  Created on: 2023年6月11日
 *      Author: xikai
 */

#ifndef EXT_FLASH_CONFIG_H_
#define EXT_FLASH_CONFIG_H_

#include "Public_Var.h"

// WE (Write Enable) - GPIO24 (A组)
#define Flash_WE_SET   GpioDataRegs.GPASET.bit.GPIO24
#define Flash_WE_CLR   GpioDataRegs.GPACLEAR.bit.GPIO24
#define Flash_WE_DAT   GpioDataRegs.GPADAT.bit.GPIO24
#define Flash_WE_DIR   GpioCtrlRegs.GPADIR.bit.GPIO24

// ALE (Address Latch Enable) - GPIO25 (A组)
#define Flash_ALE_SET  GpioDataRegs.GPASET.bit.GPIO25
#define Flash_ALE_CLR  GpioDataRegs.GPACLEAR.bit.GPIO25
#define Flash_ALE_DAT  GpioDataRegs.GPADAT.bit.GPIO25
#define Flash_ALE_DIR  GpioCtrlRegs.GPADIR.bit.GPIO25

// CLE (Command Latch Enable) - GPIO26 (A组)
#define Flash_CLE_SET  GpioDataRegs.GPASET.bit.GPIO26
#define Flash_CLE_CLR  GpioDataRegs.GPACLEAR.bit.GPIO26
#define Flash_CLE_DAT  GpioDataRegs.GPADAT.bit.GPIO26
#define Flash_CLE_DIR  GpioCtrlRegs.GPADIR.bit.GPIO26

// RB (Ready/Busy) - GPIO27 (A组) 
#define Flash_RB_SET   GpioDataRegs.GPASET.bit.GPIO27     // 改成 GPASET
#define Flash_RB_CLR   GpioDataRegs.GPACLEAR.bit.GPIO27   // 改成 GPACLEAR
#define Flash_RB_DAT   GpioDataRegs.GPADAT.bit.GPIO27     // 改成 GPADAT
#define Flash_RB_DIR   GpioCtrlRegs.GPADIR.bit.GPIO27     // 改成 GPADIR

// RE (Read Enable) - GPIO32 (B组)
#define Flash_RE_SET   GpioDataRegs.GPBSET.bit.GPIO32
#define Flash_RE_CLR   GpioDataRegs.GPBCLEAR.bit.GPIO32
#define Flash_RE_DAT   GpioDataRegs.GPBDAT.bit.GPIO32
#define Flash_RE_DIR   GpioCtrlRegs.GPBDIR.bit.GPIO32

// CE (Chip Enable) - GPIO33 (B组)
#define Flash_CE_SET   GpioDataRegs.GPBSET.bit.GPIO33
#define Flash_CE_CLR   GpioDataRegs.GPBCLEAR.bit.GPIO33
#define Flash_CE_DAT   GpioDataRegs.GPBDAT.bit.GPIO33
#define Flash_CE_DIR   GpioCtrlRegs.GPBDIR.bit.GPIO33

// #define Flash_ALE_SET  GpioDataRegs.GPASET.bit.GPIO23
// #define Flash_ALE_CLR  GpioDataRegs.GPACLEAR.bit.GPIO23
// #define Flash_ALE_DAT  GpioDataRegs.GPADAT.bit.GPIO23
// #define Flash_ALE_DIR  GpioCtrlRegs.GPADIR.bit.GPIO23

// #define Flash_CLE_SET  GpioDataRegs.GPASET.bit.GPIO22
// #define Flash_CLE_CLR  GpioDataRegs.GPACLEAR.bit.GPIO22
// #define Flash_CLE_DAT  GpioDataRegs.GPADAT.bit.GPIO22
// #define Flash_CLE_DIR  GpioCtrlRegs.GPADIR.bit.GPIO22

// #define Flash_RB_SET   GpioDataRegs.GPBSET.bit.GPIO59
// #define Flash_RB_CLR   GpioDataRegs.GPBCLEAR.bit.GPIO59
// #define Flash_RB_DAT   GpioDataRegs.GPBDAT.bit.GPIO59
// #define Flash_RB_DIR   GpioCtrlRegs.GPBDIR.bit.GPIO59

// #define Flash_RE_SET   GpioDataRegs.GPBSET.bit.GPIO60
// #define Flash_RE_CLR   GpioDataRegs.GPBCLEAR.bit.GPIO60
// #define Flash_RE_DAT   GpioDataRegs.GPBDAT.bit.GPIO60
// #define Flash_RE_DIR   GpioCtrlRegs.GPBDIR.bit.GPIO60

// #define Flash_CE_SET   GpioDataRegs.GPBSET.bit.GPIO61
// #define Flash_CE_CLR   GpioDataRegs.GPBCLEAR.bit.GPIO61
// #define Flash_CE_DAT   GpioDataRegs.GPBDAT.bit.GPIO61
// #define Flash_CE_DIR   GpioCtrlRegs.GPBDIR.bit.GPIO61

#define OUT     1
#define IN      0




void EXT_Flash_PortInit();
void Flash_SendTime(unsigned char *Time);//秒分时日周月年
void FlashReadAll(void);
void FlashReadOneFrame();
void FLASH_SaveOneFrame();
void FLASH_SaveSettings(void);
void FLASH_ReadSettings(void);
void Flash_EraseAll(void);

void Flash_Read_SamplePoint(void);

void Flash_ReadPageGUI(unsigned int BlockAddr,unsigned char PageAddr,unsigned int ColumnAddr,unsigned int Length);
void Flash_WritePageGUI_LOGdata(unsigned int BlockAddr,unsigned char PageAddr,unsigned int ColumnAddr,unsigned int Length);
void Flash_ResetOperation(void);


void Test_NAND_ReadID(void);

void Test_Save_Read_Settings(void);
void Test_FlashControlPins_AllLow_High(void);

#endif /* EXT_FLASH_CONFIG_H_ */
