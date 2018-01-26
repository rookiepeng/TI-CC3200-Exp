/*
 *   board_conf.c: board configuration
 * 
 *   Copyright (C) 2017  Zhengyu Peng, https://zpeng.me
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "hw_ints.h"
#include "hw_mcspi.h"
#include "hw_udma.h"
#include "rom.h"
#include "rom_map.h"
#include "interrupt.h"
#include "prcm.h"
#include "gpio.h"

#include "board_conf.h"

/*
 * Board initialization & configuration
 *
 * \param
 * none
 *
 * \return
 * none
 */
void BoardInit(void)
{
/* In case of TI-RTOS vector table is initialize by OS itself */
#ifndef USE_TIRTOS

// Set vector table base
#if defined(ccs)
    MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);
#endif
#if defined(ewarm)
    MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif
#endif

    // Enable Processor
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);

    PRCMCC3200MCUInit();
}

/*
 * GPIO interrupt initialization
 *
 * \param
 * SW2IntHandler: button 2 interrupt handler
 * SW3IntHandler: button 3 interrupt handler
 *
 * \return
 * none
 */
void GPIO_IF_Init(void (*SW2IntHandler)(void), void (*SW3IntHandler)(void))
{
    // SW3
    // Set Interrupt Type for GPIO
    MAP_GPIOIntTypeSet(GPIOA1_BASE, GPIO_PIN_5, GPIO_FALLING_EDGE);

// Register Interrupt handler
#if defined(USE_TIRTOS) || defined(USE_FREERTOS) || defined(SL_PLATFORM_MULTI_THREADED)
    // USE_TIRTOS: if app uses TI-RTOS (either networking/non-networking)
    // USE_FREERTOS: if app uses Free-RTOS (either networking/non-networking)
    // SL_PLATFORM_MULTI_THREADED: if app uses any OS + networking(simplelink)
    osi_InterruptRegister(INT_GPIOA1, (P_OSI_INTR_ENTRY)SW3IntHandler,
                          INT_PRIORITY_LVL_1);
#else
    MAP_IntPrioritySet(INT_GPIOA1, INT_PRIORITY_LVL_1);
    MAP_GPIOIntRegister(GPIOA1_BASE, SW3IntHandler);
#endif

    // Enable Interrupt
    MAP_GPIOIntClear(GPIOA1_BASE, GPIO_PIN_5);
    MAP_GPIOIntEnable(GPIOA1_BASE, GPIO_INT_PIN_5);

    // SW2
    // Set Interrupt Type for GPIO
    MAP_GPIOIntTypeSet(GPIOA2_BASE, GPIO_PIN_6, GPIO_FALLING_EDGE);

// Register Interrupt handler
#if defined(USE_TIRTOS) || defined(USE_FREERTOS) || defined(SL_PLATFORM_MULTI_THREADED)
    // USE_TIRTOS: if app uses TI-RTOS (either networking/non-networking)
    // USE_FREERTOS: if app uses Free-RTOS (either networking/non-networking)
    // SL_PLATFORM_MULTI_THREADED: if app uses any OS + networking(simplelink)
    osi_InterruptRegister(INT_GPIOA2, (P_OSI_INTR_ENTRY)SW2IntHandler,
                          INT_PRIORITY_LVL_1);
#else
    MAP_IntPrioritySet(INT_GPIOA2, INT_PRIORITY_LVL_1);
    MAP_GPIOIntRegister(GPIOA2_BASE, SW2IntHandler);
#endif

    // Enable Interrupt
    MAP_GPIOIntClear(GPIOA2_BASE, GPIO_PIN_6);
    MAP_GPIOIntEnable(GPIOA2_BASE, GPIO_INT_PIN_6);
}

/*
 * Enables Push Button GPIO Interrupt
 *
 * \param
 * ucSwitch: Push Button Swich Enum - SW2,SW3
 *
 * \return
 * none
 */
void GPIO_IF_EnableInterrupt(unsigned char ucSwitch)
{
    if (ucSwitch & SW2)
    {
        //Enable GPIO Interrupt
        MAP_GPIOIntClear(GPIOA2_BASE, GPIO_PIN_6);
        MAP_IntPendClear(INT_GPIOA2);
        MAP_IntEnable(INT_GPIOA2);
        MAP_GPIOIntEnable(GPIOA2_BASE, GPIO_PIN_6);
    }

    if (ucSwitch & SW3)
    {
        //Enable GPIO Interrupt
        MAP_GPIOIntClear(GPIOA1_BASE, GPIO_PIN_5);
        MAP_IntPendClear(INT_GPIOA1);
        MAP_IntEnable(INT_GPIOA1);
        MAP_GPIOIntEnable(GPIOA1_BASE, GPIO_PIN_5);
    }
}

/*
 * Disables Push Button GPIO Interrupt
 *
 * \param
 * ucSwitch: Push Button Swich Enum - SW2,SW3
 *
 * \return
 * none
 */
void GPIO_IF_DisableInterrupt(unsigned char ucSwitch)
{
    if (ucSwitch & SW2)
    {
        //Clear and Disable GPIO Interrupt
        MAP_GPIOIntDisable(GPIOA2_BASE, GPIO_PIN_6);
        MAP_GPIOIntClear(GPIOA2_BASE, GPIO_PIN_6);
        MAP_IntDisable(INT_GPIOA2);
    }

    if (ucSwitch & SW3)
    {
        //Clear and Disable GPIO Interrupt
        MAP_GPIOIntDisable(GPIOA1_BASE, GPIO_PIN_5);
        MAP_GPIOIntClear(GPIOA1_BASE, GPIO_PIN_5);
        MAP_IntDisable(INT_GPIOA1);
    }
}
