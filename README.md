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

### Key Features
1. Uses SPI communication protocol for fast data reads.
2. SPI usage allows to multiple cartridges to be connected on the same BUS.
