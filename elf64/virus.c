int main(void) asm ("entry");

#include "lib/lib.c"
#include "lib/infect.c"

extern size_t memaddr;
extern void _infect();
extern size_t size;

extern size_t text_start;
extern size_t text_length;

void decrypt(void *s, size_t n){

}

int main(){
	println("Hello World!");
	printnb(memaddr == (size_t)_infect);
	decrypt((void*)text_start, text_length);
}
