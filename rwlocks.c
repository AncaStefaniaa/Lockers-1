#include <stdio.h>

sempahore rw_mutex = 1;
semaphore mutex = 1;
int read_count = 0;

void writer() {
	wait(rw_mutex);

	/* WRITE */
	FILE *file_to_write = fopen("my_file.txt", "a");
	fprintf("Data written by the write");
	fclose(f);
	/* END */

	signal(rw_mutex);
}


void reader() {
	wait(mutex);
	read_count++;
	if (read_count == 1)
		wait(rw_mutex);
	signal(mutex);

	/* READ */
	FILE *file_to_read = fopen("my_file.txt", "r");
	fprintf(file_to_read, "Data read by the reader!\n");
	sleep(1);
	fclose(file_to_read);

	wait(mutex);
	read_count--;
	if (read_count == 0)
		signal(rw_mutex);
}


int main() {

	return 0;
}
