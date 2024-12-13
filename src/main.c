#include "init.h"
#include "spi_flash_interface.h"

int main(void){

	Sys_Init();

	// Read the README in the base directory of this project.
	spi_flash_interface_initialize_SPI();

	while(1)
	{
		//uint8_t out = spi_flash_read_status_register();

		//printf("%d\r\n", out);
	}
}
