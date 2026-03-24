/*
 * GPIO_Config.h
 *
 *  Created on: 2023Äê6ÔÂ11ÈÕ
 *      Author: xikai
 */

#ifndef BASIC_CONFIG_GPIO_CONFIG_H_
#define BASIC_CONFIG_GPIO_CONFIG_H_

#include "Public_Var.h"

void Set_GPIO_In(Uint32 GPIO);
void Set_GPIO_Out(Uint32 GPIO);
void Set_GPIO_High(Uint32 GPIO);
void Set_GPIO_Low(Uint32 GPIO);
unsigned char Get_GPIO_Value(Uint32 GPIO);

void Enable_Pullup_Gpio(Uint32 GPIO);
void Disable_Pullup_Gpio(Uint32 GPIO);

#endif /* BASIC_CONFIG_GPIO_CONFIG_H_ */
