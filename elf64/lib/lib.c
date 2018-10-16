#include <stddef.h>
#include "lib.h"

//asm(R"(
//	.globl call
//	.type call, @function
//	call:
//		.cfi_startproc
//		push %rbp
//		mov %rsp, %rbp
//		mov %rcx, %rax
//		mov %r8, %r10
//		mov %r9, %r8
//		mov 0x10(%rbp), %r9
//		syscall
//		leave
//		ret
//		.cfi_endproc
//)");

size_t __attribute__((section (".textearly"))) call(size_t p1, size_t p2, size_t p3, size_t p4, ...)
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

//==================== LOW FUNCTIONS =====================

void __attribute__((section (".textearly"))) *_malloc(size_t size, int flag)
{
	ssize_t *p = (void*)CALL(SYS_mmap, NULL, sizeof(size_t)+size, 6, 34, -1, 0);
	if (SYS_HAVE_FAIL(p))
		return NULL;
	p[0] = size; //storing the size
	return p + sizeof(size_t);
}


void __attribute__((section (".textearly"))) *ft_malloc(size_t size)
{
	return _malloc(size, 34);
}

void *malloc_shared(size_t size)
{
	return _malloc(size, 33);
}

//==================== HIGHER FUNCTIONS =====================

size_t __attribute__((section (".textearly"))) slen(char const *s)
{
	size_t i = 0;
	while (s[i])
		i++;
	return i;
}

void print(const char *s)
{
	ft_write(1, s, slen(s));
}

void println(const char *s)
{
	print(s);
	print("\n");
}

int fget(const char *filename, char **ptr, size_t *l)
{
	int       fd;
	struct stat buf;

	if (ft_xstat(filename, &buf) < 0)
		return FALSE;
	if (!S_ISREG(buf.st_mode))
		return FALSE;
	if ((fd = ft_open(filename, 0, buf.st_mode)) < 0)
		return FALSE;
	(*l) = ft_lseek(fd, 0, 2);
	(*ptr) = (void*)CALL(SYS_mmap, NULL, *l, 3, 2, fd, 0);
	ft_close(fd);
	if (SYS_HAVE_FAIL(*ptr))
		return FALSE;
	return TRUE;
}

int ffree(char *ptr, size_t l)
{
	size_t i = (size_t)CALL2(SYS_munmap, ptr, l);
}

int fput(const char *filename, char *ptr, size_t l)
{
	int fd;
	mode_t m = 0755;	
	struct stat buf;
	if (!(ft_xstat(filename, &buf) < 0)){
		//file exist
		if (!S_ISREG(buf.st_mode)) return FALSE;
		m = buf.st_mode;
	}
	if ((fd = ft_open(filename, 65, m)) < 0)
		return FALSE;
	ft_write(fd, ptr, l);
	ft_close(fd);
	return TRUE;
}

int     str_equal(const char *s1, const char *s2)
{
	int i;

	i = 0;
	if (!s1 || !s2)
		return FALSE;
	while (s1[i] && s2[i]
			&& (s1[i] == s2[i]))
		i++;
	return (s1[i] - s2[i]) == 0;
}

int     scmp(const char *s1, const char *s2)
{
    int i;

    i = 0;
    if (!s1 || !s2)
        return (0);
    while ((unsigned char)s1[i] && (unsigned char)s2[i]
            && ((unsigned char)s1[i] == (unsigned char)s2[i]))
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int     sncmp(const char *s1, const char *s2, size_t n)
{
    size_t i;

    i = 0;
    if (!s1 || !s2)
        return (0);
    while (i < n && (unsigned char)s1[i] && (unsigned char)s2[i]
            && ((unsigned char)s1[i] == (unsigned char)s2[i]))
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}


int     startswith(char *s, char *d)
{
    size_t i;

    if (!s || !d)
        return (FALSE);
    i = 0;
    while (s[i] && d[i] && s[i] == d[i])
    {
        i += 1;
    }
    if (!d[i])
        return (TRUE);
    return (FALSE);
}

int     contains(char *s, char *d)
{
    size_t i;
    size_t n;

    i = 0;
    n = slen(d);
    while (s[i])
    {
        if (sncmp(s + i, d, n) == 0)
            return (TRUE);
        i += 1;
    }
    return (FALSE);
}

void  snbr(size_t nb, char *sn)
{
	int   i;
	int   j;
	char  tmp;

	i = 0;
	if (!nb)
		sn[i++] = '0';
	while (nb)
	{
		sn[i++] = (nb % 10) + '0';
		nb /= 10;
	}
	sn[i] = 0;
	i -= 1;
	j = 0;
	while (j <= i)
	{
		tmp = sn[j];
		sn[j] = sn[i];
		sn[i] = tmp;
		j += 1;
		i -= 1;
	}
}

void printnb(size_t nb)
{
	char s[64];
	snbr(nb, s);
	print(s);
}

void printnbln(size_t nb)
{
	printnb(nb);
	println("");
}

__attribute__((section (".textearly"))) void	restore_rt()
{
	asm(
			"leave\n"
			"mov $15, %rax\n"
			"syscall\n"
	   );
}

#define SA_RESTORER 0x04000000

__attribute__((section (".textearly"))) int ft_signal(int signal, void (*fn)(int))
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

void __attribute__((section (".textearly"))) *ft_memcpy(void *dest, const void *src, size_t n)
{
	while (n)
	{
		n--;
		((unsigned char*)dest)[n] = ((unsigned char*)src)[n];
	}
	return dest;
}

char __attribute__((section (".textearly"))) *ft_add_base(char *dirname, char *filename)
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

int __attribute__((section (".textearly"))) is_number(char *str)
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

int __attribute__((section (".textearly"))) d_isdir(struct linux_dirent *d){
	return (*(((char *)d) + d->d_reclen - 1)) == DT_DIR;
}
