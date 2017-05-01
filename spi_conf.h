#ifndef SPI_CONF_H_
#define SPI_CONF_H_

//*****************************************************************************
//
// Application Master/Slave mode selector macro
//
// MASTER_MODE = 1 : Application in master mode
// MASTER_MODE = 0 : Application in slave mode
//
//*****************************************************************************
#define MASTER_MODE 1

#define MASTER_MSG "This is CC3200 SPI Master Application\n\r"
#define SLAVE_MSG "This is CC3200 SPI Slave Application\n\r"

#define SPI_Interface GSPI_BASE  // SPI interface
#define SPI_IF_BIT_RATE 20000000 // * must be an integer divisor of 20000000 *
#define TR_BUFF_SIZE 1024

void SPI_Init(void (*SPIIntHandler)(void));

#endif /* SPI_CONF_H_ */
