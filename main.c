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

#include "pinmux.h"
#include "board_conf.h"
#include "gpio_if.h"

void PushButtonHandlerSW2()
{
    unsigned long ulPinState = GPIOIntStatus(GPIOA2_BASE, 1);
    //MAP_GPIOIntClear(GPIOA2_BASE,GPIO_PIN_6);
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
    //MAP_GPIOIntClear(GPIOA1_BASE,GPIO_PIN_5);

    if (ulPinState & GPIO_PIN_5)
    {
        GPIO_IF_DisableInterrupt(SW3);
    }
    GPIO_IF_LedOn(MCU_ALL_LED_IND);
    GPIO_IF_EnableInterrupt(SW3);
}

void main(void)
{

    BoardInit(); // Initialize Board configurations
    PinMuxConfig();

    //
    // configure LEDs
    //
    GPIO_IF_LedConfigure(LED1 | LED2 | LED3);
    GPIO_IF_LedOff(MCU_ALL_LED_IND);
    //GPIO_IF_LedOn(MCU_ALL_LED_IND);

    //Initialize Push Botton Switch
    GPIO_IF_Init(PushButtonHandlerSW2, PushButtonHandlerSW3);

    //Enable GPIO Interrupt
    GPIO_IF_EnableInterrupt(SW3);
    GPIO_IF_EnableInterrupt(SW2);

    while (1)
    {
    }

    //return 0;
}
