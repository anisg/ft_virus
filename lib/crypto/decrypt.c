#include "crypto.h"

/*
	decrypt (s, n, key, iscompressed)
	-----------------------------------------------------
	- s: area to decrypt
	- n: length of the area
	- key: 16 bytes key
	- iscompressed: boolean that tells to decompress or not
	              the area after decryption
	-----------------------------------------------------
	return -1 if fail, 0 on success 
*/

int64_t __start decrypt(char *s, uint64_t n, uint32_t *key, bool iscompressed){
    for (uint64_t i = 0; i < n; i += 8){
        if (i + 8 < n)
            decrypt_block((uint32_t*)(s + i), key);
	}

	if (iscompressed){
		String out = decompress(string(s,n));
		if (out.s == NULL)
			return -1;
		ft_memcpy(s, out.s, out.n);
	}
	return 0;
}
