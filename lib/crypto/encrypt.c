#include "crypto.h"

void encrypt_block(uint32_t* v, uint32_t *k) {
	uint32_t v0=v[0], v1=v[1], sum=0, i;           /* initialisation */
	uint32_t delta=0x9e3779b9;                     /* constantes de clef */
	uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];   /* mise en cache de la clef */
	for (i=0; i < 32; i++) {                       /* boucle principale */
		sum += delta;
		v0 += ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
		v1 += ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
	}
	v[0] = v0;
	v[1]=v1;
}

void __encrypt(char *s, uint64_t n, uint32_t *k){
	for (uint64_t i = 0; i < n; i += 8){
		if (i + 8 < n){
			encrypt_block((uint32_t*)(s+i), k);
		}
	}
}

uint64_t encrypt(char *s, uint64_t n, uint32_t *k, bool *compressed){
	if ((*compressed) == FALSE){
		__encrypt(s, n, k);
		return 0;
	}
	String out = compress(string(s,n));
	if (out.n >= n){
		debug("compress.nope: did not compress");
		__encrypt(s, n, k);
		*compressed = FALSE;
		return 0;
	} else {
		*compressed = TRUE;
		__encrypt(out.s, out.n, k);
		ft_memcpy(s, out.s, out.n);
		debugnb("compress.stat: (before)", n, ", (now) ", out.n);
		return n - out.n;
	}
}
