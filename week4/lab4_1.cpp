#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

float	data[100];
float	power[100];
float 	sum[100];
int	i = 0;

void *threadA(void *selector);
void *threadB(void *selector);
void *threadC(void *selector);

int main(void)
{
	pthread_t	tid1, tid2, tid3;
	pthread_attr_t	attr1, attr2, attr3;

	pthread_attr_init(&attr1);
	pthread_attr_init(&attr2);
	pthread_attr_init(&attr3);

	pthread_create(&tid1, &attr1, threadA, NULL);
	pthread_create(&tid2, &attr2, threadB, NULL);
	pthread_create(&tid3, &attr3, threadC, NULL);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_join(tid3, NULL);

	return 0;


void	*threadA(void *selector)
{
	while(1)
	{
		printf("\nEnter value : ");
		scanf("%f", &data[i]);
		if (data[i] <= 100)
		{
			power[i] = data[i] * data[i];
			if (i == 0)
				sum[i] = data[i];
			else
				sum[i] = data[i] + sum[i-1];
			i++;
		}
		else break;
	}
	pthread_exit(0);
}

void	*threadB(void *selector)
{
	int j = 0;
	while (data[j] <= 100)
	{
		if (i == 0)
		{
			printf("\nProcess B is wating for the first number\n");
			sleep(2);
		}
		if (j == i)
		{
			j = 0;
		}
		else 
		{
			printf("\nPower of %d number is : %f\n", (j+1), power[j]);
			j++;
			sleep(1);
		}
        }
        pthread_exit(0);
}

void	*threadC(void *selector)
{
	int	j = 0;
	while (data[j] <= 100)
	{
		if (i == 0){
			printf("\nProcess C is wating for the first number\n");
			sleep(2);
		}
		if (j == i)
		{
			j = 0;
		}
		else
		{
			printf("\nTotal sum of %d number is : %f\n", (j+1), sum[j]);
			j++;
			sleep(2);
		}
	}
	pthread_exit(0);
}
