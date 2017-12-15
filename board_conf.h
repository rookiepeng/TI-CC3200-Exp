/*
//    Copyright (C) 2017  Zach (Zhengyu) Peng, https://zpeng.me
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __BOARD_CONF_H__
#define __BOARD_CONF_H__

#if defined(ccs)
extern void (*const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif

typedef enum {
  NO_SW,
  SW1 = 0x1, /* SW1/Reset Button */
  SW2 = 0x2, /* SW2/GP22/Pin15 */
  SW3 = 0x4  /* SW3/GP13/Pin4 */

} eSwNum;

void BoardInit(void);

void GPIO_IF_Init(void (*SW2IntHandler)(void), void (*SW3IntHandler)(void));
void GPIO_IF_EnableInterrupt(unsigned char ucSwitch);
void GPIO_IF_DisableInterrupt(unsigned char ucSwitch);

#endif /* __BOARD_CONF_H__ */
