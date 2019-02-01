/*
 *   main.c: main function
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
#include "rom_map.h"
#include "udma.h"
#include "udma_if.h"
#include "utils.h"
#include "interrupt.h"
#include "prcm.h"
#include "gpio.h"

#include "pinmux.h"
#include "board_conf.h"
#include "gpio_if.h"

/*
 * Push button 2 interrupt handler
 *
 * \param
 * none
 *
 * \return
 * none
 */
void PushButtonHandlerSW2()
{
    unsigned long ulPinState = GPIOIntStatus(GPIOA2_BASE, 1);
    if (ulPinState & GPIO_PIN_6)
    {
        GPIO_IF_DisableInterrupt(SW2);
    }
    GPIO_IF_LedOff(MCU_ALL_LED_IND);
    GPIO_IF_EnableInterrupt(SW2);
}

/*
 * Push button 3 interrupt handler
 *
 * \param
 * none
 *
 * \return
 * none
 */
void PushButtonHandlerSW3()
{
    unsigned long ulPinState = GPIOIntStatus(GPIOA1_BASE, 1);
    if (ulPinState & GPIO_PIN_5)
    {
        GPIO_IF_DisableInterrupt(SW3);
    }
    GPIO_IF_LedOn(MCU_ALL_LED_IND);
    GPIO_IF_EnableInterrupt(SW3);
}

/*
 * Main function
 *
 * \param
 * none
 *
 * \return
 * none
 */
void main(void)
{

    BoardInit(); // Initialize Board configurations
    PinMuxConfig();

    // configure LEDs
    GPIO_IF_LedConfigure(LED1 | LED2 | LED3);

    //Initialize Push Botton Switch
    GPIO_IF_Init(PushButtonHandlerSW2, PushButtonHandlerSW3);

    //Enable GPIO Interrupt
    GPIO_IF_EnableInterrupt(SW3);
    GPIO_IF_EnableInterrupt(SW2);

    while (1)
    {
    }
}
