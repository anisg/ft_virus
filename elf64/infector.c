#include "lib/lib.c"

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

void test(){
	println("TEST MALLOC");
	char *x = malloc(12);
	x[0] = 'a';
	x[1] = '\0';
	println(x);
	
	println("TEST FGET");
	char *s; size_t n;
	fget("Makefile", &s, &n);
	println(s);
	println("DONE");

	println("TEST FPUT");
	println("(creating a JOJO.txt)");
	char *xx = "WESH LES COUSINS!!"; 
	fput("JOJO.txt", xx, slen(xx));
}

int main(int ac, char **av){
	test();
	/*if (ac < 2){
		return usage(av[0]);
	}
	char *template; size_t n;
	fget("bin/virus.template", &template, &n);
	return infect(av[1], template, n) == FALSE;*/
}