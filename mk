#!/bin/bash


rm main.d main.elf main.o

avr-g++ -std=c++11 -c -g -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega328 -DF_CPU=16000000L -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=105 -D__PROG_TYPES_COMPAT__ -I/usr/share/arduino/hardware/arduino/cores/arduino -I/usr/share/arduino/hardware/arduino/variants/standard main.cpp -I/usr/lib/gcc/avr/4.8.2/include/

avr-gcc -Os -Wl,--gc-sections -mmcu=atmega328 -o main.elf main.o -lm

avr-objcopy -O ihex -R .eeprom main.elf main.hex

sudo avrdude -c avrisp2 -p atmega328 -u -U flash:w:main.hex

