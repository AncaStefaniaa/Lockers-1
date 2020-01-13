#include <stdio.h>

sempahore rw_mutex = 1;
semaphore mutex = 1;
int read_count = 0;

void writer() {
	do {
		wait(rw_mutex);

		/* WRITE */

		signal(rw_mutex);
	} while (true);
}


void reader() {
	do {
		wait(mutex);
		read_count++;
		if (read_count == 1)
			wait(rw_mutex);
		signal(mutex);

		/* READ */

		wait(mutex);
		read_count--;
		if (read_count == 0)
			signal(rw_mutex);
	} while (true);
}


int main() {
	
	return 0;
}
