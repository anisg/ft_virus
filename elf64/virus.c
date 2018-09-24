int main(void) asm ("entry");

#include "lib/lib.c"
#include "lib/infect.c"

extern size_t size;

int main(){
	println("Hello World!");
}
