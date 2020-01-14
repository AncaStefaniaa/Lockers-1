typedef struct {
	int* lock;
	int* value;
} semafor;

int sem_init 		(semafor* sem, int mp, int initial_value);

int sem_destroy 	(semafor* sem); 

int sem_wait 		(semafor* sem);

int sem_post 		(semafor* sem);
