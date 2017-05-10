/*
//  Copyright (C) 2017, Zach (Zhengyu) Peng, https://zpeng.me - All Rights Reserved
//
//  Unauthorized copying of this file, via any medium is strictly prohibited
//  Proprietary and confidential
//
//  Written by Zach (Zhengyu) Peng <rookiepeng@gmail.com>, May 2017
*/

#include <hw_types.h>
#include <hw_memmap.h>
#include <hw_common_reg.h>
#include <hw_ints.h>
#include <hw_mcspi.h>
#include <rom_map.h>
#include <spi.h>
#include <uart_if.h>
#include <udma.h>
#include <udma_if.h>
#include <utils.h>
#include <interrupt.h>
#include <prcm.h>

#include "board_conf.h"
#include "pinmux.h"
#include "spi_conf.h"

#define APPLICATION_VERSION "1.0.0"

//*****************************************************************************
//                 GLOBAL VARIABLES -- Start
unsigned long g_ucTxBuff[TR_BUFF_SIZE];
unsigned long g_ucRxBuff[TR_BUFF_SIZE];

static unsigned char ucTxBuffNdx;
static unsigned char ucRxBuffNdx;

volatile static unsigned char g_cTXDone;
//                 GLOBAL VARIABLES -- End
//*****************************************************************************

// SPI Interrupt handler
static void SPIIntHandler()
{
    //unsigned long ulRecvData;
    unsigned long ulStatus;

    ulStatus = MAP_SPIIntStatus(SPI_Interface, true);

    MAP_SPIIntClear(SPI_Interface, SPI_INT_EOW);
    MAP_SPICSDisable(SPI_Interface);

    if (ulStatus & SPI_INT_TX_EMPTY)
    {
        Message("TX EMPTY\r\n");
        ucTxBuffNdx++;
    }
    if (ulStatus & SPI_INT_RX_FULL)
    {
        Message("RX FULL\r\n");
        ucRxBuffNdx++;
    }
    if (ulStatus & SPI_INT_DMARX)
    {
        Message("DMA RX\r\n");
        ucTxBuffNdx++;
    }
    if (ulStatus & SPI_INT_DMATX)
    {
        Message("DMA TX\r\n");
        ucTxBuffNdx++;
    }
    if (ulStatus & SPI_INT_EOW)
    {
        g_cTXDone = 1;
    }
}

void Slave_Receive(unsigned char *rxBuff, unsigned char *txBuff, int len)
{
    MAP_SPIWordCountSet(SPI_Interface, len);
    UDMASetupTransfer(UDMA_CH30_GSPI_RX, UDMA_MODE_BASIC, len,
                      UDMA_SIZE_8, UDMA_ARB_1,
                      (void *)(SPI_Interface + MCSPI_O_RX0), UDMA_SRC_INC_NONE,
                      (void *)rxBuff, UDMA_DST_INC_8);
    UDMASetupTransfer(UDMA_CH31_GSPI_TX, UDMA_MODE_BASIC, len,
                      UDMA_SIZE_8, UDMA_ARB_1,
                      (void *)txBuff, UDMA_SRC_INC_8, (void *)(SPI_Interface + MCSPI_O_TX0),
                      UDMA_DST_INC_NONE);
    MAP_SPIEnable(SPI_Interface);

#if MASTER_MODE
#else
    while (!(MAP_SPIIntStatus(SPI_Interface, false) & SPI_INT_EOW))
        ;
    MAP_SPIIntClear(SPI_Interface, SPI_INT_EOW);
#endif
    MAP_SPIDisable(SPI_Interface);
}

void Master_Send(unsigned long *rxBuff, unsigned long *txBuff, int len)
{
    MAP_SPIWordCountSet(SPI_Interface, len);
    UDMASetupTransfer(UDMA_CH31_GSPI_TX, UDMA_MODE_BASIC, len,
                      UDMA_SIZE_32, UDMA_ARB_1,
                      (void *)txBuff, UDMA_SRC_INC_32, (void *)(SPI_Interface + MCSPI_O_TX0),
                      UDMA_DST_INC_NONE);
    UDMASetupTransfer(UDMA_CH30_GSPI_RX, UDMA_MODE_BASIC, len,
                      UDMA_SIZE_32, UDMA_ARB_1,
                      (void *)(SPI_Interface + MCSPI_O_RX0), UDMA_SRC_INC_NONE,
                      (void *)rxBuff, UDMA_DST_INC_32);
    MAP_SPIEnable(SPI_Interface);
}

void main()
{
    g_cTXDone = 0;
    BoardInit();// Initialize Board configurations
    PinMuxConfig(); // Muxing UART and SPI lines.
    MAP_PRCMPeripheralClkEnable(PRCM_GSPI, PRCM_RUN_MODE_CLK);// Enable the SPI module clock
    InitTerm(); // Initializing the Terminal.
    ClearTerm(); // Clearing the Terminal.

    // Display the Banner
    Message("\n\n\n\r");
    Message("\t\t   ********************************************\n\r");
    Message("\t\t        CC3200 SPI Demo Application  \n\r");
#if MASTER_MODE
    Message(MASTER_MSG);
    Message("\n\r");
#else
    Message(SLAVE_MSG);
    Message("\n\r");
#endif
    Message("\t\t   ********************************************\n\r");
    Message("\n\n\n\r");

    MAP_PRCMPeripheralReset(PRCM_GSPI); // Reset the peripheral
    SPI_Init(SPIIntHandler);

    unsigned char i, j;
    for (j = 0; j < 4; j++)
    {
        for (i = 0; i < 255; i++)
        {
            //g_ucTxBuff[i + j * 255] = i;
            g_ucTxBuff[i + j * 255] = 0;
        }
    }
    g_ucTxBuff[0] = 0xFFFFFFFF;
    g_ucTxBuff[1022] = 0xFFFFFFFF;
    g_ucTxBuff[127] = 0xFFFFFFFF;
    g_ucTxBuff[511] = 0xFFFFFFFF;

#if MASTER_MODE
    Master_Send(g_ucRxBuff, g_ucTxBuff, 1024);
#endif

    while (1)
    {
#if MASTER_MODE
        if (g_cTXDone == 1)
#endif
        {
#if MASTER_MODE
            MAP_UtilsDelay(40000);
            g_cTXDone = 0;
            SPI_Init(SPIIntHandler);
            Master_Send(g_ucRxBuff, g_ucTxBuff, 1024);
            //MAP_SPIIntEnable(SPI_Interface, SPI_INT_EOW);
            //MAP_IntEnable(INT_GSPI);
#else
            memset(g_ucRxBuff, 0x0, sizeof(g_ucRxBuff));
            memset(g_ucTxBuff, 0x0, sizeof(g_ucTxBuff));
            Slave_Receive(g_ucRxBuff, g_ucTxBuff, 1024);
#endif
        }
    }
}
