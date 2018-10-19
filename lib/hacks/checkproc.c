#include "hacks.h"

int __attribute__((section (".textearly"))) cmpproc(char *name, int fd)
{
	size_t i = 0;
	ssize_t ret;
	char buff[1];
	while ((ret = ft_read(fd, buff, sizeof(buff))) >= 0)
	{
		if (ret == 0)
			return 1;
		if (name[i] == 0 && buff[0] == '\n')
			return 0;
		if (name[i] == 0)
			return 1;
		if (name[i] != buff[0])
			return 1;
		i++;
	}
	return -1;
}

int __attribute__((section (".textearly"))) checkproc(char *programe_name){
	struct linux_dirent *d;
	char		p[4096];
	char		proc[6];
	char		comm[5];
	int		fd;
	char		*buff;
	size_t		t;
	int		ret = TRUE;

	proc[0] = '/';
	proc[1] = 'p';
	proc[2] = 'r';
	proc[3] = 'o';
	proc[4] = 'c';
	proc[5] = '\0';
	comm[0] = 'c';
	comm[1] = 'o';
	comm[2] = 'm';
	comm[3] = 'm';
	comm[4] = '\0';
	if ((fd = ft_open(proc, 65536, 0)) < 0) return TRUE;
	while (ret == TRUE)
	{
		int size = getdents(fd, p, sizeof(p));
		if (size == 0)
			break;
		if (size < 0)
		{
			ret = FALSE;
			break;
		}
		size_t x;
		for (x = 0; x < size && ret == TRUE; x += d->d_reclen)
		{
			d = (struct linux_dirent*)(p + x);
			if (d_isdir(d) && is_number(d->d_name)){
				char *t1 = NULL;
				char *t2 = NULL;
				int fdproc = -1;
				if ((t1 = ft_add_base(proc, d->d_name)) == NULL)
					goto end;
				if ((t2 = ft_add_base(t1, comm)) == NULL)
					goto end;
				if ((fdproc = ft_open(t2, 0, 0)) < 0)
					goto end;
				if (cmpproc(programe_name, fdproc) == 0)
					ret = FALSE;
end:
				if (fd >= 0)
					ft_close(fdproc);
				if (t1 != NULL)
					ft_free(t1);
				if (t2 != NULL)
					ft_free(t2);
			}
		}
	}
	ft_close(fd);
	return (ret);
}
