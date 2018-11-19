#ifndef CRYPTO_H
#define CRYPTO_H

# include <stdint.h>
# include <stddef.h>
# include "ft_lib.h"

//==============================================================================
// tea_encrypt.c
//------------------------------------------------------------------------------
// Methods for encrypting using TEA algorithm (in ASM)
//==============================================================================

int64_t encrypt(char *s, uint64_t n, uint32_t *k, bool *compressed, void (*fn)(char *, size_t, uint64_t[2]));

//==============================================================================
// tea_decrypt.s (in ASM)
//------------------------------------------------------------------------------
// Methods for decrypting using TEA algorithm
//==============================================================================

void decrypt_block(uint32_t *s, uint32_t *key);
int64_t decrypt(char *s, uint64_t n, uint32_t *key, bool iscompressed , void (*fn)(char *, size_t, uint64_t[2]));

//==============================================================================
// compress_use.c
//------------------------------------------------------------------------------
// utility methods and data structure for the compression.
//==============================================================================

//-------------------- Sort ------------------------

void sort(void *arr, uint64_t n, uint64_t len, int (*comp)(void *a, void *b));

//-------------------- MinQueue -------------------

typedef struct s_minQueue {
	void *arr;
	int  n; //item count
	int  mn; //max n limit
	int  len; //item size
	int (*less)(void *a, void *b);
}              MinQueue;

//note: MinQueue with fixed length
MinQueue *minQueue(void *arr, uint64_t n, uint64_t len, int (*less)(void *a, void *b));
bool mqEmpty(MinQueue *mq);
void *mqExtractMin(MinQueue *mq);
bool mqInsert(MinQueue *mq, void *d);

//-------------------- String ---------------------

typedef struct	s_string {
	char     *s;
	uint64_t n;
}				String;

String string(char *s, uint64_t n); //pack (s,n) to String

//-------------------- Other ---------------------

bool fget(const char *filename, char **ptr, size_t *l);
bool fput(const char *filename, char *ptr, size_t l);

//==============================================================================
// compress.c
//------------------------------------------------------------------------------
// Huffman compression and decompression methods
//==============================================================================

String compress(String b);
String decompress(String b);

//-------------------- Structs ---------------------

#define NOP 256

typedef struct s_node{
	uint32_t v; //value: between 0 and 255 and NOP
	uint32_t freq;
	struct s_node *l;
	struct s_node *r;
}              Node;

typedef struct s_encoded_node8{
	unsigned char v;
	uint8_t freq;
} __attribute__((packed)) EncodedNode8;

typedef struct s_encoded_node16{
	unsigned char v;
	uint16_t freq;
} __attribute__((packed)) EncodedNode16;

typedef struct s_encoded_node32{
	unsigned char v;
	uint32_t freq;
} __attribute__((packed)) EncodedNode32;

typedef EncodedNode32 EncodedNode;

typedef struct s_char_code{
	uint64_t rep;
	int n;
} CharCode;

#endif
