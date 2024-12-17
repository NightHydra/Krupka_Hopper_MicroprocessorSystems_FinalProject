/*
 * cartridge_object.c
 *
 *  Created on: Dec 17, 2024
 *      Author: Alek
 */

#include "cartridge_object.h"

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
