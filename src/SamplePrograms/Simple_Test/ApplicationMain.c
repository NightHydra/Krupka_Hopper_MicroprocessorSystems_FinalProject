/*
 * ApplicationMain.c
 *
 *  Created on: Dec 13, 2024
 *      Author: Alek
 */

#include "SamplePrograms/Simple_Test/ApplicationMain.h"

/** Test out statics in this file to see whether programs still work with it */

static uint32_t somevar = 2;

static uint32_t toIncBy = 1;


void appFrame(void * arg)
{
	uint32_t * numArg = (uint32_t *) arg;
	while (1)
	{
		*numArg += toIncBy;
		mult2(numArg);
	}
}

void mult2(uint32_t * a)
{
	*a /= somevar;
}




