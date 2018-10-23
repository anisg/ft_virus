#include "ft_lib.h"

uint64_t fail(char *s){
	debug("FAIL: ", s);
	return 0;
}

__attribute__((optimize("O0"))) __start size_t call(size_t p1, size_t p2, size_t p3, size_t p4, ...)
{
    asm(R"(
            mov %rcx, %rax
            mov %r8, %r10
            mov %r9, %r8
            mov 0x10(%rbp), %r9
            syscall
            leave
            ret
    )");
}

__start void	restore_rt()
{
	asm(
			"leave\n"
			"mov $15, %rax\n"
			"syscall\n"
	   );
}

 __start void *_malloc(size_t size, int flag)
{
    ssize_t *p = (void*)CALL(SYS_mmap, NULL, sizeof(size_t)+size, 6, 34, -1, 0);
    if (SYS_HAVE_FAIL(p))
        return NULL;
    p[0] = size; //storing the size
    return (char*)p + sizeof(size_t);
}


__start void *ft_malloc(size_t size)
{
    return _malloc(size, 34);
}


#define SA_RESTORER 0x04000000

__start int ft_signal(int signal, void (*fn)(int))
{
	struct
	{
		void (*sa_sigaction)(int);
		unsigned long sa_flags;
		void *restore;
		char ali[128];
	} toto = {fn, SA_RESTORER, &restore_rt, {0}};

	return (size_t)CALL(SYS_rt_sigaction, signal, &toto, NULL, 8);
}

extern char **environ;

char *ft_getenv(char *k){
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

__start char *ft_add_base(char *dirname, char *filename)
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

__start int is_number(char *str)
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

int d_isfile(struct linux_dirent *d){
	return (*(((char *)d) + d->d_reclen - 1)) == DT_REG;
}

__start int d_isdir(struct linux_dirent *d){
	return (*(((char *)d) + d->d_reclen - 1)) == DT_DIR;
}

void ft_bzero(void *s, uint64_t n){
	for (uint64_t i = 0; i < n; i++){
		((char*)s)[i] = 0;
	}
}
