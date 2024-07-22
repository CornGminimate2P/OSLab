#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define USE_MUTEX
pthread_mutex_t mutex;

float	data[100];
float	power[100];
float 	sum[100];
int		i = 0;

void *threadA(void *selector);
void *threadB(void *selector);
void *threadC(void *selector);

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
}

void	*threadA(void *selector)
{
	//char	ch[100];
	float	numbani;

	while(1)
	{
		printf("\nEnter value : ");
		scanf("%f", &numbani);
		//if (!ft_str_is_alpha(ch))
		//{
			pthread_mutex_lock(&mutex);
			data[i] = numbani;//atof(ch);
			pthread_mutex_unlock(&mutex);
			if (data[i] <= 100)
			{
				pthread_mutex_lock(&mutex);
				power[i] = data[i] * data[i];
				pthread_mutex_unlock(&mutex);
				if (i == 0)
				{
					pthread_mutex_lock(&mutex);
					sum[i] = data[i];
					pthread_mutex_unlock(&mutex);
				}
				else
				{
					pthread_mutex_lock(&mutex);
					sum[i] = data[i] + sum[i-1];
					pthread_mutex_unlock(&mutex);
				}
				i++;
			}
			else 
			{
				break;
			}
		//}
		//else
			//pthread_exit(0);
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
			pthread_mutex_lock(&mutex);
			printf("\nPower of %d number is : %f\n", (j+1), power[j]);
			pthread_mutex_unlock(&mutex);
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
			pthread_mutex_lock(&mutex);
			printf("\nTotal sum of %d number is : %f\n", (j+1), sum[j]);
			pthread_mutex_unlock(&mutex);
			j++;
			sleep(2);
		}
	}
	pthread_exit(0);
}
