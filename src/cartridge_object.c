/*
 * cartridge_object.c
 *
 *  Created on: Dec 17, 2024
 *      Author: Alek
 */

#include "cartridge_object.h"
#include "stdbool.h"

/**
 * @brief Checks to see if a cartridge's previous id is equivalent to the one just
 *     read in.  Useful for determining if the chip has been switched out or not.
 */
bool cart_ids_match(cartridge_t const * const the_cart, uint8_t * read_id)
{
	uint8_t const * cart_id = the_cart->cart_unique_id;

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
	uint8_t const * const hdr, uint8_t const index)
{
	for (uint8_t i = 0; i<4; ++i)
	{
		cart_ptr->num_bytes |= hdr[i] << (8 * (3-i));
		cart_ptr->mainoffset |= hdr[4+i] << (8 * (3-i));
	}
	printf("Cart header read in slot %d: (size = %ld), (main offset = %ld)\r\n",
		index+1, cart_ptr->num_bytes, cart_ptr->mainoffset);

	if (cart_ptr->num_bytes == 0xFFFFFFFF || cart_ptr->num_bytes == 0x00)

	{
		return false;
	}

	if (cart_ptr->mainoffset >= cart_ptr->num_bytes) return false;

	return true;
}
