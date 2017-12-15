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
