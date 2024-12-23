/*
 * fib.c
 *
 *  Created on: Dec 19, 2024
 *      Author: Alek
 */

#include "SamplePrograms/Fib_Test/fib.h"

/**
 * @brief Simple fibonachi function to test stuff out
 */
void fib(void * fibNum)
{
	uint32_t n1 = 0;
	uint32_t n2 = 1;

	uint32_t * fNum = (uint32_t*)(fibNum);

	while (1)
	{
		*fNum = n1+n2;
		n1 = n2;
		n2 = *fNum;

	}
}
