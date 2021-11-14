#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <string.h> 
#include <errno.h> 
#include <ctype.h>
#ifndef WITH
#	define WITH 50
#endif

#ifndef HEIGHT
#	define HEIGHT 25
#endif

#include <signal.h>
char	art[HEIGHT][WITH];

char	**colors;


#define KNRM  0
#define KRED  1
#define KGRN  2
#define KYEL  3
#define KBLU  4
#define KMAG  5
#define KCYN  6 
#define KWHT  7


void	printf_in_color(char c)
{
	printf("%s%c%s",colors[c % 6],c,colors[0]);
}

void	print_art(void)
{
	int		i,j;

	i = 0;
	j = 0;
	
	while(i < HEIGHT)
	{
		j = 0;
		while(j < WITH)
		{
			printf_in_color(art[i][j]);
			j++;
		}
		putchar ('\n');
		i++;
	}
}
void	bk_fill()
{
	int		i,j;

	i = 1;
	j = 1;
	
	memset(art, '#', sizeof(art));
	strcpy(&art[0][4],"Triziana Coin#");
	while(i < HEIGHT - 1)
	{
		j = 2;
		while(j < WITH - 2)
		{
			art[i][j] = '-';
			j++;
		}
		i++;
	}
}


int			random_pid = 0;

void		random_generator(int fd[2])
{
	int		pid;
	 char *pro[] = { "cat", "/dev/random", 0 };


	pid = fork();
	if(pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);
		execvp(pro[0],pro);
		perror("Execution");
		exit(10);
	}
	random_pid = pid;
}

char	get_char(int fd)
{
	char	c[2];
	memset(&c,'\0',sizeof(c));
	while(read(fd,&c,1))
	{
		if(c[0] && isalnum(c[0]))
			break;
	}
	return (c[0]);
}


void	start_nft(int	fd)
{
	int		j;
	int		i;

	i = 3;
	j = 2;
	while (j < HEIGHT - 2)
	{
		i = 3;
		while (i < WITH - 3)
		{
			art[j][i] =  get_char(fd);
			i++;
		}
		j++;
	}
}


int		main(int argc, char **argv)
{
	colors	= (char **)malloc(9 * sizeof(char *));
	colors[0] = strdup("\x1B[0m");
	colors[1] = strdup("\x1B[31m");
	colors[2] = strdup("\x1B[32m");
	colors[3] = strdup("\x1B[33m");
	colors[4] = strdup("\x1B[34m");
	colors[5] = strdup("\x1B[35m");
	colors[6] = strdup("\x1B[36m");
	colors[7] = strdup("\x1B[37m");
	colors[8] = NULL;
	int		pipe_fd[2];
	
	bk_fill();
	pipe(pipe_fd);
	random_generator(pipe_fd);
	close(pipe_fd[1]);
	start_nft(pipe_fd[0]);
	close (pipe_fd[0]);
	print_art();
	kill(random_pid, SIGINT);
	fflush(stdout);
}