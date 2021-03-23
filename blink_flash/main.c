/* SPDX-License-Identifier: MIT */
#include "regs.h"
#include <stdint.h>

uint32_t read_reg(uint32_t addr) {
        return (*((volatile uint32_t *)addr));
}

void write_reg(uint32_t addr, uint32_t value) {
        *((volatile uint32_t *)addr) = value;
}

void write_reg_op(uint32_t addr, uint32_t value, uint32_t op) {
        write_reg(addr | op, value);
}

int main(void) {

        /////////////////////
        // This initialise code is ported from https://github.com/dwelch67/raspberrypi-pico/blob/master/blinker00/notmain.c
        // release reset on IO_BANK0
        write_reg_op(RESETS_RESET, 1<<5, OP_CLR); //IO_BANK0
        //wait for reset to be done
        while(1) {
                if((read_reg(RESETS_RESET_DONE)&(1<<5))!=0) break;
        }
        write_reg_op(RESETS_RESET, (1<<8), OP_CLR); //PADS_BANK0
        while(1) {
                if((read_reg(RESETS_RESET_DONE)&(1<<8))!=0) break;
        }
        /////////////////////

        // GPIO init
        write_reg(SIO_GPIO_OE_CLR, (1ul<<25));
        write_reg(SIO_GPIO_OUT_CLR, (1ul<<25));
        uint32_t ra = read_reg(PADS_GPIO25);
        write_reg_op(PADS_GPIO25, (ra^0x40)&0xC0, OP_XOR);

        write_reg(IO_GPIO25_CTRL, 0x5);
        uint32_t v = read_reg(IO_GPIO25_CTRL);

        // Blink
        write_reg(SIO_GPIO_OE_SET, (1ul<<25));
        while (1) {
                for (int i = 100000; i != 0; i--) ;
                write_reg(SIO_GPIO_OUT_XOR, (1ul<<25));
        }
        return 0;
}
