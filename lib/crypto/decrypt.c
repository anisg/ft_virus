#include "crypto.h"

void __start decrypt(char *s, uint64_t n, uint32_t *key, bool iscompressed){
    for (uint64_t i = 0; i < n; i += 8)
        if (i + 8 < n)
            decrypt_block((uint32_t*)(s + i), key);
	if (iscompressed){
		String out = decompress(string(s,n));
		ft_memcpy(s, out.s, out.n);
	}
}
