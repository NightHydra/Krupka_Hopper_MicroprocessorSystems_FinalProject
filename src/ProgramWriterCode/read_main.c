#include "init.h"
#include "spi_flash_interface.h"
#include "SamplePrograms/Simple_test/ApplicationMain.h"
#include "cartridge_object.h"
#include "stdbool.h"


/*=========================================================================
 * DEFINES
 ========================================================================== */

#define SWITCH_GAME_BUTTON_PORT (GPIOA)
#define SWITCH_GAME_BUTTON_PIN (GPIO_PIN_0)

#define STACK_SPACE_NEEDED_FOR_MAIN (1000)
#define STACK_SPACE_FOR_OS_CALL (100)
#define STACK_SPACE_NEEDED_PER_FUNCTION (500)

#define MAX_NUMBER_OF_EXTERNAL_PROGRAMS (3)


/** =======================================================================
 * Global Variables
 ========================================================================== */

uint32_t base_sp;
uint32_t sp_start_for_function;

uint8_t prog_index = MAX_NUMBER_OF_EXTERNAL_PROGRAMS-1;



uint8_t data_to_write[] = "To catch them is my real test, to train them is my cause";
uint8_t read_buf[256] = {0};

uint8_t num_to_inc1 = 0;
uint8_t num_to_inc2 = 0;


//#define ERASE
#define WRITEx
#define READ


static cartridge_t my_carts[3];

void (* myFunc1)(uint8_t *, uint8_t);
void (* myFunc2)(uint8_t *, uint8_t);
void (* myFunc3)(uint8_t *, uint8_t);

DMA_HandleTypeDef DMAHandle;
void InitDMA()
{
	__HAL_RCC_DMA2_CLK_ENABLE();
	__HAL_RCC_SYSCFG_CLK_ENABLE();


	DMAHandle.Instance = DMA2_Stream0;
	DMAHandle.Init.Channel = DMA_CHANNEL_0;
	DMAHandle.Init.Direction = DMA_MEMORY_TO_MEMORY;
	DMAHandle.Init.PeriphInc = DMA_PINC_DISABLE;
	DMAHandle.Init.MemInc = DMA_MINC_DISABLE;
	DMAHandle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	DMAHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	DMAHandle.Init.Mode = DMA_CIRCULAR;
	DMAHandle.Init.Priority = DMA_PRIORITY_VERY_HIGH;				// Table 9.7 Textbook
	DMAHandle.Init.FIFOMode = DMA_FIFOMODE_ENABLE;				//
	DMAHandle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;		// Table 9.12 Textbook
	DMAHandle.Init.MemBurst = DMA_MBURST_SINGLE;				// Table 9.13 Textbook
	DMAHandle.Init.PeriphBurst = DMA_MBURST_SINGLE;
	//DMAHandle.XferHalfCpltCallback = NULL;
	//DMAHandle.XferErrorCallback = NULL;


	//HAL_DMA_Start(&DMAHandle, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);
	HAL_DMA_Init(&DMAHandle);

	//HAL_DMA_RegisterCallback(&hdma, HAL_DMA_XFER_CPLT_CB_ID, &DMATransferComplete);
}


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

void SimpleOS();
void safeFuncCall();

int main(void){

	Sys_Init();
	InitDMA();

	// Read the README in the base directory of this project.
	spi_flash_interface_initialize_SPI();

	switchGameButtonInit();

	//inc(&x);

	spi_flash_begin_autonomous_reads(my_carts);

	// Store the "base stack pointer" in this variable
	asm("STR r13, %0" : "=m" (base_sp));

	for (uint8_t i = 0; i<3; ++i)
	{
		my_carts[i].data_initialized = false;
	}


	myFunc1 = (void (*) (uint8_t *, uint8_t) )(my_carts[0].cart_rom+1);
	myFunc2 = (void (*) (uint8_t *, uint8_t) )(my_carts[1].cart_rom+1);
	myFunc3 = (void (*) (uint8_t *, uint8_t) )(my_carts[2].cart_rom+1);

	printf("DONE\r\n");



	while(1)
	{
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

void EXTI0_IRQHandler()
{
	//HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);

	EXTI->SWIER |= 1;
	EXTI->PR |= 1;

	//NVIC_ClearPendingIRQ(EXTI0_IRQn);

	printf("Num1 : %d, NUm2 : %d \r\n", num_to_inc1, num_to_inc2);

	uint8_t * goto_ptr = ((uint8_t *)(justCallTheOS));
	uint32_t addr = (uint32_t)(goto_ptr);

	//asm volatile ("STR r14, %0" : addrToCopyTo);


	//asm volatile("MOV r14, #0xFFFFFFF9 ");
	asm volatile("STR %0, [r13, #32]" : "+r" (addr));

}

void HAL_GPIO_EXTI_Callback(uint16_t pin)
{

}

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
	prog_index_inc(&prog_index);

	while (!my_carts[prog_index].data_initialized)
	{
		prog_index_inc(&prog_index);
	}

	uint32_t new_sp_loc;
	if (prog_index == 0)
	{
		// Adjust the stack pointer and go into the function
		new_sp_loc = (base_sp - STACK_SPACE_NEEDED_FOR_MAIN - STACK_SPACE_FOR_OS_CALL);
		asm volatile ("MOV sp, %0" :
				"+r" (new_sp_loc));
		// Now call the function
		myFunc1(&num_to_inc1, 1);
	}

	else if (prog_index == 1)
	{
		new_sp_loc = base_sp - STACK_SPACE_NEEDED_FOR_MAIN - STACK_SPACE_FOR_OS_CALL -
				STACK_SPACE_NEEDED_PER_FUNCTION;
		asm volatile ("MOV sp, %0" :
				"+r" (new_sp_loc));
		myFunc2(&num_to_inc2, 2);
	}

	else if (prog_index == 2)
	{
		new_sp_loc = base_sp - STACK_SPACE_NEEDED_FOR_MAIN - STACK_SPACE_FOR_OS_CALL -
			2*STACK_SPACE_NEEDED_PER_FUNCTION;
		asm volatile ("MOV sp, %0" :
			"+r" (new_sp_loc));
		myFunc3(&num_to_inc2, -1);
	}


}


