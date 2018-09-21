#include <lib.h>

int infect(char *fname, char *to_inject, size_t n){
	//if (isFile(fname) == FALSE) return fail("not file");
	//if (isElf64(fname) == FALSE) return fail("not elf64 binary");
	char *s; size_t n;
	if (!fget(fname, &s, &n))
		return FALSE;
	return TRUE;
}