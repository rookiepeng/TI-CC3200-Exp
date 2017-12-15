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

#include <stddef.h>
#include <hw_types.h>
#include <hw_memmap.h>
#include <hw_common_reg.h>
#include <hw_ints.h>
#include <hw_mcspi.h>
#include <rom_map.h>
#include <udma.h>
#include <udma_if.h>
#include <utils.h>
#include <interrupt.h>
#include <prcm.h>
#include <gpio.h>
#include <adc.h>
#include <hw_udma.h>
#include <adc.h>
#include <hw_adc.h>

#include "pinmux.h"
#include "board_conf.h"
#include "gpio_if.h"
#include "adc_conf.h"

unsigned long g_ucADCBuffPRI[ADC_BUFF_SIZE];
unsigned long g_ucADCBuffALT[ADC_BUFF_SIZE];
//UART_PRINT("\n\rbits[13:2] : ADC sample\n\r");
//UART_PRINT("\n\rbits[31:14]: Time stamp of ADC sample \n\r");

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

void ADCIntHandler()
{
    unsigned long ulMode;
    unsigned long ulStatus;

    ulStatus = ADCIntStatus(ADC_BASE, ADC_CH_2);
    ADCIntClear(ADC_BASE, ADC_CH_2, ulStatus | ADC_DMA_DONE);
    ulMode = MAP_uDMAChannelModeGet(UDMA_CH16_ADC_CH2 | UDMA_PRI_SELECT);
    if (ulMode == UDMA_MODE_STOP)
    {
        UDMASetupTransfer(UDMA_CH16_ADC_CH2 | UDMA_PRI_SELECT, UDMA_MODE_PINGPONG,
                          ADC_BUFF_SIZE,
                          UDMA_SIZE_32, UDMA_ARB_1,
                          (void *)(ADC_BASE + ADC_O_channel4FIFODATA), UDMA_SRC_INC_NONE,
                          (void *)&(g_ucADCBuffPRI), UDMA_DST_INC_32);
        //ulChannelStructIndex = UDMA_CH16_ADC_CH2 | UDMA_PRI_SELECT;
        //pDataDumpBuff = &(g_ucADCBuffPRI[0]);
        //TempBuff=g_ucADCBuffPRI;
        //ADCDMADisable(ADC_BASE, ADC_CH_2);
        //ADCIntDisable(ADC_BASE, ADC_CH_2, ADC_DMA_DONE);
        //ADCDisable(ADC_BASE);
        GPIO_IF_LedOn(MCU_ALL_LED_IND);
    }
    else
    {
        ulMode = MAP_uDMAChannelModeGet(UDMA_CH16_ADC_CH2 | UDMA_ALT_SELECT);
        if (ulMode == UDMA_MODE_STOP)
        {
            //ulChannelStructIndex = UDMA_CH16_ADC_CH2 | UDMA_ALT_SELECT;
            //pDataDumpBuff = &(g_ucADCBuffALT[0]);
            //ADCDMADisable(ADC_BASE, ADC_CH_2);
            //ADCIntDisable(ADC_BASE, ADC_CH_2, ADC_DMA_DONE);
            //ADCDisable(ADC_BASE);
            UDMASetupTransfer(UDMA_CH16_ADC_CH2 | UDMA_ALT_SELECT, UDMA_MODE_PINGPONG,
                              ADC_BUFF_SIZE,
                              UDMA_SIZE_32, UDMA_ARB_1,
                              (void *)(ADC_BASE + ADC_O_channel4FIFODATA), UDMA_SRC_INC_NONE,
                              (void *)&(g_ucADCBuffALT), UDMA_DST_INC_32);
            GPIO_IF_LedOff(MCU_ALL_LED_IND);
        }
    }
}

void main(void)
{
    BoardInit(); // Initialize Board configurations
    PinMuxConfig();

    // configure LEDs
    GPIO_IF_LedConfigure(LED1 | LED2 | LED3);
    GPIO_IF_LedOn(MCU_ALL_LED_IND);
    //GPIO_IF_LedOff(MCU_ALL_LED_IND);

    //Initialize Push Botton Switch
    GPIO_IF_Init(PushButtonHandlerSW2, PushButtonHandlerSW3);

    //Enable GPIO Interrupt
    GPIO_IF_EnableInterrupt(SW3);
    GPIO_IF_EnableInterrupt(SW2);

    InitAdcDma(g_ucADCBuffPRI, g_ucADCBuffALT, ADCIntHandler);

    while (1)
    {
    }

    //return 0;
}
