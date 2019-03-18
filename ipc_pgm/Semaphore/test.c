#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define SEM_MUTEX_KEY "/tmp/sem-mutex-key"
#define SEM_BUFFER_COUNT_KEY "/tmp/sem-buffer-count-key"

// Buffer data structures
#define MAX_BUFFERS 10
char buf [MAX_BUFFERS] [100];
int buffer_index;
int buffer_print_index;

int mutex_sem, buffer_count_sem;

void *producer (void *arg);

int main (int argc, char **argv)
{
    /* for semaphore */
    key_t s_key = 1;
    union semun  
    {
        int val;
        struct semid_ds *buf;
        ushort array [1];
    } sem_attr;
    pthread_t tid_producer [10];
    int i, r;

    // initialization
    buffer_index = buffer_print_index = 0;

    //  mutual exclusion semaphore, mutex_sem with an initial value 1.
    /* generate a key for creating semaphore  */
    if ((mutex_sem = semget (s_key, 1, 0660 | IPC_CREAT)) == -1) {
        perror ("semget"); exit (1);
    }
    // Giving initial value. 
    sem_attr.val = 1;        // unlocked
    if (semctl (mutex_sem, 0, SETVAL, sem_attr) == -1) {
        perror ("semctl SETVAL"); exit (1);
    }
    
    // counting semaphore, indicating the number of available buffers.
    /* generate a key for creating semaphore  */
    if ((buffer_count_sem = semget (s_key, 1, 0660 | IPC_CREAT)) == -1) {
        perror ("semget"); exit (1);
    }
    // giving initial values
    sem_attr.val = MAX_BUFFERS;    // MAX_BUFFERS are available
    if (semctl (buffer_count_sem, 0, SETVAL, sem_attr) == -1) {
        perror (" semctl SETVAL "); exit (1);
    }
    // Create 10 producer threads
    int thread_no [10];
    for (i = 0; i < 10; i++) {
        thread_no [i] = i;
        if ((r = pthread_create (&tid_producer [i], NULL, producer, (void *) &thread_no [i])) != 0) {
            fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
        }
    }
    // Wait for producers to terminate
    for (i = 0; i < 10; i++)
        if ((r = pthread_join (tid_producer [i], NULL)) == -1) {
            fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
        }
    
    // remove semaphores
    if (semctl (mutex_sem, 0, IPC_RMID) == -1) {
        perror ("semctl IPC_RMID"); exit (1);
    }
    if (semctl (buffer_count_sem, 0, IPC_RMID) == -1) {
        perror ("semctl IPC_RMID"); exit (1);
    }

    exit (0);
}

// producer: produce strings for printing
// There might be multiple producer threads
void *producer (void *arg)
{
    // Create 10 strings and terminate
    int i;
    int my_id = *((int *) arg);
    struct sembuf asem [1];
    int count = 0;

    asem [0].sem_num = 0;
    asem [0].sem_op = 0;
    asem [0].sem_flg = 0;

    for (i = 0; i < 10; i++) {
        // get a buffer: P (buffer_count_sem);
        asem [0].sem_op = -1;
        if (semop (buffer_count_sem, asem, 1) == -1) {
	    perror ("semop: buffer_count_sem"); exit (1);
        }
    
        /* There might be multiple producers. We must ensure that 
            only one producer uses buffer_index at a time.  */
        // P (mutex_sem);
        asem [0].sem_op = -1;
        if (semop (mutex_sem, asem, 1) == -1) {
	    perror ("semop: mutex_sem"); exit (1);
        }

	    // Critical section
            int j = buffer_index;
            buffer_index++;
            if (buffer_index == MAX_BUFFERS)
                buffer_index = 0;

        // Release mutex sem: V (mutex_sem)
        asem [0].sem_op = 1;
        if (semop (mutex_sem, asem, 1) == -1) {
	    perror ("semop: mutex_sem"); exit (1);
        }
    
	// Produce a string
        sprintf (buf [j], "Thread %d: %d\n", my_id, ++count);
	// Tell spooler that there is a string to print: V (spool_signal_sem);
   	printf("%s", *buf); 
        // Take a nap
        sleep (1);
    }
}
