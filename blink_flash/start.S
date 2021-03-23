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

/* reset handler */
	.thumb_func
	.global reset
reset:
	ldr r0, =0x20001000
	mov sp, r0
	bl main
	b hang
	
.thumb_func
hang:	b .
