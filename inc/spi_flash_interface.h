/*
 * spi_flash_interface.h
 *
 *  Created on: Dec 12, 2024
 *      Author: Alek
 */

#ifndef INC_SPI_FLASH_INTERFACE_H_
#define INC_SPI_FLASH_INTERFACE_H_

#include "init.h"
#include "cartridge_object.h"


void spi_flash_interface_initialize_SPI();

void spi_flash_begin_autonomous_reads(cartridge_t * const cart_slots);

void spi_flash_interface_validate_chip();

void spi_flash_enable_write(uint8_t const cart_slot);
uint8_t spi_flash_read_status_register(uint8_t const cart_slot);

void spi_flash_write_page(uint8_t * const data, uint16_t const datalen,
	uint32_t const addr, uint8_t const cart_slot);

void spi_flash_read_page(uint8_t * const databuf, uint16_t const datalen,
		uint32_t const addr, uint8_t const cart_slot);

void spi_flash_erase_sector(uint32_t addr, uint8_t const cart_slot);

void spi_flash_write_function(uint32_t const flash_addr, uint16_t num_bytes,
	uint8_t * memptr, uint8_t const cart_slot);

#endif /* INC_SPI_FLASH_INTERFACE_H_ */
