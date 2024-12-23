#include "init.h"
#include "spi_flash_interface.h"
#include "SamplePrograms/Simple_test/ApplicationMain.h"
#include "SamplePrograms/Fib_Test/fib.h"
#include "SamplePrograms/Test2/Check1.h"


extern uint32_t _appbegin;
extern uint32_t _append;


#define BEGINNING_FUNCTION (fib)
#define CART_TO_WRITE_TO (0)

int main(void){

	Sys_Init();

	// Read the README in the base directory of this project.
	spi_flash_interface_initialize_SPI();

	// Calculate the starting memory address of the application
	//     based on the linker script variables.
	// The address actually needs to be taken here since the data is actually
	//     garbage and the starting location we are after is actually the address
	uint32_t app_begin_location = (uint32_t)(&_appbegin);
	uint32_t app_end_section = (uint32_t)(&_append);

	// Calculate the progam size
	uint32_t prog_size = app_end_section - app_begin_location;


	// If we conveniently make sure to copy the previous function then everything
	//     works out alright
	spi_flash_write_func_memory(0x00, prog_size, (uint8_t*) app_begin_location,
			CART_TO_WRITE_TO, (BEGINNING_FUNCTION-1) - app_begin_location);

	// And have a nice print statement saying it is done.
	printf("Wrote program of %ld bytes to cart %d\r\n", prog_size,
		CART_TO_WRITE_TO);

	while(1)
	{

	}
}
