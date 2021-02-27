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

void init_bss() {
        extern uint32_t __bss_start__;
        extern uint32_t __bss_end__;
        uint32_t *p = &__bss_start__;
        uint32_t *end = &__bss_end__;
        while (p != end) {
                *p++ = 0;
        }
}

void init_systick() {
        write_reg(SYSTICK_RVR, 1000-1); /* Systick interrupt will issue per 1ms. */
        write_reg(SYSTICK_CVR, 0);
        write_reg(SYSTICK_CSR, 0x3); /* TICKINT=1, ENABLE=1 */
}

static uint32_t tick_count = 0;
void isr_systick() {
        ++tick_count;
}

void delay_ms(uint32_t ms) {
        uint32_t expire = tick_count + ms;
        while (tick_count != expire)
                __asm__ __volatile__("");
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

        // SysTick init
        init_systick();

        // Blink
        write_reg(SIO_GPIO_OE_SET, (1ul<<25));
        while (1) {
                delay_ms(100); //100ms
                write_reg(SIO_GPIO_OUT_XOR, (1ul<<25));
        }
        return 0;
}
