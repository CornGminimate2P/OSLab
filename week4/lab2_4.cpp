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

struct
{
	int	first[100];
	int	second[100];
	int	i = 0;
} share_mem;

void ProcessB(pid_t pid_b)
{
	if (pid_b < 0)
	{
		fprintf(stderr, "Fork failed.\n");
		exit(-1);
	}
	else if (pid_b == 0)
	{
		int j = 0;
		while (1)
		{
			if (j < share_mem.i)
			{
				//if (j == share_mem.i)
				//{
					//j = 0;
					printf("First number in %d = %d", j+1, share_mem.first[j]);
					sleep(3);
					j++;
				/*}
				else
				{
					printf("First number in %d = %d", j+1, share_mem.first[j]);
					j++;
					sleep(3);
				}*/
			}
		}
		exit(0);
	}
}
/*
void ProcessC(pid_t pid_c, struct Number *share_mem)
{
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
			if (share_mem->i == 0)
			{
				printf("\nProcess C is wating for the first number\n");
				sleep(2);
			}
			if(j == share_mem->i) 
			{
				j = 0;
			}
			else
			{
				printf("\nTotal sum of %d number is : %f\n", (j+1), share_mem->sum[j]);
				j++;
				sleep(2);
			}
		}
		exit(0);
	}
}*/

int main(void)
{
	/*struct Number *share_mem;
	int mem_size = sizeof(struct Number);
	int mem_id = shmget(IPC_PRIVATE, mem_size, 0666 | IPC_CREAT );
	share_mem = (struct Number *)shmat(mem_id, NULL, 0);*/

	pid_t	pid_b;
	pid_t	pid_c;

	
	/*pid_c = fork();
	ProcessC(pid_c, share_mem);*/

	//while (1)
	//{
		printf("\nEnter First value : ");
		scanf("%i", &share_mem.first[share_mem.i]);
		if (share_mem.first[share_mem.i] < 0)
			exit(-1);
		printf("\nEnter Second value : ");
		scanf("%i", &share_mem.second[share_mem.i]);
		share_mem.i = share_mem.i + 1;	
		pid_b = fork();
		ProcessB(pid_b);
	//}
	waitpid(pid_b,NULL,0);
	//waitpid(pid_c,NULL,0);
	return 0;
}
