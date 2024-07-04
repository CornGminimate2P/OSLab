#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/ipc.h>

struct Number
{
	float	data[5];
	float	power[5];
	float 	sum[5];
	int i = 0;
}s;

int ft_str_is_alpha(char *str)
{
        int     i;
        i = 0;
        while (str[i])
        {
                if ((str[i] >= 0) && (str[i] <= 42)) // Allows user to type +
                        i++;
                else if (str[i] == 44 && str[i] == 47) // Allows user to type negative or decimal number
                        i++;
                else if (str[i] >= 58)
                        i++;
                else return (0);
        }
        return (1);
}

void ProcessB(pid_t pid_b, struct Number *share_mem){
	if (pid_b < 0)
	{
		fprintf(stderr, "Fork failed.\n");
                exit(-1);
    }
    else if (pid_b == 0)
    {
        int j = 0;
        while (share_mem->data[j] <= 100)
        {
            if (share_mem->i == 0){
                printf("\nProcess B is wating for the first number\n");
                sleep(2);
            }
            if(j == share_mem->i) {
                j = 0;
            }
            else{
                printf("\nPower of %d number is : %f\n", (j+1), share_mem->power[j]);
                j++;
                sleep(1);
            }
        }
        exit(0);
    }
}

void ProcessC(pid_t pid_c, struct Number *share_mem){
	if (pid_c < 0)
	{
		fprintf(stderr, "Fork failed.\n");
		exit(-1);
	}
	else if (pid_c == 0)
	{
		int	j = 0;
		while (share_mem->data[j] <= 100)
		{
            if (share_mem->i == 0){
                printf("\nProcess C is wating for the first number\n");
                sleep(2);
            }

            if(j == share_mem->i) {
                j = 0;
            }
            else{
                printf("\nTotal sum of %d number is : %f\n", (j+1), share_mem->sum[j]);
                j++;
                sleep(2);
            }
		}
		exit(0);
	}
}

int main(void)
{
    struct Number *share_mem;
    int mem_size = sizeof(struct Number);
    int mem_id = shmget(IPC_PRIVATE, mem_size, 0666 | IPC_CREAT );
    share_mem = (struct Number *)shmat(mem_id, NULL, 0);

	pid_t	pid_b;
	pid_t	pid_c;
	//char	ch[20];
    pid_b = fork();
    ProcessB(pid_b, share_mem);
    pid_c = fork();
    ProcessC(pid_c, share_mem);

	while (1)
	{
		printf("\nEnter value : ");
		scanf("%f", &share_mem->data[share_mem->i]);
		/*if (!ft_str_is_alpha(ch))
		{
			share_mem->data[share_mem->i] = atof(ch);*/
		if (share_mem->data[share_mem->i] <= 100)
		{
			share_mem->power[share_mem->i] = share_mem->data[share_mem->i] * share_mem->data[share_mem->i];
			if (share_mem->i == 0)
				share_mem->sum[share_mem->i] = share_mem->data[share_mem->i];
			else
				share_mem->sum[share_mem->i] = share_mem->data[share_mem->i] + share_mem->sum[share_mem->i-1];
			share_mem->i++;
		}
		else break;
		//}
		//else break;
    }
    waitpid(pid_b,NULL,0);
    waitpid(pid_c,NULL,0);
    return 0;
}
