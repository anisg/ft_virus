#include "crypto.h"

void encrypt_block(uint32_t* v, uint32_t *k) {
	uint32_t v0=v[0], v1=v[1], sum=0, i;          
	uint32_t delta=0x9e3779b9;                     
	uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];   
	for (i=0; i < 32; i++) {                       
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

/*
	encrypt (s, n, key, compressed)
	--------------------------------------------------------------------------
	- s: area to decrypt
	- n: length of the area
	- key: 16 bytes key
	- compressed: pointer to boolean that do 2 things;
				  1) if value is FALSE, it will not compress
				  2) if value is TRUE, it will try to compress, then it will
				     change the value to TRUE or FALSE if the compression succeed
	--------------------------------------------------------------------------
	return -1 if fail, 0, or more if the compression work (the number of removed bytes after compression)
*/

int64_t encrypt(char *s, uint64_t n, uint32_t *k, bool *compressed, void (*fn)(char *, size_t, uint64_t[2])){
	if ((*compressed) == FALSE){
		fn(s, n, k);
		//__encrypt(s, n, k);
		return 0;
	}
	String out = compress(string(s,n));
	if (out.s == NULL)
		return -1;

	if (out.n >= n){
		debug("compress.nope: did not compress");
		fn(s, n, k);
		*compressed = FALSE;
		return 0;
	} else {
		*compressed = TRUE;
		fn(out.s, out.n, k);
		ft_memcpy(s, out.s, out.n);
		debug_ext("compress.stat: (before)", n, ", (now) ", out.n, "\n");
		return n - out.n;
	}
}
