#include "ft_lib.h"

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

void ffree(char *ptr, size_t l)
{
	CALL2(SYS_munmap, ptr, l);
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

void print(const char *s)
{
	ft_write(1, s, slen(s));
}

void println(const char *s)
{
	print(s);
	print("\n");
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
