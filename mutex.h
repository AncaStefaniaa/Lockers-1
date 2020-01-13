typedef struct {
    int available;
} lock;

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
