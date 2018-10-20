#ifndef LIB_H
# define LIB_H

#include <sys/types.h>
#include <sys/stat.h>
#include "bool.h"
#include <stddef.h>
#include <syscall.h>
#include <dirent.h>
# include <stdint.h>
# include <stddef.h>

size_t call(size_t p1, size_t p2, size_t p3, size_t sys_num, ...);
// rax      rdi        rsi        rdx        rcx         r8         r9        16(%rbp)
// rax      rdi        rsi        rdx        rax         r10        r8        r9

#define CALL(X,a1,a2,a3,...)		call((size_t)(a1), (size_t)(a2), (size_t)(a3), X, ##__VA_ARGS__)
#define CALL0(X)			CALL(X, 0, 0, 0)
#define CALL1(X,a1)			CALL(X, a1, 0, 0)
#define CALL2(X,a1,a2)			CALL(X, a1, a2, 0)

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) <= (b) ? (a) : (b))

#define SYS_MAX_ERRNO ((size_t)(4000))
#define SYS_ERROR ((size_t)(-1))
#define SYS_HAVE_FAIL(X) ((size_t)(X) > (size_t)(~0) - SYS_MAX_ERRNO)
#define SYS_RET(X) (SYS_HAVE_FAIL(X) ? SYS_ERROR : (size_t)(X))

#include "ft_syscalls.h"

void *ft_malloc(size_t size);
int ft_signal(int signum, void (*handler)(int));

size_t slen(const char *s);
void snbr(size_t nbr, char *sn);

void print(const char *s);
void println(const char *s);
void printnb(size_t nb);

int is_number(const char *str);

int     sncmp(const char *s1, const char *s2, size_t n);
int fget(const char *filename, char **ptr, size_t *l);
void ffree(char *ptr, size_t l);
int fput(const char *filename, char *ptr, size_t l);
int str_equal(const char *s1, const char *s2);
void ft_bzero(void *s, uint64_t n);
int     startswith(const char *s, const char *d);
void *ft_memcpy(void *dest, const void *src, size_t n);

char *ft_getenv(const char *k);

char* ft_add_base(const char *dirname, const char *filename);

struct linux_dirent {
	unsigned long	d_ino;
	int64_t		d_off;
	unsigned short	d_reclen;
	char		d_name[];
};

int d_isfile(const struct linux_dirent *d);
int d_isdir(const struct linux_dirent *d);

#endif
