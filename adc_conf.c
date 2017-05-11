/*
//  Copyright (C) 2017, Zach (Zhengyu) Peng, https://zpeng.me - All Rights Reserved
//
//  Unauthorized copying of this file, via any medium is strictly prohibited
//  Proprietary and confidential
//
//  Written by Zach (Zhengyu) Peng <rookiepeng@gmail.com>, May 2017
*/

#include <stddef.h>

#include <hw_adc.h>
#include <hw_types.h>
#include <adc.h>
#include <hw_udma.h>
#include <udma.h>
#include <rom_map.h>
#include <hw_memmap.h>
#include <udma_if.h>

#include "adc_conf.h"
#include "board_conf.h"

#include <gpio.h>
#include "gpio_if.h"

//main()
//{
//    BoardInit();
//    PinMuxConfig();
//    InitTerm();
//    DisplayBanner(APP_NAME);
//    InitAdcDma();
//}

//unsigned long DmaDataDumpPing[640];
//unsigned long DmaDataDumpPong[640];


void InitAdcDma(unsigned long *g_ucADCBuffPRI, unsigned long *g_ucADCBuffALT, void (*ADCIntHandler)(void))
{
    unsigned short Status;

    UDMAInit();
    //PinTypeADC(PIN_59, 0xFF);
    MAP_uDMAChannelAssign(UDMA_CH16_ADC_CH2);
    UDMASetupTransfer(UDMA_CH16_ADC_CH2 | UDMA_PRI_SELECT, UDMA_MODE_PINGPONG,
                      ADC_BUFF_SIZE,
                      UDMA_SIZE_32, UDMA_ARB_1,
                      (void *)(ADC_BASE + ADC_O_channel4FIFODATA), UDMA_SRC_INC_NONE,
                      (void *)&(g_ucADCBuffPRI), UDMA_DST_INC_32);

    UDMASetupTransfer(UDMA_CH16_ADC_CH2 | UDMA_ALT_SELECT, UDMA_MODE_PINGPONG,
                      ADC_BUFF_SIZE,
                      UDMA_SIZE_32, UDMA_ARB_1,
                      //(void *)(0x4402E874 + ADC_CH_2), UDMA_SRC_INC_NONE,
                      (void *)(ADC_BASE + ADC_O_channel4FIFODATA), UDMA_SRC_INC_NONE,
                      (void *)&(g_ucADCBuffALT), UDMA_DST_INC_32);

    MAP_ADCDMAEnable(ADC_BASE, ADC_CH_2);
    MAP_ADCIntRegister(ADC_BASE, ADC_CH_2, ADCIntHandler);
    Status = MAP_ADCIntStatus(ADC_BASE, ADC_CH_2);
    MAP_ADCIntClear(ADC_BASE, ADC_CH_2, Status | ADC_DMA_DONE);
    MAP_ADCIntEnable(ADC_BASE, ADC_CH_2, ADC_DMA_DONE);
    MAP_ADCChannelEnable(ADC_BASE, ADC_CH_2);
    //
    // Configure ADC timer which is used to timestamp the ADC data samples
    //
    MAP_ADCTimerConfig(ADC_BASE,2^17);

    //
    // Enable ADC timer which is used to timestamp the ADC data samples
    //
    MAP_ADCTimerEnable(ADC_BASE);

    MAP_ADCEnable(ADC_BASE);
}
