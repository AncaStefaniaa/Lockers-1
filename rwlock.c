#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

typedef struct {
	sempahore rw_mutex;
	semaphore mutex;
	int read_count;
} rwlock;


int rwl_init(rwlock *rwl) {
	int err;
	
	err = sem_init(rwl->rw_mutex, 0, 1);
	if (err != 0) 
		return err;

	err = sem_init(rwl->mutex, 0, 1);
	if (err != 0)
		return err;

	rwl->read_count = 0;
	
	return 0;
}

int rwl_destroy(rwlock *rwl) {
	int err;
	
	err = sem_destroy(&rwl->rw_mutex);
	if (err != 0)
		return err;
	
	err = sem_destroy(&rwl->mutex);
	if (err != 0)
		return err;

	return 0;
}
	

void writer(rwlock *rwl) {
	sem_wait(rwl->rw_mutex);

	/* WRITE */
	int fd_out = open("my_file.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd_out == -1) {
		perror(NULL);
		return errno;
	}

	const char *buff = "Data written by the writer\n";
	size_t len_buff = strlen(buf);
	ssize_t writeb = write(fd_out, buff, len_buff);
	if (write_b != len_buff) {
		perror(NULL);
		return errno;
	}

	if (close(fd_out) == -1) {
		perror(NULL);
		return errno;
	}
	/* END */

	sem_post(rwl->rw_mutex);
}


void reader() {
	sem_wait(mutex);
	read_count++;
	if (read_count == 1)
		sem_wait(rw_mutex);
	sem_signal(mutex);

	/* READ */
	/* Acelasi comentariu ca in cazul write, de folosit open si read in loc de fopen si fprintf */
	int fd_out = open("my_file.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd_out == -1) {
		perror(NULL);
		return errno;
	}
	
	char *buff = "Data read by the reader!\n";
	size_t len_buff = strlen(buf);
	ssize_t writeb = write(fd_out, buff, len_buff);
	if (write_b != len_buff) {
		perror(NULL);
		return errno;
	}
	

	sleep(1);
	
	if (close(fd_out) == -1) {
		perror(NULL);
		return errno;
	}

	sem_wait(mutex);
	read_count--;
	if (read_count == 0)
		sem_signal(rw_mutex);
	sem_signal(mutex);
}


int main() {

	/*Cum nu mi-a zis nimic concludent, creati aici cateva threaduri reader si cateva writer*/
	/*Apoi faceti join pentru toate si verificati ca nu s-au transmis mesaje incomplete sau interpuse de exemplu "Data read Data read by the reader!by the reader!" daca mutex-ul functioneaza mesajele se vor trimite complet inainte ca alt thread cititor sau scriitor sa intervina*/
	return 0;
}
