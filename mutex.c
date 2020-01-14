#include "mutex.h"
#include <stdio.h>



int lock_init(lock *mutex) {
	mutex->available = 0;
}

int lock_aquire(lock *mutex) {
	while (__sync_val_compare_and_swap(&mutex->available, 0, 1) != 0);
	return 0;
}

int lock_release(lock *mutex) {
	mutex->available = 0;
	return 0;
}

int mutex_destroy(lock *mutex){
	printf("Mutexul a fost distrus");
	return 0;
}

void __attribute__((constructor)) initLibrary(void){
	printf("Library is initialized\n");
}

void __attribute__((destructor)) cleanUpLibrary(void){
	printf("Library is exited\n");
}
