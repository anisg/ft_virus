int main(int ac, char **av) asm ("entry");

#include "lib/lib.h"
#include "lib/remote.h"
//#include "lib/infect.c"

//extern size_t memaddr;
extern void _infect();
extern size_t size;

//======================= WOODY ==============================

extern size_t text_start;
extern size_t text_length;

extern char test_area;

extern char   key[16];

#include <elf.h>

/*
void decrypt_block(uint32_t* v, uint32_t *k) {
	uint32_t v0=v[0], v1=v[1], sum=0xC6EF3720, i; 
	uint32_t delta=0x9e3779b9;                     
	uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];   
	for (i=0; i<32; i++) {                         
		v1 -= ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
		v0 -= ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
		sum -= delta;
	}
	v[0]=v0; v[1]=v1;
}
*/

extern void decrypt_block_asm(uint32_t *v, uint32_t *k);

void decrypt(char *s, uint64_t n, uint32_t *k){
	for (uint64_t i = 0; i < n; i += 8){
		if (i + 7 <= n){
			decrypt_block_asm((uint32_t*)(s+i), k);
		}
	}
}

void cant_decrypt(){
	println("invalid key!");
	exit(-1);
}

//=============================================================

int main(int ac, char **av){
	//remote();
	println("Hello, I am Famine");
}
