/*
 * spi_flash_interface.c
 *
 *  Created on: Dec 12, 2024
 *      Author: Alek
 */


#include "spi_flash_interface.h"

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

GPIO_TypeDef * cartridge_nss_ports[MAX_NUMBER_OF_CARTRIDGES] = {GPIOA, GPIOH, GPIOJ};
uint16_t cartridge_nss_pin_numbers[MAX_NUMBER_OF_CARTRIDGES] = {GPIO_PIN_11, GPIO_PIN_6, GPIO_PIN_4};


/** =============================================================
 *  Initialization Function
    ============================================================= */

void spi_flash_gpio_init()
{
	// Initialize the MOSI and MISO pins
	GPIO_InitTypeDef spi_pins_init_struct = {0};

	spi_pins_init_struct.Pin = MOSI_PIN |MISO_PIN;
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
	}

}

void spi_flash_interface_initialize_SPI()
{
	spi_flash_gpio_init();

	flash_spi_handle.Instance = SPI2;
	flash_spi_handle.Init.Mode = SPI_MODE_MASTER; // Set master mode
	flash_spi_handle.Init.TIMode = SPI_TIMODE_DISABLE; // Use Motorola mode, not TI mode
	flash_spi_handle.Init.Direction = SPI_DIRECTION_2LINES; //Subject to change?
	flash_spi_handle.Init.FirstBit = SPI_FIRSTBIT_LSB;
	flash_spi_handle.Init.DataSize = SPI_DATASIZE_16BIT;
	flash_spi_handle.Init.CLKPolarity = SPI_POLARITY_LOW;
	flash_spi_handle.Init.CLKPhase = SPI_PHASE_2EDGE;
	flash_spi_handle.Init.NSS = SPI_NSS_SOFT;
	flash_spi_handle.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
	flash_spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;

	HAL_SPI_Init(&flash_spi_handle);
}

uint8_t spi_flash_read_status_register()
{
	uint8_t to_tx[2] = {0x05, 0x00};

	uint8_t rx_buf[2];

	HAL_SPI_TransmitReceive(&flash_spi_handle, to_tx, rx_buf, 2, HAL_MAX_DELAY);

	return rx_buf[1];
}
