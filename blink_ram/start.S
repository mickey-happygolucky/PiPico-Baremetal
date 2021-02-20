/* SPDX-License-Identifier: MIT */
	.cpu cortex-m0plus
	.thumb

	.global reset
.thumb_func
reset:
	ldr r0, =0x20001000
	mov sp, r0
	bl main
	b hang
	
.thumb_func
hang:	b .
