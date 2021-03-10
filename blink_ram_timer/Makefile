# SPDX-License-Identifier: MIT
CROSS_COMPILE ?= arm-none-eabi-

MCPU = -mcpu=cortex-m0plus

ASMOPT = $(MCPU) -g
COPT = $(MCPU) -ffreestanding -g -O0
LOPT = -nostdlib -nostartfiles

all: ram

clean:
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *~

start.o: start.S
	$(CROSS_COMPILE)as $(ASMOPT) start.S -o start.o

main.o: main.c
	$(CROSS_COMPILE)gcc $(COPT) -fpic -mthumb -c main.c -o main.o

ram: start.o main.o
	$(CROSS_COMPILE)ld $(LOPT) start.o main.o -T memmap_ram.ld  -o led.elf 

