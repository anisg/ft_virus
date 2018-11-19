#include "ft_lib.h"

uint64_t fail(char *s){
	debug("FAIL: ", s);
	return 0;
}

void __start *ft_malloc(size_t size)
{
    ssize_t *p = (void*)CALL(SYS_mmap, NULL, sizeof(size_t)+size, 6, 34, -1, 0);
    if (SYS_HAVE_FAIL(p))
        return NULL;
    p[0] = size; //storing the size
    return (char*)p + sizeof(size_t);
}

void __start *ft_malloc_x(size_t size)
{
    ssize_t *p = (void*)CALL(SYS_mmap, NULL, sizeof(size_t)+size, PROT_READ|PROT_WRITE,34, -1, 0);
    if (SYS_HAVE_FAIL(p))
        return NULL;
    p[0] = size; //storing the size
    return (char*)p + sizeof(size_t);
}


#define SA_RESTORER 0x04000000
#define SA_SIGNAL_NB 128

__start int ft_signal(int signal, void (*fn)(int))
{
	struct
	{
		void (*sa_sigaction)(int);
		unsigned long sa_flags;
		void *restore;
		char ali[SA_SIGNAL_NB];
	} toto = {fn, SA_RESTORER, &&restore_rt, {0}};

	asm volatile goto("jmp %l[sys]\n" :::: sys);
restore_rt:
	asm volatile("mov $15, %rax\nsyscall\n");
sys:

	return (size_t)CALL(SYS_rt_sigaction, signal, &toto, NULL, SA_SIGNAL_NB / 16);
}

extern char **environ;

char *ft_getenv(const char * restrict k){
	if (!environ)
		return NULL;
	for (int i = 0; environ[i]; i++){
		if (startswith(environ[i], k)){
			size_t l = slen(k);
			if (environ[i][l] == '=')
				return environ[i] + l + 1;
		}
	}
	return NULL;
}

__start void  *ft_memcpy(void *dest, const void *src, size_t n)
{
	while (n)
	{
		n--;
		((unsigned char*)dest)[n] = ((unsigned char*)src)[n];
	}
	return dest;
}

char __start *ft_add_base(const char * restrict dirname, const char *restrict filename)
{
	size_t dir_len = slen(dirname);
	size_t file_len = slen(filename);

	char *ret = ft_malloc(dir_len + file_len + 1 + 1);
	if (ret == NULL)
		return NULL;
	ft_memcpy(ret, dirname, dir_len);
	ret[dir_len] = '/';
	ft_memcpy(ret + 1 + dir_len, filename, file_len);
	ret[dir_len + 1 + file_len] = '\0';
	return ret;
}

int __start is_number(const char * restrict str)
{
	size_t i = 0;

	while (str[i])
	{
		if (str[i] > '9' || str[i] < '0')
			return 0;
		i++;
	}
	return 1;
}

int d_isfile(const struct linux_dirent *d){
	return (*(((char *)d) + d->d_reclen - 1)) == DT_REG;
}

int __start d_isdir(const struct linux_dirent *d){
	return (*(((char *)d) + d->d_reclen - 1)) == DT_DIR;
}

void ft_bzero(void * restrict s, uint64_t n){
	for (uint64_t i = 0; i < n; i++){
		((char*)s)[i] = 0;
	}
}
