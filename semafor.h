#ifndef semafor_H
#define semafor_H

typedef struct {
	int* lock;
	int* value;
} sem_t;

int sem_init 		(sem_t* sem, int mp, int initial_value);

int sem_destroy 	(sem_t* sem);

int sem_wait 		(sem_t* sem);

int sem_post 		(sem_t* sem);

#endif
