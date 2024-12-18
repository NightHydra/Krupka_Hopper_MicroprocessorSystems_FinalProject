/*
 * ApplicationMain.c
 *
 *  Created on: Dec 13, 2024
 *      Author: Alek
 */

#include "SamplePrograms/Simple_Test/ApplicationMain.h"

static uint32_t somevar = 2;

void appFrame(uint8_t * a, uint8_t x)
{
	while (1)
	{
		(*a) += x;
		mult2(a);
	}
}

void mult2(uint8_t * a)
{
	*a *= somevar;
	++somevar;
}


