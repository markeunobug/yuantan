/*
 * GPIO_Config.c
 *
 *  Created on: 2023Äê6ÔÂ11ÈÕ
 *      Author: xikai
 */
#include "GPIO_Config.h"

void Set_GPIO_In(Uint32 GPIO)
{
    EALLOW;
    if(GPIO<32)
        GpioCtrlRegs.GPADIR.all &= ~((unsigned long)1<<GPIO);
    else if(GPIO<64)
        GpioCtrlRegs.GPBDIR.all &= ~((unsigned long)1<<(GPIO-32));
    else
        GpioCtrlRegs.GPCDIR.all &= ~((unsigned long)1<<(GPIO-64));
    EDIS;
}

void Set_GPIO_Out(Uint32 GPIO)
{
    EALLOW;
    if(GPIO<32)
        GpioCtrlRegs.GPADIR.all |= ((unsigned long)1<<GPIO);
    else if(GPIO<64)
        GpioCtrlRegs.GPBDIR.all |= ((unsigned long)1<<(GPIO-32));
    else
        GpioCtrlRegs.GPCDIR.all |= ((unsigned long)1<<(GPIO-64));
    EDIS;
}

void Set_GPIO_High(Uint32 GPIO)
{
    if(GPIO<32)
        GpioDataRegs.GPASET.all |= ((unsigned long)1<<GPIO);
    else if(GPIO<64)
        GpioDataRegs.GPBSET.all |= ((unsigned long)1<<(GPIO-32));
    else
        GpioDataRegs.GPCSET.all |= ((unsigned long)1<<(GPIO-64));
}

void Set_GPIO_Low(Uint32 GPIO)
{
    if(GPIO<32)
        GpioDataRegs.GPACLEAR.all |= ((unsigned long)1<<GPIO);
    else if(GPIO<64)
        GpioDataRegs.GPBCLEAR.all |= ((unsigned long)1<<(GPIO-32));
    else
        GpioDataRegs.GPCCLEAR.all |= ((unsigned long)1<<(GPIO-64));
}

void Enable_Pullup_Gpio(Uint32 GPIO)
{
    EALLOW;
    if(GPIO<32)
        GpioCtrlRegs.GPAPUD.all &= ~((unsigned long)1<<GPIO);
    else if(GPIO<64)
        GpioCtrlRegs.GPBPUD.all &= ~((unsigned long)1<<(GPIO-32));
    else
        GpioCtrlRegs.GPCPUD.all &= ~((unsigned long)1<<(GPIO-64));
    EDIS;
}


void Disable_Pullup_Gpio(Uint32 GPIO)
{
    EALLOW;
    if(GPIO<32)
        GpioCtrlRegs.GPAPUD.all |= ((unsigned long)1<<GPIO);
    else if(GPIO<64)
        GpioCtrlRegs.GPBPUD.all |= ((unsigned long)1<<(GPIO-32));
    else
        GpioCtrlRegs.GPCPUD.all |= ((unsigned long)1<<(GPIO-64));
    EDIS;
}

unsigned char Get_GPIO_Value(Uint32 GPIO)
{
    if(GPIO<32)
        return (GpioDataRegs.GPADAT.all >> GPIO) & 0x01;
    else if(GPIO<64)
        return (GpioDataRegs.GPBDAT.all >> (GPIO-32)) & 0x01;
    else
        return (GpioDataRegs.GPCDAT.all >> (GPIO-64)) & 0x01;
}



