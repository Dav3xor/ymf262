#!/bin/bash

avr-g++ -c -g -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega328 -DF_CPU=16000000L -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=105 -D__PROG_TYPES_COMPAT__ -I/usr/share/arduino/hardware/arduino/cores/arduino -I/usr/share/arduino/hardware/arduino/variants/standard test.c

avr-gcc -Os -Wl,--gc-sections -mmcu=atmega328 -o test.elf test.o -lm

avr-objcopy -O ihex -R .eeprom test.elf test.hex

sudo avrdude -c avrisp2 -p atmega328 -u -U flash:w:test.hex

