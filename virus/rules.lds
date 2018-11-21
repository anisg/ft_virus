ENTRY(_infect)
	SECTIONS
{
	. = 0x100;
	bin_start = .;
	.text : {
		*(.textstart)
		*(.textearly)
		cmpr_start = .;
		*(.textcmpr)
		cmpr_end = .;
		crypt_start = .;
		*(.text)
	}
	.data : {
		*(.data)
	}
	crypt_end = .;
	bin_end = .;
}
