ENTRY(_start)
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
}
