#include "init.h"
#include "spi_flash_interface.h"
#include "SamplePrograms/Simple_test/ApplicationMain.h"

uint8_t data_to_write[] = "To catch them is my real test, to train them is my cause";
uint8_t read_buf[256] = {0};

//#define ERASE
#define WRITE
#define READx


uint8_t copyfunc[200];

int main(void){

	Sys_Init();

	// Read the README in the base directory of this project.
	spi_flash_interface_initialize_SPI();

	/**
	memcpy(func, inc-1, 1000);
	uint8_t x = 0;

	void (*myFunc)(uint8_t * );
	myFunc = (void (*)(uint8_t * )) (func-1);
	*/

	uint8_t x = 0;

	//inc(&x);

	// If we conveniently make sure to copy the previous function then everything
	//     works out alright
	memcpy(copyfunc, (uint8_t *)(appFrame-41), 200);

	spi_flash_write_function(0x00, 200, (uint8_t *) (appFrame-1));

	void (* myFunc)(uint8_t *, uint8_t);
	myFunc = (void (*)(uint8_t *, uint8_t )) (copyfunc+41);

	// UNCOMMENT THIS LINE TO SEE HOW RELATIVE ADDRESSES WORK
	myFunc(&x, 1);

	printf("WROTE FUNCTION\r\n");
	printf("%x\r\n", copyfunc);
	while(1)
	{

	}
}
