#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

// Variable global que incrementan los hilos.
static long glob = 0;

sem_t semaforo;

void increment_glob(int loops)
{
    int loc, j;

    // incrementa glob
    for (j = 0; j < loops; j++) {
        sem_wait(&semaforo);
        loc = glob;
        loc++;
        glob = loc;
        sem_post(&semaforo);
    }
}

void *hiloRutina(void *arg) {
    int loops = *((int *) arg);
    increment_glob(loops);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int loops;

    // Controla numero de argumentos.
    if (argc != 2) {
        fprintf(stderr, "Uso: %s ciclos\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    loops = atoi(argv[1]);

    // Verifica argumentos.
    if (loops < 1) {
        fprintf(stderr, "Error: ciclos debe ser mayor a cero.\n");
        exit(EXIT_FAILURE);
    }
   
    pthread_t hilos[2];
    int i;

    sem_init(&semaforo, 0, 1);

    for(i = 0; i < 2; i++) {
       pthread_create(&hilos[i], NULL, hiloRutina, &loops);
    }

    for(i = 0; i < 2; i++) {
       pthread_join(hilos[i], NULL);
    }

    sem_destroy(&semaforo);
    printf("%ld\n", glob);

    exit(EXIT_SUCCESS);
}
