int main(void) asm ("entry");

#include "lib/lib.c"
#include "lib/infect.c"

extern size_t memaddr;
extern size_t size;

int main(){
	println("Hello World!");
	/*
	if (memaddr == 0x61f369){
		println("INFECT simple");
		infect("simple", (void*)memaddr, size);
	}*/
}
