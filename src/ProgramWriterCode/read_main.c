#include "init.h"
#include "spi_flash_interface.h"
#include "SamplePrograms/Simple_test/ApplicationMain.h"
#include "cartridge_object.h"
#include "stdbool.h"


/*=========================================================================
 * DEFINES
 ========================================================================== */


/**
 * @brief Port and Pin defines for the on board push button
 */
#define SWITCH_GAME_BUTTON_PORT (GPIOA)
#define SWITCH_GAME_BUTTON_PIN (GPIO_PIN_0)

/**
 * Defines for how much stack space is needed for main,
 *     the call for @ref SimpleOS and the stack space needed for each function
 */
#define STACK_SPACE_NEEDED_FOR_MAIN (1000)
#define STACK_SPACE_FOR_OS_CALL (100)
#define STACK_SPACE_NEEDED_PER_FUNCTION (500)


/**
 * @brief Define for the max number of cartridges
 * @todo This should probably be in a better location
 */
#define MAX_NUMBER_OF_EXTERNAL_PROGRAMS (3)


/** =======================================================================
 * Global Variables
 ========================================================================== */

/**
 * @brief Stores the starting memory addresses for the different stack pointers
 */
uint32_t base_sp;
uint32_t sp_start_for_function;

/**
 * @brief Start the program index at the last one since
 *     it will increment by 1
 */
uint8_t prog_index = MAX_NUMBER_OF_EXTERNAL_PROGRAMS-1;

/**
 * @brief Testing variables for showing how the OS works
 */
static uint32_t opnum1 = 0;
static uint32_t opnum2 = 0;


/**
 * @brief Memory for the cartridge representations
 */
static cartridge_t my_carts[3];

/**
 * @brief Global variables for each of the cartidge functions to call
 */
void (* myFunc1)(void *);
void (* myFunc2)(void *);
void (* myFunc3)(void *);


/**
 * @brief Initialize the push button on the front of the board in interrupt mode
 *    and enable the interrupt via NVIC
 */
void switchGameButtonInit()
{
	GPIO_InitTypeDef switch_game_button_handler = {0};

	switch_game_button_handler.Pin = SWITCH_GAME_BUTTON_PIN;
	switch_game_button_handler.Mode = GPIO_MODE_IT_FALLING;
	switch_game_button_handler.Pull = GPIO_PULLDOWN;
	switch_game_button_handler.Speed = GPIO_SPEED_HIGH;

	HAL_GPIO_Init(SWITCH_GAME_BUTTON_PORT, &switch_game_button_handler);

	// Enable the Interrupt
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

/**
 * @brief The function for the OS logic
 * @note Most of the time this will never return
 */
void SimpleOS();

int main(void){

	Sys_Init();

	// Start the continuous poll of the carts via DMA
	spi_flash_interface_initialize_SPI();

	// Initialize the button
	switchGameButtonInit();


	// Store the "base stack pointer" in this variable
	asm("STR r13, %0" : "=m" (base_sp));


	// Make sure the carts are considered uninitialized so
	//     it doesnt try to call a bad function
	for (uint8_t i = 0; i<3; ++i)
	{
		my_carts[i].data_initialized = false;
	}


	while(1)
	{
		// Just call the OS
		SimpleOS();
	}
}

/* ======================================================================
 * HELPER FUNCTION DEFINITIONS
 ======================================================================== */

void justCallTheOS()
{

	uint32_t new_sp_location = (base_sp - STACK_SPACE_NEEDED_FOR_MAIN);
	asm volatile("MOV sp, %0" : "+r" (new_sp_location));

	SimpleOS();
}


/**
 * Dont even try calling the callback here.  Just clear the flags
 *    since we also have to worry about changing the program counter
 *    in the exception frame
 */
void EXTI0_IRQHandler()
{

	EXTI->SWIER |= 1;
	EXTI->PR |= 1;

	// Testing code
	printf("Num1 : %lu\r\n", opnum1);

	// Calculate the address of where we should return to

	uint8_t * goto_ptr = ((uint8_t *)(justCallTheOS));
	uint32_t addr = (uint32_t)(goto_ptr);

	//asm volatile ("STR r14, %0" : addrToCopyTo);


	// In the following line of code we move the address for the beginning
	//     of "JustCallTheOs" into a specific location of memory on the stack.
	// The specific word we are writing to is the location of the return program
	//    counter in the exception frame.  When the exception frame is popped back
	//    into all of the registers what is popped into the program counter
	//    is 32 bytes up the stack frame from where we are now.  This is because
	//    through experimentation, this function pushes 8 bytes onto the register
	//    and the return pc is
	asm volatile("STR %0, [sp, #32]" : "+r" (addr));

}


/**
 * @brief Helper function for incrementing which cart to call the function from
 * @note This handles reseting the index back to 0 which can eat up a lot of space
 *    and clog up functions so having the helper is nice
 */
void prog_index_inc(uint8_t * ind)
{
	++(*ind);
	if (*ind == MAX_NUMBER_OF_EXTERNAL_PROGRAMS)
	{
		*ind = 0;
	}
}

void SimpleOS()
{
	// Read in the next program, or go to the next flash cart that is
	//     initialized
	prog_index_inc(&prog_index);

	while (!my_carts[prog_index].data_initialized)
	{
		prog_index_inc(&prog_index);
	}

	// Location of where we should set the stack pointer to.
	// We need to give each function its own stack since if we ever want
	//     full context switching we need this since we dont want the stacks
	//     interfering with each other
	uint32_t new_sp_loc;
	if (prog_index == 0)
	{
		printf("Process 1 Playing \r\n");
		// Adjust the stack pointer and go into the function
		new_sp_loc = (base_sp - STACK_SPACE_NEEDED_FOR_MAIN - STACK_SPACE_FOR_OS_CALL);

		// Use assembly to move the new stack pointer into the stack pointer register
		asm volatile ("MOV sp, %0" :
				"+r" (new_sp_loc));
		// Now call the function
		myFunc1 = (void (*) (void *) )
			(my_carts[0].cart_rom+my_carts[0].mainoffset+1);

		// And finally, call the function read on flash.
		// This might never return so the only way out is through an interrupt
		myFunc1(&opnum1);
	}

	else if (prog_index == 1)
	{
		printf("Process 2 Playing \r\n");
		new_sp_loc = base_sp - STACK_SPACE_NEEDED_FOR_MAIN - STACK_SPACE_FOR_OS_CALL -
				STACK_SPACE_NEEDED_PER_FUNCTION;
		asm volatile ("MOV sp, %0" :
				"+r" (new_sp_loc));
		myFunc2 = (void (*) (void *) )
			(my_carts[1].cart_rom+my_carts[1].mainoffset+1);
		myFunc2(&opnum1);
	}

	else if (prog_index == 2)
	{
		printf("Process 3 Playing \r\n");
		new_sp_loc = base_sp - STACK_SPACE_NEEDED_FOR_MAIN - STACK_SPACE_FOR_OS_CALL -
			2*STACK_SPACE_NEEDED_PER_FUNCTION;
		asm volatile ("MOV sp, %0" :
			"+r" (new_sp_loc));

		myFunc3 = (void (*) (void *) )
			(my_carts[2].cart_rom+my_carts[2].mainoffset+1);
		myFunc3(&opnum1);
	}
}


