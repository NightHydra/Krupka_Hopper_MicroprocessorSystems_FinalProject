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

void spi_flash_enable_write();
uint8_t spi_flash_read_status_register();

void spi_flash_write_page(uint8_t * const data, uint16_t const datalen,
	uint32_t const addr);

void spi_flash_read_page(uint8_t * const databuf, uint16_t const datalen,
		uint32_t const addr);

void spi_flash_erase_sector(uint32_t addr);

#endif /* INC_SPI_FLASH_INTERFACE_H_ */
