#include "ft_lib.h"

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
