#ifndef CRYPTO_H
#define CRYPTO_H

# include <stdint.h>
# include <stddef.h>

//=======================================================
//==================== Encrypt ==========================

void encrypt(char *s, uint64_t n, uint32_t *k);

//=======================================================
//==================== Decrypt ==========================

void decrypt(char *s, uint64_t n, uint32_t *k);


#endif
