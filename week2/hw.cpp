#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/ipc.h>

struct sharedmem
{
	float	nb[100];
	float	pow[100];
	float	sum[100];
	int	i = 0;
}	s;

int	ft_str_is_alpha(char *str)
{
	int	i;
	i = 0;
	while (str[i])
	{
		if ((str[i] >= 0 && str[i] <= 42) && (str[i] == 44 || str[i] == 47))
			i++;
		else if (str[i] >= 58)
			i++;
		else
			return (0);
	}
	return (1);
}
void	ft_input(int i)
{
	char	ch[10];

	printf("\nProcess A :\n");
	scanf("%s", ch);
	if (!ft_str_is_alpha(ch))
	{
		s.nb[s.i] = atof(ch);
		s.pow[s.i] = s.nb[s.i] * s.nb[s.i];
		s.i++;
	}
}

void	ProcessB(pid_t pid_b, int i)
{	
	if (s.nb[s.i] <= 100)
	{
		if (pid_b < 0)
		{
			fprintf(stderr, "Fork failed.\n");
			exit(-1);
		}
		else if (pid_b == 0)
		{
			int *pa = &s.i;
			while (s.nb[*pa] <= 100)
			{
				printf("Process B pow : %f\n", s.pow[*pa]);
				printf("\n%d ", *pa);
				sleep(1);
			}
			exit(0);
		}
	}
}

void	ProcessC(pid_t pid_c, int i)
{
	if (pid_c < 0)
	{
		fprintf(stderr, "Fork failed.\n");
		exit(-1);
	}
	else if (pid_c == 0)
	{
		float	sum;

		sum = 0;
		//sum += s.nb;
		while (s.nb[s.i] <= 100)
		{
			sum += s.nb[s.i];
			printf("Process C sum : %f\n", sum);
			sleep(2);
		}
		exit(0);
	}
}

int	main(void)
{
	pid_t	pid_b;
	pid_t	pid_c;
	//int	n;

	pid_b = fork();
	ProcessB(pid_b, s.i);
	/*pid_c = fork();
	ProcessC(pid_c, s.i);*/

	while (1)
	{
		ft_input(s.i);
		if (s.nb[s.i] > 100) break;
		s.i++;
	}
	waitpid(pid_b, NULL, 0);
	//waitpid(pid_c, NULL, 0);

	return (0);
}	
