#include "elf64.c"
#include <stdio.h>

char KEY[16];

static int _insert(char **s1, size_t *n1, size_t pos, char *s2, size_t n2){
        char *ns;
        if ((ns = (char *)malloc((*n1) + n2)) == NULL)
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
        char *ns = (char *)malloc((*n) + add);
        size_t i,j,l;
        for (i = 0; i <= pos; i += 1){ ns[i] = (*s)[i]; }
        for (j = 0; j < add; j += 1){ns[i+j] = 0; }
        for (l = 0; i+l < *n; l += 1){ ns[i+j+l] = (*s)[i+l]; }
	*s = ns;
	*n = (*n) + add;
}

void update(char *b, size_t n, size_t old_entry, size_t entry, size_t text_addr, size_t text_length){
	//add a few information about himself
    Elf64_Ehdr *bh = (void*)b;
	size_t pos = elf_offset_entry(b, n);
	int DATA = 0x02;
	//modifying 2bit after
	((size_t*)((char*)(b + pos + DATA)))[0] = max(old_entry,entry) - min(old_entry,entry);
	((size_t*)((char*)(b + pos + DATA)))[1] = entry - pos;
	//printf("diff: %zx, size: %zx\n", entry - pos, n);
	((size_t*)((char*)(b + pos + DATA)))[2] = n;
	size_t ok = max(text_addr,entry) - min(text_addr,entry);
	((size_t*)((char*)(b + pos + DATA)))[3]=ok;
	((size_t*)((char*)(b + pos + DATA)))[4] = text_length;
	//inserting KEY
	((size_t*)((char*)(b + pos + DATA)))[5] = ((size_t*)KEY)[0];
	((size_t*)((char*)(b + pos + DATA)))[6] = ((size_t*)KEY)[1];

}

static size_t _prepare(char **s, size_t *n, char *b, size_t bn){
    Elf64_Ehdr *h = (void*)*s;
    Elf64_Phdr *ph = (*(void**)s) + h->e_phoff;

	size_t old_entry = h->e_entry;
    int x = elf_last_load_segment(*s, *n);
	size_t text_addr = elf_addr_text_section(*s,*n);
	size_t text_length = elf_size_text_section(*s,*n);
    size_t diff = ph[x].p_memsz - ph[x].p_filesz;
    if (diff > 0){
    	//it means the segment will get bigger in mem, but we don't need that so we make it bigger in the file
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
	_insert(s, n, pos, b, bn);
	//reset it
    	elf_change_size_last_load_segment(*s, *n, bn);
    	elf_update_flags_of_load_segments(*s, *n);
	elf_set_off_entry(*s, *n, pos + 1 + elf_offset_entry(b, bn));
	elf_shift_offset(*s, *n, pos, bn);
    	h = (void*)*s;
	update((*s) + pos + 1, bn, old_entry, h->e_entry, text_addr, text_length);
	return pos;
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
                if (i + 7 <= n){
                        encrypt_block((uint32_t*)(s+i), k);
                }
        }
}

void encrypt_text_section(char *s, size_t n){
    Elf64_Ehdr *h = (void*)s;
    Elf64_Phdr *ph = ((void*)s) + h->e_phoff;

	size_t text_off = elf_off_text_section(s,n);
	size_t text_length = elf_size_text_section(s,n);
	//printf("%zu v %zu\n", text_off, text_length);
	encrypt(s + text_off, text_length, (uint32_t*)KEY);
}

//=============================================================

int create_woody(char *fname, char *b, size_t bn){
	char *s; size_t n;
	if (!fget(fname, &s, &n))
		return FALSE;
	if (elf_check_valid(s, n) == FALSE) return FALSE;
	//after insert, update data
	encrypt_text_section(s,n);
	_prepare(&s,&n, b, bn);
	fput("woody", s, n);
	return TRUE;
}
