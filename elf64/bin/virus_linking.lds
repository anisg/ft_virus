ENTRY(_infect)
SECTIONS
{
   . = 0x100;
   .text : { *(.text) }
   .data : { *(.data) }
   .bss : { *(.bss) }
}
