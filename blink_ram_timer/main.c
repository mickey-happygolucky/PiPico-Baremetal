/* SPDX-License-Identifier: MIT */
#include "regs.h"
#include <stdint.h>
#include <stdbool.h>

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

void init_xosc() {
        write_reg(XOSC_CTRL, 0xaa0); /* 1_15MHZ */
        write_reg(XOSC_STARTUP, 47); /* (((12 * MHZ) / 1000) + 128) / 256 = 47 */
        write_reg_op(XOSC_CTRL, 0xfab << 12, OP_SET); /* ENABLE */

        /* Wait for XOSSC to be stable */
        while (!(read_reg(XOSC_STATUS) & 0x80000000)) ;
}

void init_clocks() {
        /*  Disable resus that may be enabled from previous software */
        write_reg(CLOCKS_CLK_SYS_RESUS_CTRL, 0);

        init_xosc();

        /* source of clk_ref to xosc */
        write_reg_op(CLOCKS_REF_CTRL, 0x2, OP_SET);

        /* reset the source of clk_sys to clk_ref which references xosc */
        write_reg(CLOCKS_SYS_CTRL, 0x0);
}

void irq_set_enabled(uint32_t irq, bool enabled) {
        uint32_t mask = (1<<irq);

        if (enabled) {
                write_reg(NVIC_ICPR, mask);
                write_reg(NVIC_ISER, mask);
        } else {
                write_reg(NVIC_ICER, mask);
        }
}

void init_timer() {
        /* reset TIMER */
        write_reg_op(RESETS_RESET, (1<<21), OP_CLR);
        /* wait for reset TIMER to be done */
        while (!read_reg(RESETS_RESET_DONE)&(1<<21)) ;

        /* Enable timer interrupt */
        write_reg_op(TIMER_INTE, 1, OP_SET);

        /* Enable IRQ */
        irq_set_enabled(0, true); /* TIMER_IRQ_0=0 */
}

void isr_timer0() {
        /* Clear the timer0 irq */
        write_reg_op(TIMER_INTR, 1, OP_CLR);
}

void sleep_us(uint32_t us) {
        uint32_t wakeup = read_reg(TIMER_TIMERRAWL);
        wakeup += us;
        write_reg(TIMER_ALARM0, wakeup);

        /* Wait For Interupt */
        __asm__ __volatile__("wfi");
}

// us is up to max of 24bit (0xffffff)
void delay_us(uint32_t us) {
        write_reg(SYSTICK_RVR, us);
        write_reg(SYSTICK_CVR, 0);

        // wait for 0 to RVR
        while (!read_reg(SYSTICK_CVR)) __asm__ __volatile__("");

        // wait for RVR counting down to 0.
        while (read_reg(SYSTICK_CVR)) __asm__ __volatile__("");
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

        // Timer0 init
        init_timer();

        // Blink
        write_reg(SIO_GPIO_OE_SET, (1ul<<25));
        while (1) {
                sleep_us(100*1000); //100ms
                write_reg(SIO_GPIO_OUT_XOR, (1ul<<25));
        }
        return 0;
}
