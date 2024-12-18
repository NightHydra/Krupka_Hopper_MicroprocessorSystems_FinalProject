/*
 * cartridge_object.c
 *
 *  Created on: Dec 17, 2024
 *      Author: Alek
 */

#include "cartridge_object.h"
#include "stdbool.h"

bool ensure_device_id_is_still_the_same(cartridge_t const * const cart, uint8_t const cart_slot_ind)
{
	return true;
}

bool init_flash_cartridge(cartridge_t * const cart, uint8_t cart_slot_ind)
{
	return true;
}

bool cart_ids_match(cartridge_t const * const the_cart, uint8_t * read_id)
{
	uint8_t * cart_id = the_cart->cart_unique_id;

	for (unsigned int i = 0; i < 8; ++i)
	{
		if (cart_id[i] != read_id[i]) return false;
	}
	return true;
}

/**
 * @param hdr should be a uint8_t pointer to an array of 8 bytes
 *    where the first 4 bytes represent the size of the cart
 *    data and the second represents the offset of where the first
 *    function is in memory.
 */
bool processCartHeader(cartridge_t * const cart_ptr,
	uint8_t const * const hdr)
{
	for (uint8_t i = 0; i<4; ++i)
	{
		cart_ptr->num_bytes |= hdr[i] << (8 * (3-i));
		cart_ptr->mainoffset |= hdr[4+i] << (8 * (3-i));
	}
	printf("Cart header read: (size = %ld), (main offset = %d)\r\n",
		cart_ptr->num_bytes, cart_ptr->mainoffset);

	if (cart_ptr->mainoffset > cart_ptr->num_bytes) return false;

	return true;
}
