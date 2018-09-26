ENTRY(_infect)
	SECTIONS
{
	. = 0x100;
	bin_start = .;
	.text : {
		*(.textstart)
		*(.text)
	}
	.data : {
		*(.data)
	}
	bin_end = .;
	.bss : {
		*(.bss)
	}
}
