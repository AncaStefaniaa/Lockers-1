#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <stdatomic.h>
#include "mutex.h"

#define MAX_RESOURCES 10
#define NUM_THR 10


lock mtx;
int available_resources = MAX_RESOURCES;

int decrease_count(int count) {
	lock_aquire(&mtx);
	if (available_resources < count) {
		lock_release(&mtx);
		return -1;
	}
	else {
		available_resources -= count;
		printf("Got %d resources, %d remaining\n", count, available_resources);
	}
	lock_release(&mtx);
	return 0;
}

int increase_count(int count) {
	lock_aquire(&mtx);
	available_resources += count;
	printf("Released %d resources, %d remaining\n", count, available_resources);
	lock_release(&mtx);
	return 0;
}

void* consume(void* arg) {
    int res = *(int *)arg;
    while (decrease_count(res) == -1);
    increase_count(res);
    return NULL;
}

int main() {
	int cnt_threads = NUM_THR;
	pthread_t *threads = (pthread_t *)malloc(cnt_threads * sizeof(pthread_t));
	int *id = (int *)malloc(cnt_threads * sizeof(int));

	lock_init(&mtx);

	for (int i = 0; i < cnt_threads; ++i) {
        	id[i] = rand() % 5 + 1;
        	if (pthread_create(threads + i, NULL, consume, id + i)) {
            		perror(NULL);
            		return errno;
        	}
    	}

	for (int i = 0; i < cnt_threads; ++i) {
        	if (pthread_join(threads[i], NULL)) {
            		perror(NULL);
            		return errno;
        	}
    	}

	free(threads);
	free(id);
	mutex_destroy(&mtx);
	return 0;
}
