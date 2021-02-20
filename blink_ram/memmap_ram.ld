/* SPDX-License-Identifier: MIT */
MEMORY
{
      RAM(rwx) : ORIGIN =  0x20000000, LENGTH = 256k
}

ENTRY(reset)

SECTIONS
{
    .text : {
    	*(.text*)
    } > RAM
}
