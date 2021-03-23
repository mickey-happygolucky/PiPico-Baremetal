# SPDX-License-Identifier: MIT
CROSS_COMPILE ?= arm-none-eabi-

MCPU = -mcpu=cortex-m0plus

ASMOPT = $(MCPU) -g
COPT = $(MCPU) -ffreestanding -g -O0
LOPT = -nostdlib -nostartfiles

all: led.uf2

clean:
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.uf2
	rm -f *~

start.o: start.S
	$(CROSS_COMPILE)as $(ASMOPT) start.S -o start.o

boot2.o: boot2/bs2_default_padded_checksummed.S
	$(CROSS_COMPILE)as $(ASMOPT) boot2/bs2_default_padded_checksummed.S -o boot2.o

main.o: main.c
	$(CROSS_COMPILE)gcc $(COPT) -fpic -mthumb -c main.c -o main.o

led.elf: start.o boot2.o main.o
	$(CROSS_COMPILE)ld $(LOPT) start.o boot2.o main.o -T memmap.ld  -o led.elf 

led.uf2: led.elf
	elf2uf2 led.elf led.uf2
