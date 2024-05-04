#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>  

unsigned int nSegundosMax;

void *hiloRutina(void *arg) 
{
    int numHilo = *(int *) arg;
    int segundos = rand() % nSegundosMax + 1;

    printf("Hilo %d: dormire %d\n", numHilo, segundos);

    sleep(segundos);

    pthread_exit((void *)(intptr_t)segundos);
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("Uso: %s <número de hilos> <cantidad de segundos a dormir como máximo>\n", argv[0]);
        return 1;
    }

    int nHilos= atoi(argv[1]);
    nSegundosMax = (unsigned int) atoi(argv[2]);

    if (nHilos <= 0) {
        printf("Número inválido de hilos.\n");
        return 1;
     }

    int i;
    pthread_t hilos[nHilos];
    int ids[nHilos];

    for(i = 0; i < nHilos; i++) {
       ids[i] = i + 1;
       pthread_create(&hilos[i], NULL, hiloRutina, &ids[i]);
    }

    int tiempos[nHilos];
    for(i = 0; i < nHilos; i++) {
       pthread_join(hilos[i], (void **)&tiempos[i]);
    }

    for (i = 0; i < nHilos; i++) {
       printf("Hilo %d termino: %d\n", ids[i], tiempos[i]);
    }

    exit(EXIT_SUCCESS);
}

