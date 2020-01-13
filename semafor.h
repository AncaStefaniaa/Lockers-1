typedef struct {
	int *lock;
	int value;
	//pthread_t *list;
} semaphore;

int sem_wait (semaphore *sem){

	if (*sem->lock != 0 && *sem->lock != 1)
		return -1;

	while (__sync_val_compare_and_swap(sem->lock, 0, 1) != 1);

	if(sem->value <= 0){
		*sem->lock = 0;
		while(sem->value <= 0);
	}
	else{
		--sem->value;
		*sem->lock = 0;
	}
	return 0;

}


int sem_init (semaphore *sem, int multi_processing, int initial_value){

	sem->lock = (int*) calloc(1, sizeof(int));
	if (!sem->lock){
		return -1;
	}


	while (__sync_val_compare_and_swap(sem->lock, 0, 1) != 0);

	sem->value = initial_value;
	*sem->lock = 0;
	return 0;

}


int sem_post (semaphore *sem){
	if(*sem->lock != 0 && *sem->lock != 1)
		return -1;

	while (__sync_val_compare_and_swap(sem->lock, 0, 1) != 0);

	++sem->value;
	*sem->lock = 0;

	return 0;
}

