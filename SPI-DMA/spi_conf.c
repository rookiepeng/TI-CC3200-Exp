/*
 *   spi_conf.c: SPI configuration
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
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"
#include "spi.h"
#include "udma.h"
#include "hw_udma.h"
#include "udma_if.h"
#include "interrupt.h"

#include "spi_conf.h"

/*
 * Initialize SPI uDMA
 *
 * \param
 * SPIIntHandler: interrupt handler
 *
 * \return
 * none
 */
void SPI_Init(void (*SPIIntHandler)(void))
{
    //unsigned char i, j;
    unsigned char g_ucDMAEnabled = 1;
    //Enable MCSPIA2
    MAP_PRCMPeripheralClkEnable(PRCM_GSPI, PRCM_RUN_MODE_CLK);
    //Disable Chip Select
    MAP_SPICSDisable(SPI_Interface);
    //Disable SPI Channel
    MAP_SPIDisable(SPI_Interface);
    // Reset SPI * trigger a short CS signal *
    //MAP_SPIReset(SPI_Interface);

// Configure SPI interface
#if MASTER_MODE
    MAP_SPIConfigSetExpClk(SPI_Interface, PRCMPeripheralClockGet(PRCM_GSPI),
                           SPI_IF_BIT_RATE, SPI_MODE_MASTER, SPI_SUB_MODE_0,
                           (SPI_HW_CTRL_CS |
                            SPI_4PIN_MODE |
                            SPI_TURBO_OFF |
                            SPI_CS_ACTIVELOW |
                            SPI_WL_32));

#else
    MAP_SPIConfigSetExpClk(SPI_Interface, PRCMPeripheralClockGet(PRCM_GSPI),
                           SPI_IF_BIT_RATE, SPI_MODE_SLAVE, SPI_SUB_MODE_0,
                           (SPI_HW_CTRL_CS |
                            SPI_4PIN_MODE |
                            SPI_TURBO_OFF |
                            SPI_CS_ACTIVELOW |
                            SPI_WL_32));

#endif

    // Initialize UDMA
    UDMAInit();

    if (MAP_PRCMPeripheralStatusGet(PRCM_UDMA))
    {
        g_ucDMAEnabled = (HWREG(UDMA_BASE + UDMA_O_CTLBASE) != 0x0) ? 1 : 0;
    }
    else
    {
        g_ucDMAEnabled = 0;
    }

    if (g_ucDMAEnabled)
    {
        // Set DMA channel
        MAP_uDMAChannelAssign(UDMA_CH30_GSPI_RX);
        MAP_uDMAChannelAttributeDisable(UDMA_CH30_GSPI_RX, UDMA_ATTR_ALTSELECT);
        MAP_uDMAChannelAssign(UDMA_CH31_GSPI_TX);
        MAP_uDMAChannelAttributeDisable(UDMA_CH31_GSPI_TX, UDMA_ATTR_ALTSELECT);

        MAP_SPIFIFOLevelSet(SPI_Interface, 1, 1);
        MAP_SPIFIFOEnable(SPI_Interface, SPI_RX_FIFO | SPI_TX_FIFO);
        MAP_SPIDmaEnable(SPI_Interface, SPI_RX_DMA | SPI_TX_DMA);

        MAP_IntRegister(INT_GSPI, SPIIntHandler);
#if MASTER_MODE
        //interrupt setting
        MAP_SPIIntEnable(SPI_Interface, SPI_INT_EOW);
        MAP_IntEnable(INT_GSPI);
#endif
        MAP_SPICSEnable(SPI_Interface);
    }
}
