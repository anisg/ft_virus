ENTRY(_infect)
	SECTIONS
{
	. = 0x100;
	.text : {
		*(.textstart)
		*(.text)
	}
	.data : {
		*(.data)
	}
	.bss : {
		*(.bss)
	}
}
