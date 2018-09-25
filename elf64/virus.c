int main() asm ("entry");

#include "lib/lib.c"
//#include "lib/infect.c"

extern size_t memaddr;
extern void _infect();
extern size_t size;

extern size_t text_start;
extern size_t text_length;
extern char   key[16];

//======================= WOODY ==============================

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

//=============================================================

int main(){
	println("Hello World!");
	/*printnb(text_start);
	  println("");
	  printnb(text_length);
	  println("");
	  print("KEY IS: ");
	  printnb(((uint32_t*)key)[0]);print(" ");
	  printnb(((uint32_t*)key)[1]);print(" ");
	  printnb(((uint32_t*)key)[2]);print(" ");
	  printnb(((uint32_t*)key)[3]);print(" ");
	  println("");
	  */
	decrypt((void*)text_start, text_length, (uint32_t*)key);
	println("done!");
}
