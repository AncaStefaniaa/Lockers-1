#include "semafor.h"

int sem_init (sem_t* sem, int mp, int initial_value) {

	sem->lock  = (int*) calloc(1, sizeof(int));
	sem->value = (int*) malloc(	  sizeof(int));

	if (!sem->lock) {
		return -1;
	}

	if (!sem->value) {
		return -1;
	}

	*sem->value = initial_value;

	return 0;

}


int sem_destroy (sem_t* sem) {

	free(sem->lock);
	free(sem->value);
	return 0;
}



int sem_wait (sem_t* sem) {

	if (*sem->lock != 0 && *sem->lock != 1) {
		return -1;
	}

	while (__sync_val_compare_and_swap(sem->lock, 0, 1) != 0) {
		;
	}

	if (*sem->value <= 0) {

		*sem->lock = 0;					//deblochezi lockul ca sa poata umbla altii

		while(*sem->value <= 0) {

			//trebuie sa luam lockul din nou deoarece l-am eliberat mai sus
			while (__sync_val_compare_and_swap(sem->lock, 0, 1) != 0) {
				;
			}

			if(*sem->value > 0) {
				--*sem->value;
				*sem->lock = 0;
				break;

			}

			*sem->lock = 0;

		}

	}
	else {//avem deja lockul

		--*sem->value;

		*sem->lock = 0;
	}

	return 0;

}




int sem_post (sem_t* sem) {

	if (*sem->lock != 0 && *sem->lock != 1) {
		return -1;
	}

	while (__sync_val_compare_and_swap(sem->lock, 0, 1) != 0) {
		;
	}

	++*sem->value;
	*sem->lock = 0;

	return 0;
}