#include "init.h"
#include "spi_flash_interface.h"
#include "SamplePrograms/Simple_test/ApplicationMain.h"

uint8_t data_to_write[] = "To catch them is my real test, to train them is my cause";
uint8_t read_buf[256] = {0};

//#define ERASE
#define WRITEx
#define READx



static void inc(uint8_t * a, uint8_t x)
{
	(*a) +=  x;
}

static uint8_t func[200];

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


#ifdef WRITE
	spi_flash_write_function(0x00, 200, (uint8_t *) (appFrame-1));
#endif

#ifdef READ
	spi_flash_read_page(func, 200, 0x00);

	void (*myFunc)(uint8_t * , uint8_t );
	myFunc = (void (*)(uint8_t *, uint8_t )) (func-1);
#endif

	while(1)
	{
#ifdef READ

		appFrame(&x, 1);
		myFunc(&x, 2);

		printf("%d\r\n", x);

#endif
		inc(&x, 2);
	}
}
