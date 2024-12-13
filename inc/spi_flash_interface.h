/*
 * spi_flash_interface.h
 *
 *  Created on: Dec 12, 2024
 *      Author: Alek
 */

#ifndef INC_SPI_FLASH_INTERFACE_H_
#define INC_SPI_FLASH_INTERFACE_H_

#include "init.h"


void spi_flash_interface_initialize_SPI();

uint8_t spi_flash_read_status_register();

#endif /* INC_SPI_FLASH_INTERFACE_H_ */
