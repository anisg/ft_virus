#include <stddef.h>
#include "lib.h"

#define SYS_MAX_ERRNO ((size_t)(4000))
#define SYS_ERROR ((size_t)(-1))
#define SYS_HAVE_FAIL(X) ((size_t)(X) > (size_t)(~0) - SYS_MAX_ERRNO)
#define SYS_RET(X) (SYS_HAVE_FAIL(X) ? SYS_ERROR : (size_t)(X))

asm(R"(
	.globl call
	.type func, @function
	call:
		.cfi_startproc
		push %rbp
		mov %rsp, %rbp
		mov %rcx, %rax
		mov %r8, %r10
		mov %r9, %r8
		mov 0x10(%rbp), %r9
		syscall
		leave
		ret
		.cfi_endproc
)");

//==================== LOW FUNCTIONS =====================

void exit(int n)
{
	CALL1(EXIT, n);
	__builtin_unreachable();
}

void *malloc(size_t size)
{
	ssize_t *p = (void*)CALL(MMAP, NULL, sizeof(size_t)+size, 6, 34, -1, 0);
	if (SYS_HAVE_FAIL(p))
		return NULL;
	p[0] = size; //storing the size
	return p + sizeof(size_t);
}

int execve(const char *fichier, char *const argv[], char *const envp[])
{
	return CALL(EXECVE, fichier, argv, envp);
}

pid_t fork(void)
{
	return CALL0(FORK);
}

void free(void *p)
{
	size_t *tab = p;
	CALL2(MUNMAP, &tab[-1], tab[-1]);
}

ssize_t write(int fd, const void *s, size_t n)
{
	return CALL(WRITE, fd, (size_t)s, n);
}

ssize_t read(int fd, void *s, size_t n)
{
	return CALL(READ, fd, (size_t)s, n);
}

int close(int fd)
{
	return CALL1(CLOSE, fd);
}

int open(const char *filename, int flag, int mode)
{
	return CALL(OPEN, (size_t)filename, flag, mode);
}

off_t lseek(int fd, off_t offset, int whence)
{
	return CALL(LSEEK, fd, offset, whence);
}

long ptrace(long request, long pid, unsigned long addr){
	return CALL(PTRACE, request, pid, addr);
}

//==================== HIGHER FUNCTIONS =====================

size_t slen(char const *s)
{
	size_t i;
	for (i=0; s[i]; i++)
		;
	return i;
}

void print(const char *s)
{
	write(1, s, slen(s));
}

void println(const char *s)
{
	print(s);
	print("\n");
}

int fget(const char *filename, char **ptr, size_t *l)
{
	int       fd;

	if ((fd = open(filename, 0, 0755)) < 0)
		return FALSE;
	(*l) = lseek(fd, 0, 2);
	(*ptr) = (void*)CALL(MMAP, NULL, *l, 3, 2, fd, 0);
	close(fd);
	if (SYS_HAVE_FAIL(*ptr))
		return FALSE;
	return TRUE;
}

int fput(const char *filename, char *ptr, size_t l)
{
	int fd;

	if ((fd = open(filename, 65, 0755)) < 0)
		return FALSE;
	write(fd, ptr, l);
	close(fd);
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
    return (i < n || (unsigned char)s1[i] - (unsigned char)s2[i]);
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

int is_debbuger_on(){
	//return FALSE;

#define PTRACE_TRACEME 0
#define PTRACE_DETACH 17

	int x = (ptrace(PTRACE_TRACEME, 0, 1) == -1);
	ptrace(PTRACE_DETACH, 0, 0);
	return x;
}

extern char **environ;

char *getenv(char *k){
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
