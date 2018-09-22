int main(void) asm ("entry");

#include "lib/lib.c"

/*
//extern size_t memaddr;
//extern size_t size;

	//using the extern label, we can now access our own position in memory, and our size, so we can replicate
	void propagation(){
		infect("other_file", (void*)memaddr, size);
	}
*/

int main(){
	println("Hello World!");
	//exit(0);
}
