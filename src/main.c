#include "init.h"
#include "spi_flash_interface.h"
#include "SamplePrograms/Simple_test/ApplicationMain.h"

uint8_t data_to_write[] = "To catch them is my real test, to train them is my cause";
uint8_t read_buf[256] = {0};

static uint8_t incSize = 0;

//#define ERASE
//#define WRITE
#define READ

void inc(uint8_t * a)
{
	(*a) += incSize;
}

static uint8_t func[1000];

int main(void){

	Sys_Init();

	// Read the README in the base directory of this project.
	spi_flash_interface_initialize_SPI();

	memcpy(func, inc-1, 1000);
	uint8_t x = 0;

	void (*myFunc)(uint8_t * );
	myFunc = (void (*)(uint8_t * )) (func-1);




	//inc(&x);


#ifdef ERASE
	spi_flash_erase_sector(0x00);
#endif

#ifdef WRITE
	spi_flash_write_page(data_to_write, sizeof(data_to_write), 0x300);
#endif

	while(1)
	{
#ifdef READ
		// spi_flash_read_page(read_buf, sizeof(data_to_write), 0x300);

		//printf("%d\r\n", spi_flash_read_status_register());

		++incSize;

		myFunc(&x);


		printf("%d\r\n", x);
#endif
	}
}
