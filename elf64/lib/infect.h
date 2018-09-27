#ifndef INFECT_H
# define INFECT_H

# include <stdint.h>
# include <stddef.h>

extern char KEY[16];
extern char OLD_KEY[16];
extern int DATA;

//======================= WOODY ==============================

void encrypt_block(uint32_t* v, uint32_t *k);
void encrypt(char *s, uint64_t n, uint32_t *k);
int encrypt_text_section(char *s, size_t n);

//=============================================================

static int _insert(char **s1, size_t *n1, size_t pos, char *s2, size_t n2);
static void _insert_zeros(char **s, size_t *n, size_t pos, size_t add);
void update(char *b, size_t n, size_t old_entry, size_t entry, size_t text_addr, size_t text_length);
static int _prepare(char **s, size_t *n, char *b, size_t bn);

//=============================================================

int check_already_packed(char *s, size_t n);
int create_woody(char *fname, char *b, size_t bn, int force);

#endif
