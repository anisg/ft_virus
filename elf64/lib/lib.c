#include <stddef.h>
#include "lib.h"

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

void *_malloc(size_t size, int flag)
{
	ssize_t *p = (void*)CALL(MMAP, NULL, sizeof(size_t)+size, 6, 34, -1, 0);
	if (SYS_HAVE_FAIL(p))
		return NULL;
	p[0] = size; //storing the size
	return p + sizeof(size_t);
}


void *malloc(size_t size)
{
	return _malloc(size, 34);
}

void *malloc_shared(size_t size)
{
	return _malloc(size, 33);
}

int execve(const char *fichier, char *const argv[], char *const envp[])
{
	return CALL(EXECVE, fichier, argv, envp);
}

pid_t fork(void)
{
	return CALL0(FORK);
}

pid_t getpid(void)
{
	return CALL0(GETPID);
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

long ptrace(long request, long pid, unsigned long addr, void *data){
	return CALL(PTRACE, request, pid, addr, data);
}

int xstat(const char *filename, struct stat *buf){
	return CALL2(STAT, filename, buf);
}

//==================== HIGHER FUNCTIONS =====================

size_t slen(char const *s)
{
	size_t i = 0;
	while (s[i])
		i++;
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
	struct stat buf;

	if (xstat(filename, &buf) < 0)
		return FALSE;
	if (!S_ISREG(buf.st_mode))
		return FALSE;
	if ((fd = open(filename, 0, buf.st_mode)) < 0)
		return FALSE;
	(*l) = lseek(fd, 0, 2);
	(*ptr) = (void*)CALL(MMAP, NULL, *l, 3, 2, fd, 0);
	close(fd);
	if (SYS_HAVE_FAIL(*ptr))
		return FALSE;
	return TRUE;
}

int ffree(char *ptr, size_t l)
{
	size_t i = (size_t)CALL2(MUNMAP, ptr, l);
}

int fput(const char *filename, char *ptr, size_t l)
{
	int fd;
	mode_t m = 0755;	
	struct stat buf;
	if (!(xstat(filename, &buf) < 0)){
		//file exist
		if (!S_ISREG(buf.st_mode)) return FALSE;
		m = buf.st_mode;
	}
	if ((fd = open(filename, 65, m)) < 0)
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

//pid_t wait(int *stat_loc){
//	return CALL(WAIT, 0, stat_loc, 0, NULL);
//}

pid_t waitpid(pid_t pid, int *stat_loc, int option){
	return CALL(WAIT, pid, stat_loc, option, NULL);
}

void	restore_rt();
asm(
		".align 16\n"
		"restore_rt:\n"
		"mov $15, %rax\n"
		"syscall\n"
   );

#define SA_RESTORER 0x04000000

int signal(int signal, void (*fn)(int))
{
	struct
	{
		void (*sa_sigaction)(int);
		unsigned long sa_flags;
		void *restore;
		char ali[128];
	} toto = {fn, SA_RESTORER, restore_rt, {0}};

	return (size_t)CALL(SIGACTION, signal, &toto, NULL, 8);
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


void add_base(char *tmp, char *dir, char *file, int lim){
	int i,j;
	for (i = 0; dir[i] && i < lim; i++)
		tmp[i] = dir[i];
	tmp[i++] = '/';
	for (j = 0; file[j] && i + j < lim; j++)
		tmp[i+j] = file[j];
	tmp[i+j] = '\0';
}

int d_isfile(struct linux_dirent *d){
	return (*(((char *)d) + d->d_reclen - 1)) == DT_REG;
}

int d_isdir(struct linux_dirent *d){
	return (*(((char *)d) + d->d_reclen - 1)) == DT_DIR;
}

int getdents(int fd, char *buff, size_t buff_size)
{
	return CALL(GETDENTS, fd, buff, buff_size);
}
