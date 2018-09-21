#include <lib.h>

int fail(char *reason){
	print("FAIL: ");
	print(reason);
	println("");
	return 1;
}

int usage(char *name){
	print("usage: ./");
	print(name);
	print(" binary\n\nnote: the binary must be in elf64\n");
	return -1;
}

int infect(char *fname){
	if (isFile(fname) == FALSE) return fail("not file");
	if (isElf64(fname) == FALSE) return fail("not elf64 binary");
	char *s; size_t n;
	fget(fname, &s, &n);	
	fput("");
	return TRUE;
}

int main(int ac, char **av){
	if (ac < 2){
		return usage(av[0]);
	}
	return infect(av[1]);
}
