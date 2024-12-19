/*
 * Check1.c
 *
 *  Created on: Dec 19, 2024
 *      Author: hoppeb
 */

#include "SamplePrograms/Test2/Check1.h"
#include "hello.h"
#include "stm32f769xx.h"
#include <stdint.h>
#include <stdbool.h>

#define NUMBER_OF_LEDS_TO_CONTROL (4)

//  TSRQ PONM LKJI HGFE DCBA
#define GPIO_CLOCKS_TO_ENABLE_BITS_TO_SET (0x022D)

#define PIN_HIGH (1)
#define PIN_LOW (0)

// Some statically defined arrays
static GPIO_TypeDef * const ld_port_numbers[NUMBER_OF_LEDS_TO_CONTROL] =
    {GPIOJ, GPIOJ, GPIOA, GPIOD};

static uint8_t const ld_pin_numbers[NUMBER_OF_LEDS_TO_CONTROL] = {13,5,12,4};

static GPIO_TypeDef * const ard_port_numbers[NUMBER_OF_LEDS_TO_CONTROL] =
    {GPIOC, GPIOC, GPIOJ, GPIOF};

static uint8_t const ard_pin_numbers[NUMBER_OF_LEDS_TO_CONTROL] = {7, 6, 1, 6};

static bool const inverted_led[NUMBER_OF_LEDS_TO_CONTROL] = {false, false, false, true};

static uint8_t previous_input_data[NUMBER_OF_LEDS_TO_CONTROL] = {PIN_LOW, PIN_LOW, PIN_LOW, PIN_LOW};

uint8_t a = 1;

void checkapp(void * a){
		RCC->AHB1ENR |= GPIO_CLOCKS_TO_ENABLE_BITS_TO_SET;

		uint8_t i;

		for (i = 0; i<NUMBER_OF_LEDS_TO_CONTROL; ++i)
		{
			// Configure the LD pins in output mode
			ld_port_numbers[i]->MODER |= (0x0001U << (ld_pin_numbers[i]*2));
			ld_port_numbers[i]->MODER &= ~(0x0002U << (ld_pin_numbers[i]*2));

			// Configure the LD pins to be push-pull output
			ld_port_numbers[i]->OTYPER &= ~(0x0001 << ld_pin_numbers[i]);

			// Configure the LD pins to be low speed output
			ld_port_numbers[i]->OSPEEDR &= ~(0x0003U << (ld_pin_numbers[i]*2));

			// TOGGLE THE LED ON
			ld_port_numbers[i]->ODR |= (0x0001 << ld_pin_numbers[i]);


			// Configure the input pins
			ard_port_numbers[i]->MODER &= ~(0x0003U << (ld_pin_numbers[i]*2));

			// COnifigure the pins to be pull down for standard logic
			ard_port_numbers[i]->PUPDR |= (0x0002 << ld_pin_numbers[i]);

			// Reset the pin to off by default
			if (inverted_led[i] == false)
			{
				ld_port_numbers[i]->BSRR |= ((0x01 << 16) << ld_pin_numbers[i]);
			}
			else
			{
				ld_port_numbers[i]->BSRR |= (0x01 << ld_pin_numbers[i]);
			}
    }

    while(1)
    {

    	// Look at all the input register for each input
    	for (i = 0; i < NUMBER_OF_LEDS_TO_CONTROL; ++i)
    	{
    		uint8_t input_high = (
    				(ard_port_numbers[i]->IDR >> ard_pin_numbers[i]) & 0x01 );

    		if (input_high == previous_input_data[i])
    		{
    			// Dont update the LED since the input didnt change
    		}

    		else if ( (input_high != 0 && inverted_led[i] == false) ||
    				  (input_high == 0 && inverted_led[i] == true ) )
    		{
    			// Drive the pin high
    			ld_port_numbers[i]->BSRR |= (0x01 << ld_pin_numbers[i]);
    		}
    		else
    		{
    			ld_port_numbers[i]->BSRR |= ((0x01 << 16) << ld_pin_numbers[i]);
    		}
    		previous_input_data[i] = input_high;
    	}

    }
}
