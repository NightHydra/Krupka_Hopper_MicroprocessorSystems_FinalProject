/*
 * ApplicationMain.c
 *
 *  Created on: Dec 13, 2024
 *      Author: Alek
 */

#include "SamplePrograms/Simple_Test/ApplicationMain.h"


void appFrame(uint8_t * a, uint8_t x)
{
	while (1)
	{
		(*a) += x;
	}
}
