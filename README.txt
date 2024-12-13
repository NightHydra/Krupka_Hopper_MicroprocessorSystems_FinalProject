This project is a base project for the MPS Final Project.  
The configuration of the project is similar to Lab 1.  
In order to modify for your application, you will need to enable
all the peripherals you desire, minus the defaults here.  To do so,
you need to perform two steps.  

1) In the file inc/stm32f7xx_hal_conf.h, uncomment any line that applies
to your desired peripherals. For example, for I2C, uncomment:
	// #define HAL_I2C_MODULE_ENABLED
2) In the stm32lib/STM32F7xx_HAL_Driver/Src folder, un-exclude any
applicable source file for your desired peripherals.  Note that you
generally do not need the low-level (LL_*) or Extension (*_ex.c)
files, but you may.

Additionally, In this project, you are provided additional libraries
within the Libraries/ folder.  To use these libraries, you have two options:

1. copy the applicable *.h files into the inc/ folder and *.c files into the
src/ folder.  You'll have to do this iteratively as you identify more files
that are related to the desired .h/.c. Further, you may have to change #include
paths as you move files.

2. Unexclude the applicable folders. This will have the side-effect of including
more files than desired. These may be either ignored, deleted, excluded, or moved
depending on your desired approach. If ignored, more HAL support will need to 
be enabled to allow the project to build

In either case, many HAL peripherals will likely need to be enabled. Follow the
procedure above to enable those as identified.


Note: In the BSP library, there is LCD support libraries (among other things).

For using the LCD on the B03 revision DISCO boards, you need to add the
predefined symbol "USE_STM32F769I_DISCO_REVB03" to the project. The revision #
is found underneath the LCD on a white sticker which may be seen at an angle. One
line on the sticker will end in the revision #, such as: "MB1225-F769I-B03". Note
that this extra symbol is not needed for other revisions.

To add the "USE_STM32F769I_DISCO_REVB03" symbol to the project:
   go to Project Properties -> C/C++ Build -> Settings -> MCU GCC Compiler -> Preprocessor
       Click the "Add..." button on the top right and add the define value
