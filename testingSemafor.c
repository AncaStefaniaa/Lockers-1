#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>


#include "semafor.h"

#define threadsNumber 5

semafor sem;

pthread_mutex_t mtx;
int S = 0;

int barrier_point()
{
	pthread_mutex_lock(&mtx);
	S++;
	pthread_mutex_unlock(&mtx);

	if(S < threadsNumber)
	{	
		if(sem_wait(&sem))
		{
			perror(NULL);
			return errno;
		}

    }
    else {
		int i;
		for (i = 0; i < threadsNumber; ++i){
			sem_post(&sem);
		}
	}
	return 0;

}

void* tfun(void* v)
{
	int* tid = (int*) v;
	printf("%d reached the barrier ", *tid);
	barrier_point();
	printf("\n%d passed the barrier \n", *tid);
}


int main()
{
	pthread_t thr[5];
	int i;

	if(sem_init(&sem, 0, 0))
	{
		perror(NULL);
		return errno;
	}

	if(pthread_mutex_init(&mtx, NULL))
	{
		perror(NULL);
		return errno;
	}

	for(i = 0; i < threadsNumber; i++)
	{
        int *k = (int *)malloc(1);
        *k = i;
		if(pthread_create(&thr[i], NULL, tfun, k))
			{
				perror(NULL);
				return errno;
			}
	}

	void* result = (void*) malloc(1);

	for(i = 0; i < threadsNumber; i++){

		if(pthread_join(thr[i], &result))
		{
			perror(NULL);
			return errno;
		}

	}

	sem_destroy(&sem);
	return 0;
}
