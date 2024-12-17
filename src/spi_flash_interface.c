/*
 * spi_flash_interface.c
 *
 *  Created on: Dec 12, 2024
 *      Author: Alek
 */


#include "spi_flash_interface.h"
#include "stdbool.h"

/** ===================================================================
 * #DEFINES
 * ==================================================================== */

// MOSI port and pin for flash
#define MOSI_MISO_PORT (GPIOB)
#define MOSI_PIN (GPIO_PIN_15)
#define MISO_PIN (GPIO_PIN_14)

#define SCLK_PORT (GPIOA)
#define SCLK_PIN (GPIO_PIN_12)

#define MAX_NUMBER_OF_CARTRIDGES (3)

/** ==================================================================
 * GLOBAL VARIABLES
 * =================================================================== */


SPI_HandleTypeDef flash_spi_handle;

GPIO_TypeDef * cartridge_nss_ports[MAX_NUMBER_OF_CARTRIDGES] = {GPIOA, GPIOH, GPIOJ}; // D10, D9,D8
uint16_t cartridge_nss_pin_numbers[MAX_NUMBER_OF_CARTRIDGES] = {GPIO_PIN_11, GPIO_PIN_6, GPIO_PIN_4};


/** =============================================================
 *  Initialization Function
    ============================================================= */

void spi_flash_gpio_init()
{
	// Initialize the MOSI and MISO pins
	GPIO_InitTypeDef spi_pins_init_struct = {0};

	spi_pins_init_struct.Pin = MOSI_PIN | MISO_PIN;
	spi_pins_init_struct.Mode = GPIO_MODE_AF_PP;
	spi_pins_init_struct.Pull = GPIO_NOPULL;
	spi_pins_init_struct.Speed = GPIO_SPEED_HIGH;
	spi_pins_init_struct.Alternate = GPIO_AF5_SPI2;			// From Table 13 of stm32f769xx_Datasheet
	HAL_GPIO_Init(MOSI_MISO_PORT, &spi_pins_init_struct);

	// Initialize the SCLK PIN
	spi_pins_init_struct.Pin = SCLK_PIN;

	HAL_GPIO_Init(SCLK_PORT, &spi_pins_init_struct);

	// Initialize the NSS Pins as normal GPIO Pins

	GPIO_InitTypeDef nss_control_pin_init_struct = {0};

	nss_control_pin_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
	nss_control_pin_init_struct.Pull = GPIO_NOPULL;
	nss_control_pin_init_struct.Speed = GPIO_SPEED_HIGH;

	for (uint8_t i = 0; i<MAX_NUMBER_OF_CARTRIDGES; ++i)
	{
		nss_control_pin_init_struct.Pin = cartridge_nss_pin_numbers[i];
		HAL_GPIO_Init(cartridge_nss_ports[i], &nss_control_pin_init_struct);

		// MAKE SURE IT IS HIGH TO START
		HAL_GPIO_WritePin(cartridge_nss_ports[i], cartridge_nss_pin_numbers[i], GPIO_PIN_SET);
	}

}

void spi_flash_interface_initialize_SPI()
{
	spi_flash_gpio_init();

	flash_spi_handle.Instance = SPI2;
	flash_spi_handle.Init.Mode = SPI_MODE_MASTER; // Set master mode
	flash_spi_handle.Init.TIMode = SPI_TIMODE_DISABLE; // Use Motorola mode, not TI mode
	flash_spi_handle.Init.Direction = SPI_DIRECTION_2LINES; //Subject to change?
	flash_spi_handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
	flash_spi_handle.Init.DataSize = SPI_DATASIZE_8BIT;
	flash_spi_handle.Init.CLKPolarity = SPI_POLARITY_LOW;
	flash_spi_handle.Init.CLKPhase = SPI_PHASE_1EDGE;
	flash_spi_handle.Init.NSS = SPI_NSS_SOFT;
	flash_spi_handle.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
	flash_spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;

	HAL_SPI_Init(&flash_spi_handle);
}

void spi_flash_interface_validate_chip()
{
	uint8_t instruction = 0x4b;
	uint8_t dummy_bytes[4] = {0};

	uint8_t high_byte_buf[4] = {0};
	uint8_t low_byte_buf[4] = {0};

	HAL_GPIO_WritePin(cartridge_nss_ports[0], cartridge_nss_pin_numbers[0], GPIO_PIN_RESET);

	HAL_SPI_Transmit(&flash_spi_handle, &instruction, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(&flash_spi_handle, dummy_bytes, 4, HAL_MAX_DELAY);
	HAL_SPI_TransmitReceive(&flash_spi_handle, dummy_bytes, high_byte_buf, 4, HAL_MAX_DELAY);
	HAL_SPI_TransmitReceive(&flash_spi_handle, dummy_bytes, low_byte_buf, 4, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(cartridge_nss_ports[0], cartridge_nss_pin_numbers[0], GPIO_PIN_SET);

	printf("CHIP ID: ");
	for (uint8_t i = 0; i<4; ++i)
	{
		printf("%d", high_byte_buf[i]);
	}
	for (uint8_t i = 0; i<4; ++i)
	{
		printf("%d", low_byte_buf[i]);
	}
	printf("\r\n");
}

void spi_flash_enable_write()
{
	uint8_t to_tx = 0x06;

	uint8_t rx_buf;

	HAL_GPIO_WritePin(cartridge_nss_ports[0], cartridge_nss_pin_numbers[0], GPIO_PIN_RESET);

	HAL_SPI_TransmitReceive(&flash_spi_handle, &to_tx, &rx_buf, 1, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(cartridge_nss_ports[0], cartridge_nss_pin_numbers[0], GPIO_PIN_SET);
}

bool spi_flash_erase_or_write_in_progess()
{
	uint8_t to_tx[2] = {0x05, 0x00};

	uint8_t rx_buf[2];

	HAL_GPIO_WritePin(cartridge_nss_ports[0], cartridge_nss_pin_numbers[0], GPIO_PIN_RESET);

	HAL_SPI_TransmitReceive(&flash_spi_handle, to_tx, rx_buf, 2, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(cartridge_nss_ports[0], cartridge_nss_pin_numbers[0], GPIO_PIN_SET);

	return (rx_buf[1] &0x01);
}


uint8_t spi_flash_read_status_register()
{
	uint8_t to_tx[2] = {0x05, 0x00};

	uint8_t rx_buf[2];

	HAL_GPIO_WritePin(cartridge_nss_ports[0], cartridge_nss_pin_numbers[0], GPIO_PIN_RESET);

	HAL_SPI_TransmitReceive(&flash_spi_handle, to_tx, rx_buf, 2, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(cartridge_nss_ports[0], cartridge_nss_pin_numbers[0], GPIO_PIN_SET);

	return rx_buf[1];
}

void spi_flash_write_page(uint8_t * const data, uint16_t const datalen,
		uint32_t const addr)
{
	spi_flash_enable_write();

	HAL_GPIO_WritePin(cartridge_nss_ports[0], cartridge_nss_pin_numbers[0], GPIO_PIN_RESET);

	uint8_t setup_sequence[4];
	setup_sequence[0] = 0x02; // The write instruction
	setup_sequence[1] = (uint8_t) (addr >> 16);
	setup_sequence[2] = (uint8_t) (addr >> 8);
	setup_sequence[3] = (uint8_t) (addr);

	HAL_SPI_Transmit(&flash_spi_handle, setup_sequence, 4, HAL_MAX_DELAY);

	HAL_SPI_Transmit(&flash_spi_handle, data, datalen, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(cartridge_nss_ports[0], cartridge_nss_pin_numbers[0], GPIO_PIN_SET);

}

void spi_flash_read_page(uint8_t * const databuf, uint16_t const datalen,
		uint32_t const addr)
{
	HAL_GPIO_WritePin(cartridge_nss_ports[0], cartridge_nss_pin_numbers[0], GPIO_PIN_RESET);

	uint8_t setup_sequence[4];
	setup_sequence[0] = 0x03; // The write instruction
	setup_sequence[1] = (uint8_t) (addr >> 16);
	setup_sequence[2] = (uint8_t) (addr >> 8);
	setup_sequence[3] = (uint8_t) (addr);

	HAL_SPI_Transmit(&flash_spi_handle, setup_sequence, 4, HAL_MAX_DELAY);

	HAL_SPI_TransmitReceive(&flash_spi_handle, databuf, databuf, datalen, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(cartridge_nss_ports[0], cartridge_nss_pin_numbers[0], GPIO_PIN_SET);
}

void spi_flash_erase_sector(uint32_t addr)
{
	spi_flash_enable_write();

	HAL_GPIO_WritePin(cartridge_nss_ports[0], cartridge_nss_pin_numbers[0], GPIO_PIN_RESET);

	uint8_t command_seq[4];
	command_seq[0] = 0x20;
	command_seq[1] = (uint8_t) (addr >> 16);
	command_seq[2] = (uint8_t) (addr >> 8);
	command_seq[3] = (uint8_t) (addr);

	HAL_SPI_Transmit(&flash_spi_handle, command_seq, 4, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(cartridge_nss_ports[0], cartridge_nss_pin_numbers[0], GPIO_PIN_SET);
}

void spi_flash_write_function(uint32_t flash_addr, uint16_t num_bytes,
	uint8_t * func_ptr)
{
	spi_flash_erase_sector(flash_addr);

	while(spi_flash_erase_or_write_in_progess())
	{
		printf("Waiting....\r\n");
	}

	// Less than or equal to 256 bytes and just go to the last one
	while (num_bytes > 256)
	{

		spi_flash_write_page(func_ptr, 256, flash_addr);
		flash_addr += 256;
		func_ptr +=256;
		num_bytes -= 256;
	}


	spi_flash_write_page(func_ptr, num_bytes, flash_addr);

	while(spi_flash_erase_or_write_in_progess())
	{
		printf("Waiting2....\r\n");
	}
}

