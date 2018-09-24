int main(void) asm ("entry");

#include "lib/lib.c"
//#include "lib/infect.c"

extern size_t memaddr;
extern void _infect();
extern size_t size;

extern size_t text_start;
extern size_t text_length;

void decrypt(void *s, size_t n){

}

int main(){
	println("Hello World!");
	printnb(text_start);
	println("");
	printnb(text_length);
	println("");

	decrypt((void*)text_start, text_length);
}
