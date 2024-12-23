/*
 * CartridgeObject.h
 *
 *  Created on: Dec 17, 2024
 *      Author: Alek
 */

#ifndef INC_CARTRIDGE_OBJECT_H_
#define INC_CARTRIDGE_OBJECT_H_

#include "stdbool.h"
#include "init.h"


#define CARTRIDGE_ROM_BUFFER_SIZE (1000)
#define CARTRIDGE_SAVE_DATA_SIZE (4)

/**
 * @brief The representation of a cartridge to store all of the different attributes
 */
typedef struct
{
	/**
	 * @brief Stores the "ROM" of the flash memory cart
	 */
	uint8_t cart_rom[CARTRIDGE_ROM_BUFFER_SIZE];

	/**
	 * @brief Stores the size of the flash memory cart
	 */
	uint32_t num_bytes;

	/*
	 * @brief The address of the starting function on the ROM
	 */
	uint32_t mainoffset;

	/**
	 * This is the carts 8-bit ID.  It is stored as an array of uint8_t on the cart
	 *     and should be compared in that way to prevent issues with endianess
	 */
	uint8_t cart_unique_id[8];

	/**
	 * @brief This tells whether or not the cart has been initialized.  Should be
	 *     set to false before the data is read in.
	 */
	bool data_initialized;

	/**
	 * @brief This holds the save data of the program written on the cart.
	 * @todo : Saving data to and from the cart is not supported yet.  Add implementation
	 *     for it once save data is supported
	 */
	uint8_t cart_save_data[CARTRIDGE_SAVE_DATA_SIZE];

} cartridge_t;

/**
 * @brief compares the id stored in @ref the_cart and compares it to the 8 byte number
 *     represented by @ref read_id
 */
bool cart_ids_match(cartridge_t const * const the_cart, uint8_t * read_id);

/**
 * @brief Takes in a uint8_t array representing the cartidge header and the index
 *     of the cart, seeing if it is a valid cart and returns true if so.
 */

bool processCartHeader(cartridge_t * const cart_ptr,
    uint8_t const * const hdr, uint8_t const index);





#endif /* INC_CARTRIDGE_OBJECT_H_ */
