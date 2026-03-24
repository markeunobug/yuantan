/*
 * EXT_FLASH_Config.h
 *
 *  Created on: 2023年6月11日
 *      Author: xikai
 */

#ifndef EXT_FLASH_CONFIG_H_
#define EXT_FLASH_CONFIG_H_

#include "Public_Var.h"

#define Flash_WE_SET   GpioDataRegs.GPASET.bit.GPIO24
#define Flash_WE_CLR   GpioDataRegs.GPACLEAR.bit.GPIO24
#define Flash_WE_DAT  GpioDataRegs.GPADAT.bit.GPIO24
#define Flash_WE_DIR   GpioCtrlRegs.GPADIR.bit.GPIO24

#define Flash_ALE_SET  GpioDataRegs.GPASET.bit.GPIO23
#define Flash_ALE_CLR  GpioDataRegs.GPACLEAR.bit.GPIO23
#define Flash_ALE_DAT  GpioDataRegs.GPADAT.bit.GPIO23
#define Flash_ALE_DIR  GpioCtrlRegs.GPADIR.bit.GPIO23

#define Flash_CLE_SET  GpioDataRegs.GPASET.bit.GPIO22
#define Flash_CLE_CLR  GpioDataRegs.GPACLEAR.bit.GPIO22
#define Flash_CLE_DAT  GpioDataRegs.GPADAT.bit.GPIO22
#define Flash_CLE_DIR  GpioCtrlRegs.GPADIR.bit.GPIO22

#define Flash_RB_SET   GpioDataRegs.GPBSET.bit.GPIO59
#define Flash_RB_CLR   GpioDataRegs.GPBCLEAR.bit.GPIO59
#define Flash_RB_DAT   GpioDataRegs.GPBDAT.bit.GPIO59
#define Flash_RB_DIR   GpioCtrlRegs.GPBDIR.bit.GPIO59

#define Flash_RE_SET   GpioDataRegs.GPBSET.bit.GPIO60
#define Flash_RE_CLR   GpioDataRegs.GPBCLEAR.bit.GPIO60
#define Flash_RE_DAT   GpioDataRegs.GPBDAT.bit.GPIO60
#define Flash_RE_DIR   GpioCtrlRegs.GPBDIR.bit.GPIO60

#define Flash_CE_SET   GpioDataRegs.GPBSET.bit.GPIO61
#define Flash_CE_CLR   GpioDataRegs.GPBCLEAR.bit.GPIO61
#define Flash_CE_DAT   GpioDataRegs.GPBDAT.bit.GPIO61
#define Flash_CE_DIR   GpioCtrlRegs.GPBDIR.bit.GPIO61

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

#endif /* EXT_FLASH_CONFIG_H_ */
