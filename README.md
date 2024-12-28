# Overview

This application is really two applications split into one.

## Application 1 - Writing programs to flash memory

The first build configuration allows for users to write a custom program to a flash memory chip instruction by instruction.  One major limitation is that IRQ handlers are not copied so code may not use interrupt functionalitity not specified by the kernel.  Additionally, a special header will be written to the flash memory to indicate key information.

## Application 2 - Reading a running programs written to flash memory

The second build configuration allows users to read the prewritten programs and run them.  Multiple memory chips can be read from to have multiple programs running at once.  This application attempts to read from the memory chip for each slot once every few seconds.  Pressing a push-button on hardware will switch which program is running. Pressing the push-button triggers a hardware interrupt so this can be easily changed to accomodate for a timer interrupt.

# Hardware Specifications

## Microcontroller Usage

Both build configurations are meant to work with the [32F769IDISCOVERY Board](https://www.st.com/en/evaluation-tools/32f769idiscovery.html#overview).  This code may work with other microcontrollers but as kernel functionality exapnds, certain features may be restricted on other boards due to board-specific hardware.

## Flash Memory Chips

This project works specfically with the [W25Q32JV 32M-BIT SPI flash memory chip](https://mm.digikey.com/Volume0/opasdata/d220001/medias/docus/5059/W25Q32JV_RevI_5-4-21.pdf).  The communication protocol is written to send messages specifically to communicate with this chip so a substitute must also use the same mode of communication.

# Key Terms

1. SPI (Serial Peripheral Interface) - A Master/Slave full-duplex serial communication protocol.
2. DMA (Direct Memory Acess) - Specialized Hardware on a microcontroller that allows to data transfers that do not imvolve the CPU.
3. Kernel - The part of the main OS that allows for applications to access specific hardware.  This is needed so applications have pre-defined configurations for and pre-initialized peripherals.

# Key Features
1. Uses SPI communication protocol for fast data reads.
2. SPI usage allows to multiple cartridges to be connected on the same BUS.
3. Data is read from the flash chip using DMA meaning processes can still run efficiently during polling or reading in a program.

# Compilation Instructions

## Overview 
This project is made to compile using the free [STM Cube IDE] (https://www.st.com/en/development-tools/stm32cubeide.html#get-software).  This project also requires an STM HAL (Hardware Abstraction Library) in order to properly function.  This library can be downloaded [here](https://github.com/STMicroelectronics/STM32CubeF7).  Make sure that this is in the same folder as the cloned project is located.

## Building the project to run in write mode (which writes a program to flash memory)

Follow the instructions below

1. Click ```Run``` in the toolbar
2. Click ```Debug Configurations```
3. Select the .elf file from the WriteDebug folder under the ```C/C++ Application``` header.
4. Click Debug

## Building the project to run in read mode

1. Use the instructions from the write debug section, but use the .elf file from the read debug folder.
2. Put any custom application code to write into the ```SamplePrograms``` Folder.  If another folder is desired, replace all instances of ```SamplePrograms``` in ```LinkerScript.ld``` with the desired folder name.
3. Import any header files for the custom code files into ```write_main.c```
4. Replace the value for the #define of ```BEGINNING_FUNCTION``` with the "main" function of the custom application.  NOTE: The main function may not actually be named "main" in the custom code file as this would be confused by the compiler for the main function of the build.


# How to contribute

To contibute code to this project submit a pull request to the main branch.  If unit tests are added as a feature to this project, make sure all required unit tests pass and new tests are added to provide coverage of added code.

# Future Improvements

1. Implementing a kernel.  This is used to provide applications with pre-initialized hardware that can be used on the microcontroller.  This is needed for using the on-board DAC, ADC etc.  This is needed incase multiple applications are running that all use the same hardware.  In this case a kernel is needed to control access to this hardware so configurations do not contradict each other.  A few kernel functionality features are listed below.  NOTE : All kernel functions MUST use hardware in their respective DMA modes to minimize processing power used for the kernel functionality.
    1. Diagnostic Printing - the printf functionality uses the UART hardware on the board.  Giving all custom applications access to this is of highest priority.
    1. Receiving diagnostic data - Implementing UART receive functionality should be added to the kernel for applications to receive data via a USB cable.
    1. Screen Functionality - Added functions to allow applications to write data to the screen will be added.
    1. USB and HID functionality - We would like to add functionality for applications to implement handling for an external keyboard.

1. Ensuring the Linker Script is modified sufficiently from the default.
1. Adding Unit Tests
    1. Unit tests should be added so the functionality of new code can be verified.

# Contributors

1. Alek Krupka
1. Blake Hopper

