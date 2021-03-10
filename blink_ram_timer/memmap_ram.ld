/* SPDX-License-Identifier: MIT */
MEMORY
{
      RAM(rwx) : ORIGIN =  0x20000000, LENGTH = 256k
}

ENTRY(reset)

SECTIONS
{
    .text : {
        *(.vectors)
    	*(.text*)
    } > RAM

    .bss : {
        __bss_start__ = .;
	*(.bss*)
	. = ALIGN(4);
	__bss_end__ = .;
    } > RAM
}
