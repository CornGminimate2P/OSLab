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
}	s;

int     ft_str_is_alpha(char *str)
{
        int     i;

        i = 0;
        while (str[i])
        {
                if (str[i] >= 0 && str[i] <= 42)
                        i++;
                else if (str[i] == 44 || str[i] == 47)
                        i++;
		else if (str[i] >= 58)
			i++;
                else
                        return (0);
        }
        return (1);
}

int	main(void)
{
	pid_t	pid_b;
	pid_t	pid_c;
	int	i;
	char	ch[20];

	i = 0;
	while (1)
	{
		printf("\nEnter value : ");
		scanf("%s", ch);
		if (!ft_str_is_alpha(ch))
		{
			s.nb[i] = atof(ch);
			if (s.nb[i] <= 100)
			{
				i++;
				pid_b = fork();
				if (pid_b < 0)
				{
					fprintf(stderr, "Fork failed.\n");
					exit(-1);
				}
				else if (pid_b == 0)
				{
					float	power;
					int	n;

					power = 0;
					n = 0;
					while (s.nb[n])
					{
						power = s.nb[n] * s.nb[n];
						n++;
						printf("Pow nb %d : %f\n", n, power);
						sleep(1);
					}
					exit(0);
				}
				waitpid(pid_b, NULL, 0);
				pid_c = fork();
				if (pid_c < 0)
				{
					fprintf(stderr, "Fork failed.\n");
					exit(-1);
				}
				else if (pid_c == 0)
				{
					float	sum;
					int	n;

					sum = 0;
					n = 0;
					while (s.nb[n])
					{
						sum += s.nb[n];
						n++;
						printf("Sum nb %d : %f\n", n, sum);
						sleep(2);
					}
					exit(0);
				}
				waitpid(pid_c, NULL, 0);
			}
			else 
				exit(0);
		}
		else
			exit(-1);
	}
	exit(0);
}	
