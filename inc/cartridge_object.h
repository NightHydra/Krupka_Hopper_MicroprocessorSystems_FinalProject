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


#define CARTRIDGE_ROM_BUFFER_SIZE (255)
#define CARTRIDGE_SAVE_DATA_SIZE (1)

typedef struct
{
	uint8_t cart_rom[CARTRIDGE_ROM_BUFFER_SIZE];
	uint32_t num_bytes;

	uint8_t cart_unique_id[8];

	bool data_initialized;

	uint8_t cart_save_data[CARTRIDGE_SAVE_DATA_SIZE];

} cartridge_t;

/**
 * @brief This function takes in an already initialized cartridge and checking to make sure that the
 *     cartridge is still plugged in.  If it is not, then we know to switch it
 * @param cart is a pointer to the cartridge "object" in being used to store the cartridge data and
 *    other cartridge related data
 * @param cart_slot_ind is the "slot" (which NSS pin it is connected to) index
 * @returns returns true if the device id is still the same and false if the device id is not the same
 *
 */
bool ensure_device_id_is_still_the_same(cartridge_t const * const cart, uint8_t const cart_slot_ind);

bool init_flash_cartridge(cartridge_t * const cart, uint8_t cart_slot_ind);

bool cart_ids_match(cartridge_t const * const the_cart, uint8_t * read_id);




#endif /* INC_CARTRIDGE_OBJECT_H_ */
