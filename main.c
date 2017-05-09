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
    unsigned long ulChannelStructIndex, ulMode, ulControl;
    tDMAControlTable *pControlTable;
    unsigned long *pDataDumpBuff = NULL;
    unsigned short Status;
    //unsigned short uiIndex;

    Status = ADCIntStatus(ADC_BASE, ADC_CH_2);
    ADCIntClear(ADC_BASE, ADC_CH_2, Status | ADC_DMA_DONE);
    ulMode = MAP_uDMAChannelModeGet(UDMA_CH16_ADC_CH2 | UDMA_PRI_SELECT);
    if (ulMode == UDMA_MODE_STOP)
    {
        ulChannelStructIndex = UDMA_CH16_ADC_CH2 | UDMA_PRI_SELECT;
        //pDataDumpBuff = &(g_ucADCBuffPRI[0]);
        //TempBuff=g_ucADCBuffPRI;
        //ADCDMADisable(ADC_BASE, ADC_CH_2);
        //ADCIntDisable(ADC_BASE, ADC_CH_2, ADC_DMA_DONE);
        //ADCDisable(ADC_BASE);
        //GPIO_IF_LedOn(MCU_ALL_LED_IND);
    }
    else
    {
        ulMode = MAP_uDMAChannelModeGet(UDMA_CH16_ADC_CH2 | UDMA_ALT_SELECT);
        if (ulMode == UDMA_MODE_STOP)
        {
            ulChannelStructIndex = UDMA_CH16_ADC_CH2 | UDMA_ALT_SELECT;
            //pDataDumpBuff = &(g_ucADCBuffALT[0]);
            //ADCDMADisable(ADC_BASE, ADC_CH_2);
            //ADCIntDisable(ADC_BASE, ADC_CH_2, ADC_DMA_DONE);
            //ADCDisable(ADC_BASE);
            GPIO_IF_LedToggle(MCU_ALL_LED_IND);
        }
    }

//    if (pDataDumpBuff != NULL)
//    {
//        ulChannelStructIndex &= 0x3f;
//        pControlTable = uDMAControlBaseGet();
//        ulControl = (pControlTable[ulChannelStructIndex].ulControl &
//                     ~(UDMA_CHCTL_XFERSIZE_M | UDMA_CHCTL_XFERMODE_M));
//        ulControl |= UDMA_MODE_PINGPONG | ((640 - 1) << 4);
//        uDMAChannelControlSet(ulChannelStructIndex, ulControl);
//    }
    //GPIO_IF_LedOn(MCU_ALL_LED_IND);
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

    InitAdcDma(g_ucADCBuffPRI,g_ucADCBuffALT,ADCIntHandler);

    while (1)
    {
    }

    //return 0;
}
