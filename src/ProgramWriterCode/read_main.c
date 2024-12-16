#include "init.h"
#include "spi_flash_interface.h"
#include "SamplePrograms/Simple_test/ApplicationMain.h"


/*=========================================================================
 * DEFINES
 ========================================================================== */

#define SWITCH_GAME_BUTTON_PORT (GPIOA)
#define SWITCH_GAME_BUTTON_PIN (GPIO_PIN_0)

#define STACK_SPACE_NEEDED_FOR_MAIN (3000)
#define STACK_SPACE_NEEDED_PER_FUNCTION (1000)

/** =======================================================================
 * Global Variables
 ========================================================================== */

uint32_t base_sp;
uint32_t sp_start_for_function;



uint8_t data_to_write[] = "To catch them is my real test, to train them is my cause";
uint8_t read_buf[256] = {0};

uint8_t num_to_inc1 = 0;

//#define ERASE
#define WRITEx
#define READ


static uint8_t func[200];


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

int main(void){

	Sys_Init();

	// Read the README in the base directory of this project.
	spi_flash_interface_initialize_SPI();

	switchGameButtonInit();

	//inc(&x);

	spi_flash_read_page(func, 200, 0x00);

	asm("STR r13, %0" : "=m" (base_sp));
	sp_start_for_function = base_sp - STACK_SPACE_NEEDED_FOR_MAIN;

	void (*myFunc)(uint8_t * , uint8_t );
	myFunc = (void (*)(uint8_t *, uint8_t )) (func-1);

	volatile uint8_t p = 4;
	volatile uint8_t q = 7;

	printf("%x\r\n", base_sp);

	myFunc(&num_to_inc1, p);

	printf("DONE\r\n");
	while(1)
	{




	}
}

/* ======================================================================
 * HELPER FUNCTION DEFINITIONS
 ======================================================================== */

void EXTI0_IRQHandler()
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
	if (pin == GPIO_PIN_0)
	{
		printf("%d\r\n", num_to_inc1);
	}
}
