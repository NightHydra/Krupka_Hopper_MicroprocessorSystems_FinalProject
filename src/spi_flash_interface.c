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

/** =========================================================================
 *  SELF DEFINED TYPES
 *  ========================================================================= */


/**
 * The states for the cart polling state machine
 */
typedef enum
{
	READY_TO_POLL_STATE,
	POLLING_DEVICE_ID_STATE,
	READING_IN_ROM_HEADER_STATE,
	READING_IN_ROM_STATE
} round_robin_read_state_t;

/** ==================================================================
 * GLOBAL VARIABLES
 * =================================================================== */

/**
 * The hardware handlers for the spi flash and the dmas
 */
static SPI_HandleTypeDef flash_spi_handle;
static DMA_HandleTypeDef flash_tx_dma_handle;
static DMA_HandleTypeDef flash_rx_dma_handle;

/**
 * @brief Globals for the cartridge nss ports and pins that enable talking to a speicfic cart.
 */
GPIO_TypeDef * cartridge_nss_ports[MAX_NUMBER_OF_CARTRIDGES] = {GPIOA, GPIOH, GPIOJ}; // D10, D9,D8
uint16_t cartridge_nss_pin_numbers[MAX_NUMBER_OF_CARTRIDGES] = {GPIO_PIN_11, GPIO_PIN_6, GPIO_PIN_4};

/**
 * @brief Stores the state in our
 */
round_robin_read_state_t cart_read_state;
/**
 * @brief The index of the cart to poll next or that is currently
 *     being polled
 */
uint8_t cart_to_read_ind = 0;

/**
 * @brief The representation for our array of cartridges.  Store the pointers
 *     to the cart memory array in @ref readMain.cpp
 */
cartridge_t * cartidges_slots_to_read_into;
/**
 * @brief Handler for the timer that causes an interrupt to start polling a cart
 */
TIM_HandleTypeDef begin_poll_timer_handle;

/**
 * @brief Buffers to store received DMA data for the id and cart header
 */
uint8_t device_id_buf[8];

uint8_t cart_header_buf[8];


/** =============================================================
 *  Initialization Function
    ============================================================= */

// Very simple functions for driving the nss pin low or high

/**
 * @brief Helper function for driving an NSS pin low
 */
void cart_nss_activate(uint8_t cart_num)
{
	HAL_GPIO_WritePin(cartridge_nss_ports[cart_num], cartridge_nss_pin_numbers[cart_num], GPIO_PIN_RESET);
}

/**
 * @brief Helper function for releases an NSS pin
 */
void cart_nss_deactivate(uint8_t cart_num)
{
	HAL_GPIO_WritePin(cartridge_nss_ports[cart_num], cartridge_nss_pin_numbers[cart_num], GPIO_PIN_SET);
}

/**
 * @brief A helper functions for wrapping around the read index when
 *    it gets to the max cart number
 */
void inc_cart_to_read_ind(uint8_t * ind)
{
	++(*ind);
	if (*ind >= 3)
	{
		*ind = 0;
	}
}

/**
 * @brief Initialization for the SPI DMA
 */
void spi_flash_dma_init()
{
	// Configure the DMA first, then the SPI
	flash_tx_dma_handle.Instance = DMA1_Stream4;
	flash_tx_dma_handle.Init.Channel = DMA_CHANNEL_0;
	flash_tx_dma_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;
	// This might not make a lot of sense but when reading its actually far more
	//     efficient to just continually tx the same byte as the SPI flash chip
	//     requires a lot of dummy bytes.  txing the instruction is only 1 byte
	//     but even then we really shouldnt be using DMA for that as its efficiency
	//     is vastly reduced when only transmitting 1 byte
	flash_tx_dma_handle.Init.MemInc = DMA_MINC_DISABLE;
	flash_tx_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;
	flash_tx_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	flash_tx_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	flash_tx_dma_handle.Init.Mode = DMA_NORMAL;
	flash_tx_dma_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	flash_tx_dma_handle.Init.MemBurst = DMA_MBURST_SINGLE;
	flash_tx_dma_handle.Init.Priority = DMA_PRIORITY_VERY_HIGH;

	HAL_DMA_Init(&flash_tx_dma_handle);



	// Configure the DMA first, then the SPI
	flash_rx_dma_handle.Instance = DMA1_Stream3;
	flash_rx_dma_handle.Init.Channel = DMA_CHANNEL_0;
	flash_rx_dma_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
	flash_rx_dma_handle.Init.MemInc = DMA_MINC_ENABLE;
	flash_rx_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;
	flash_rx_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	flash_rx_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	flash_rx_dma_handle.Init.Mode = DMA_NORMAL;
	flash_rx_dma_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	flash_rx_dma_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;
	flash_rx_dma_handle.Init.Priority = DMA_PRIORITY_HIGH;


	// Initialize the dma
	HAL_DMA_Init(&flash_rx_dma_handle);

	HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
	HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);

}


/**
 * @brief Initialize the GPIO pins used for the SPI2 peripheral and the respective
 *    NSS pins for each cart
 */
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
		cart_nss_deactivate(i);
	}

}

/**
 * @brief Function for initializing the SPI interface.  This function also
 *    initializes the respective DMA and the GPIO pins
 */
void spi_flash_interface_initialize_SPI()
{
	spi_flash_gpio_init();

	spi_flash_dma_init();

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
	flash_spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;

	flash_spi_handle.hdmatx = &flash_tx_dma_handle;
	flash_spi_handle.hdmatx = &flash_rx_dma_handle;

	HAL_SPI_Init(&flash_spi_handle);

	// Even for writing entire programs (which wont use dma), no reason it needs
	//     To remain uninitialized
	__HAL_LINKDMA(&flash_spi_handle, hdmatx, flash_tx_dma_handle);
	__HAL_LINKDMA(&flash_spi_handle, hdmarx, flash_rx_dma_handle);
}


/**
 * @brief sends a 1 byte command for enabling the write bit of the status register
 *     in the flash cart.  Calling this before erases any section of flash is required.
 */
void spi_flash_enable_write(uint8_t const cart_slot)
{
	uint8_t to_tx = 0x06;
	uint8_t rx_buf;

	cart_nss_activate(cart_slot);

	HAL_SPI_TransmitReceive(&flash_spi_handle, &to_tx, &rx_buf, 1, HAL_MAX_DELAY);

	cart_nss_deactivate(cart_slot);
}


/**
 * @brief Sends a SPI command to the flash chip to see if an erase or write is
 *     occurring.  Used since you cant write while the flash chip is erasing a sector
 *     Upon testing it is determined that this function is necessary
 */
bool spi_flash_erase_or_write_in_progess(uint8_t const cart_slot)
{
	uint8_t to_tx[2] = {0x05, 0x00};

	uint8_t rx_buf[2];

	cart_nss_activate(cart_slot);

	HAL_SPI_TransmitReceive(&flash_spi_handle, to_tx, rx_buf, 2, HAL_MAX_DELAY);

	cart_nss_deactivate(cart_slot);

	return (rx_buf[1] &0x01);
}

/**
 * @brief Reads status register 1 of the SPI flash memory chip and returns
 *     the response in a UINT8.
 * @note This function blocks until the byte is received.
 */
uint8_t spi_flash_read_status_register(uint8_t const cart_slot)
{
	uint8_t to_tx[2] = {0x05, 0x00};

	uint8_t rx_buf[2];

	cart_nss_activate(cart_slot);

	HAL_SPI_TransmitReceive(&flash_spi_handle, to_tx, rx_buf, 2, HAL_MAX_DELAY);

	cart_nss_deactivate(cart_slot);

	return rx_buf[1];
}


/**
 * @brief Writes an entire page of flash memory
 * @param begin determines if this write needs to drive the nss pin low
 *     and start the write instruction.  This is needed because if we want
 *     to write 2 bytes to a page, then the remaining 254 bytes we have to do
 *     this all with 1 command.
 * @param end If true this means that we should finish the instruction with this
 *    call and if false the do not finish it and allow for the function to be called
 *    again and write more memory to the same page.
 */
void spi_flash_write_page(uint8_t * const data, uint16_t const datalen,
		uint32_t const addr, uint8_t const cart_slot, bool begin,
		bool end)
{
	if (begin)
	{
		spi_flash_enable_write(cart_slot);

		uint8_t setup_sequence[4];
		setup_sequence[0] = 0x02; // The write instruction
		setup_sequence[1] = (uint8_t) (addr >> 16);
		setup_sequence[2] = (uint8_t) (addr >> 8);
		setup_sequence[3] = (uint8_t) (addr);

		cart_nss_activate(cart_slot);

		HAL_SPI_Transmit(&flash_spi_handle, setup_sequence, 4, HAL_MAX_DELAY);
	}


	HAL_SPI_Transmit(&flash_spi_handle, data, datalen, HAL_MAX_DELAY);

	if (end)
	{
		cart_nss_deactivate(cart_slot);
	}


}

/**
 * @brief Reads @ref datalen bytes from the flash memory starting at
 *     @ref addr.
 * @note This function is blocking so it should not be used for polling
 *     carts.  Instead DMA and interrupts should be used for that functionality
 *     if blocking is not desired.
 */
void spi_flash_read_data(uint8_t * const databuf, uint16_t const datalen,
		uint32_t const addr, uint8_t const cart_slot)
{

	uint8_t setup_sequence[4];
	setup_sequence[0] = 0x03; // The write instruction
	setup_sequence[1] = (uint8_t) (addr >> 16);
	setup_sequence[2] = (uint8_t) (addr >> 8);
	setup_sequence[3] = (uint8_t) (addr);

	cart_nss_activate(cart_slot);

	HAL_SPI_Transmit(&flash_spi_handle, setup_sequence, 4, HAL_MAX_DELAY);

	HAL_SPI_TransmitReceive(&flash_spi_handle, databuf, databuf, datalen, HAL_MAX_DELAY);

	cart_nss_deactivate(cart_slot);
}

/**
 * Erase a sector of the flash memory starting at @ref addr
 */
void spi_flash_erase_sector(uint32_t addr, uint8_t const cart_slot)
{
	spi_flash_enable_write(cart_slot);

	uint8_t command_seq[4];
	command_seq[0] = 0x20;
	command_seq[1] = (uint8_t) (addr >> 16);
	command_seq[2] = (uint8_t) (addr >> 8);
	command_seq[3] = (uint8_t) (addr);

	cart_nss_activate(cart_slot);

	HAL_SPI_Transmit(&flash_spi_handle, command_seq, 4, HAL_MAX_DELAY);

	cart_nss_deactivate(cart_slot);
}


/**
 * Writes a program to the flash memory including the headers starting at @ref
 *     app_begin_addr
 */
void spi_flash_write_func_memory(uint32_t flash_addr, uint32_t num_bytes,
	uint8_t * app_begin_addr, uint8_t const cart_slot, uint32_t starting_func_offset)
{
	// Write the number of bytes and the main function pointer
	//    as the first 8 bytes
	spi_flash_erase_sector(flash_addr, cart_slot);

	while(spi_flash_erase_or_write_in_progess(cart_slot))
	{
		printf("Waiting....\r");
	}

	static uint8_t header_buf[8];
	for (uint8_t i = 0; i<4; ++i)
	{
		header_buf[i] = (uint8_t) ((num_bytes >> (8*(3-i))) & 0xFF);
		header_buf[i+4] = (uint8_t) ((starting_func_offset >> (8*(3-i))) & 0xFF);
	}

	spi_flash_write_page(header_buf, 8, flash_addr, cart_slot, true, false);
	flash_addr += 8;

	bool first_page_ended = false;

	// Take into account that the first page might not
	//     be able to contain all 256 bytes
	int16_t max_number_of_bytes_of_first_page =
		((uint32_t)flash_addr & 0xFF);
	if (num_bytes > 256 - max_number_of_bytes_of_first_page)
	{
		spi_flash_write_page(app_begin_addr, 256 - max_number_of_bytes_of_first_page
			, flash_addr, cart_slot, first_page_ended, true);
		first_page_ended = true;
		flash_addr += 256 - max_number_of_bytes_of_first_page;
		app_begin_addr += 256 - max_number_of_bytes_of_first_page;
		num_bytes -= 256 - max_number_of_bytes_of_first_page;
	}

	// Less than or equal to 256 bytes and just go to the last one
	while (num_bytes >= 256)
	{

		spi_flash_write_page(app_begin_addr, 256, flash_addr,
			cart_slot, false, true);
		flash_addr += 256;
		app_begin_addr +=256;
		num_bytes -= 256;
	}


	spi_flash_write_page(app_begin_addr, num_bytes, flash_addr,
		cart_slot, first_page_ended, true);

	while(spi_flash_erase_or_write_in_progess(cart_slot))
	{
		printf("Waiting2....\r\n");
	}
}


/**
 * Below we define all the functions for continuous reads.  This allows for the dma to just
 * 	put the program data into the flash carts
 */
void timer_init()
{
	begin_poll_timer_handle.Instance = TIM7;
	begin_poll_timer_handle.Init.Prescaler = 10800;
	begin_poll_timer_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	begin_poll_timer_handle.Init.Period = 10000; // Overflow causes an interrupt every 1s


	begin_poll_timer_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;


	HAL_TIM_Base_Init(&begin_poll_timer_handle);

	HAL_TIM_Base_Start_IT(&begin_poll_timer_handle);

	HAL_NVIC_EnableIRQ(TIM7_IRQn); //TIM 7 Interupt Enable through NVIC (TIM 7 Position is 55)
}

/**
 * @brief This function starts reading the cart data into the cartridge buffers
 *     in the array that @ref cart_slots points to
 */
void spi_flash_begin_autonomous_reads(cartridge_t * const cart_slots)
{
	// Initialize a timer and have it trigger interrupts every 1 second to start a cart poll
	cart_read_state = READY_TO_POLL_STATE;
	cart_to_read_ind = 0;
	cartidges_slots_to_read_into = cart_slots;

	// Initialize the timer and wait for everything to get loaded
	timer_init();
}


void TIM7_IRQHandler()
{
	HAL_TIM_IRQHandler(&begin_poll_timer_handle);
}


/**
 * @brief Upon the timer interrupt start polling the cart
 */
void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef * htim)
{
	static uint8_t read_device_id_instruction_byte = 0x4B;
	if (htim->Instance == TIM7)
	{
		// ONLY DO anything if we are actually ready to poll
		if (cart_read_state == READY_TO_POLL_STATE)
		{
			cart_read_state = POLLING_DEVICE_ID_STATE;
			// Just start transmitting using the dma while we go back to doing other stuff
			cart_nss_activate(cart_to_read_ind);
			HAL_SPI_Transmit_DMA(&flash_spi_handle, &read_device_id_instruction_byte, 5);
		}
	}
}

// DMA and SPI IRQ Handlers
void DMA1_Stream3_IRQHandler()
{
	HAL_DMA_IRQHandler(&flash_rx_dma_handle);
}

void DMA1_Stream4_IRQHandler()
{
	HAL_DMA_IRQHandler(&flash_tx_dma_handle);
}
void SPI2_IRQHandler()
{
	HAL_SPI_IRQHandler(&flash_spi_handle);
}


/**
 * @brief Handle when the SPI DMA is finished sending the message
 */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef * hspi)
{
	static uint8_t dummy_byte = 0;
	if (hspi->Instance == SPI2)
	{
		if (cart_read_state == POLLING_DEVICE_ID_STATE)
		{
			// If we are here then we know that we just sent the instruction
			//     to read in the device id so transmit + receive via dma
			// No need to change state though.
			HAL_SPI_TransmitReceive_DMA( &flash_spi_handle, &dummy_byte , device_id_buf, 8);
		}
	}
}

/**
 * Callback for when SPI TX/RX is complete.
 */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef * hspi)
{
	static uint8_t dummy_byte = 0;
	if (hspi->Instance == SPI2)
	{
		if (cart_read_state == POLLING_DEVICE_ID_STATE)
		{

			// At this point we have read in the whole device id into a buffer
			//    If it is 0, assume the cart is empty and move on
			// If not, compare and see if it is what is expected, if not
			//    we then read in that carts data
			cart_nss_deactivate(cart_to_read_ind);

			// Check to see if the id is all 0s, if so dont read it in.
			bool cart_all_zeros = true;
			for (uint8_t i = 0; i<8; ++i)
			{
				if (device_id_buf[i] != 0) {cart_all_zeros = false; break;}
			}

			if (cart_all_zeros)
			{
				// In this case the cart is invalid
				cartidges_slots_to_read_into[cart_to_read_ind].data_initialized = false;
				cart_read_state = READY_TO_POLL_STATE;
				inc_cart_to_read_ind(&cart_to_read_ind);
			}
			else if (cartidges_slots_to_read_into[cart_to_read_ind].data_initialized == true &&
					cart_ids_match(cartidges_slots_to_read_into+cart_to_read_ind, device_id_buf))
			{
				// Data is already read into the buffer and the poll was successful
				cart_read_state = READY_TO_POLL_STATE;
				inc_cart_to_read_ind(&cart_to_read_ind);
			}
			else
			{
				memcpy(cartidges_slots_to_read_into[cart_to_read_ind].cart_unique_id, device_id_buf, 8);
				cart_read_state = READING_IN_ROM_HEADER_STATE;
				cart_nss_activate(cart_to_read_ind);
				static uint8_t begin_read_header[4];
				begin_read_header[0] = 0x03;
				begin_read_header[1] = 0x00;
				begin_read_header[2] = 0x00;
				begin_read_header[3] = 0x00;
				// Now we go to reading in the cart data... But WE CANT do this over dma because
				//     we arent just transmitting the same byte
				// Unfortunately this does NEED to be in this callback, but hopefully its fine
				HAL_SPI_Transmit(&flash_spi_handle, begin_read_header, 4, HAL_MAX_DELAY);

				HAL_SPI_TransmitReceive_DMA(&flash_spi_handle, &dummy_byte,
						cart_header_buf, 8);
			}
		}
		else if (cart_read_state == READING_IN_ROM_HEADER_STATE)
		{
			// Done reading the cart header, evaluate it and
			//    move to the next
			if (processCartHeader(&cartidges_slots_to_read_into[cart_to_read_ind],
				cart_header_buf, cart_to_read_ind))
			{
				cart_read_state = READING_IN_ROM_STATE;
				HAL_SPI_TransmitReceive_DMA(&flash_spi_handle, &dummy_byte,
					cartidges_slots_to_read_into[cart_to_read_ind].cart_rom,
					cartidges_slots_to_read_into[cart_to_read_ind].num_bytes);
			}
			else
			{
				cart_nss_deactivate(cart_to_read_ind);
				cart_read_state = READY_TO_POLL_STATE;
				cartidges_slots_to_read_into[cart_to_read_ind].data_initialized = false;
				inc_cart_to_read_ind(&cart_to_read_ind);
			}
		}
		else if (cart_read_state == READING_IN_ROM_STATE)
		{
			cart_nss_deactivate(cart_to_read_ind);
			cartidges_slots_to_read_into[cart_to_read_ind].data_initialized = true;
			inc_cart_to_read_ind(&cart_to_read_ind);
			cart_read_state = READY_TO_POLL_STATE;
		}
	}
}

