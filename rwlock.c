#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include "semafor.h"

#define NUM_THREADS 10

typedef struct {
	sem_t rw_mutex;
	sem_t mutex;
	int read_count;
} rwlock_t;

rwlock_t rwlock;

int rwl_init(rwlock_t *rwl) {
	int err;

	err = sem_init(&rwl->rw_mutex, 0, 1);
	if (err != 0)
		return err;

	err = sem_init(&rwl->mutex, 0, 1);
	if (err != 0)
		return err;

	rwl->read_count = 0;

	return 0;
}

int rwl_destroy(rwlock_t *rwl) {
	int err;

	err = sem_destroy(&rwl->rw_mutex);
	if (err != 0)
		return err;

	err = sem_destroy(&rwl->mutex);
	if (err != 0)
		return err;

	return 0;
}

int writer(rwlock_t *rwl) {
	int err;

	err = sem_wait(&rwl->rw_mutex);
	if (err != 0)
		return err;

	/* WRITE */
	const char *buff = "Data written by the writer!\n";
	size_t len_buff = strlen(buff);
	ssize_t write_b = write(STDIN_FILENO, buff, len_buff);
	if (write_b != len_buff) {
		perror(NULL);
		return errno;
	}
	sleep(1);
	/* END WRITE */

	err = sem_post(&rwl->rw_mutex);
	if (err != 0)
		return err;

	return 0;
}

int reader(rwlock_t *rwl) {
	int err;

	err = sem_wait(&rwl->mutex);
	if (err)
		return err;

	rwl->read_count++;
	if (rwl->read_count == 1) {
		err = sem_wait(&rwl->rw_mutex);
		if (err != 0)
			return err;
	}

	err = sem_post(&rwl->mutex);
	if (err != 0)
		return err;

	/* READ */
	char *buff = "Data read by the reader!\n";
	size_t len_buff = strlen(buff);
	ssize_t write_b = write(STDIN_FILENO, buff, len_buff);
	if (write_b != len_buff) {
		perror(NULL);
		return errno;
	}
	sleep(1);
	/* END READ */

	err = sem_wait(&rwl->mutex);
	if (err != 0)
		return err;

	rwl->read_count--;
	if (rwl->read_count == 0) {
		err = sem_post(&rwl->rw_mutex);
		if (err != 0)
			return err;
	}

	err = sem_post(&rwl->mutex);
	if (err != 0)
		return err;

	return 0;
}

void *read_write(void *arg) {
    char rw = *(char *)arg;
    if (rw == 'R') {
        int err = reader(&rwlock);
        if (err != 0) {
            exit(EXIT_FAILURE);
        }
    }
    else {
        int err = writer(&rwlock);
        if (err != 0) {
            exit(EXIT_FAILURE);
        }
    }
}


int main() {
	int cnt_threads = NUM_THREADS;
	int i, err;

	err = rwl_init(&rwlock);
	if (err != 0)
		return err;

	char *rw_array = (char *)malloc(cnt_threads * sizeof(char));
	if (!rw_array)
		exit(EXIT_FAILURE);

	for (i = 0; i < cnt_threads; ++i)
		if (scanf("%c", rw_array + i) != 1) {
			errno = EINVAL;
			perror(NULL);
			free(rw_array);
			return errno;
		}

	pthread_t *threads = (pthread_t *)malloc(cnt_threads * sizeof(pthread_t));
	if (!threads) {
		free(rw_array);
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < cnt_threads; ++i)
		if (rw_array[i] == 'R' || rw_array[i] == 'W') {
			if (pthread_create(threads + i, NULL, read_write, &rw_array[i])) {
				free(rw_array);
				free(threads);
		        	perror(NULL);
		        	return errno;
		    	}
		} else {
	        // the current character is not a reader or a writer
	        // we can ignore it or we can terminate the program
		free(rw_array);
		free(threads);
	        return -1;
	    }

	for (i = 0; i < cnt_threads; ++i)
		if (pthread_join(threads[i], NULL)) {
			free(rw_array);
			free(threads);
			perror(NULL);
            		return errno;
        	}

	free(rw_array);
	free(threads);

	err = rwl_destroy(&rwlock);
	if (err != 0)
		return err;

	return 0;
}
