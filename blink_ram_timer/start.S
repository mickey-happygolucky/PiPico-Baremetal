/* SPDX-License-Identifier: MIT */
	.cpu cortex-m0plus
	.thumb

/* vector table */
	.section .vectors, "ax"
	.align 2
	.global __vectors
__vectors:
.word 0x20001000
.word reset
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word isr_timer0

/* reset handler */
	.thumb_func
	.global reset
reset:
	ldr r1, =0xE000ED08 /* VTOR */
	ldr r0, =__vectors
	str r0, [r1]

	ldr r0, =0x20001000
	mov sp, r0
	bl init_bss
	bl init_clocks
	bl main
	b hang

.thumb_func
hang:	b .
