#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>

#define ITEMS       15
#define MAX_WORD    50

struct wordheap {
    int free;
    int items;
    int max_word;
    pthread_mutex_t mutex;
    sem_t full;
    sem_t empty;
    char heap[ITEMS][MAX_WORD];
};

typedef struct wordheap wordheap_t;

void usage(char *argv[])
{
    fprintf(stderr, "Uso: %s comando parametro\n", argv[0]);
    fprintf(stderr, "Comandos:\n");
    fprintf(stderr, "\t-w pila palabra: agrega palabra en la pila\n");
    fprintf(stderr, "\t-r pila pos: elimina la palabra de la pila.\n");
    fprintf(stderr, "\t-p pila: imprime la pila de palabras.\n");
    fprintf(stderr, "\t-c pila: crea una zona de memoria compartida con el nombre indicado donde almacena la pila.\n");
    fprintf(stderr, "\t-d pila: elimina la pila indicada.\n");
    fprintf(stderr, "\t-h imprime este mensaje.\n");
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    if (argv[1][0] != '-') {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    wordheap_t *stack;

    int shm_fd;

    int i;

    int pos;

    char option = argv[1][1];

    switch(option) {
        case 'w':
            shm_fd = shm_open(argv[2], O_RDWR, 0666);
            ftruncate(shm_fd, sizeof(wordheap_t));
            stack = mmap(0, sizeof(wordheap_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

            sem_wait(&stack->empty);
            pthread_mutex_lock(&stack->mutex);
    
            strcpy(stack->heap[stack->items], argv[3]);
            stack->items++;
    
            pthread_mutex_unlock(&stack->mutex);
            sem_post(&stack->full);
            close(shm_fd);
            break;
        case 'r':
            pos = atoi(argv[3]);
            char word[MAX_WORD];

            shm_fd = shm_open(argv[2], O_RDWR, 0666);
            ftruncate(shm_fd, sizeof(wordheap_t));
            stack = mmap(0, sizeof(wordheap_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

            sem_wait(&(stack->full));
            pthread_mutex_lock(&(stack->mutex));

            strcpy(word, stack->heap[pos]);
            for (i = pos; i < stack->items - 1; i++) {
              strcpy(stack->heap[i], stack->heap[i + 1]);
            }
            stack->items--;

            pthread_mutex_unlock(&(stack->mutex));
            sem_post(&(stack->empty));
            close(shm_fd);
            break;
        case 'p':
            shm_fd = shm_open(argv[2], O_RDWR, 0666);
            ftruncate(shm_fd, sizeof(wordheap_t));
            stack = mmap(0, sizeof(wordheap_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

            pthread_mutex_lock(&(stack->mutex));

            for (i = stack->items - 1; i >= 0; i--) {
               printf("%s\n", stack->heap[i]);
            } 

            pthread_mutex_unlock(&(stack->mutex));
            close(shm_fd);
            break;
        case 'c':
            
            shm_fd = shm_open(argv[2], O_CREAT | O_RDWR, 0666);
            ftruncate(shm_fd, sizeof(wordheap_t));
            stack = mmap(0, sizeof(wordheap_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
            close(shm_fd);

            (stack)->free = 0;
            (stack)->items = 0;
            (stack)->max_word = MAX_WORD;

            pthread_mutex_init(&((stack)->mutex), NULL);
            sem_init(&((stack)->full), 1, 0);
            sem_init(&((stack)->empty), 1, ITEMS);
            break;
        case 'd':
            shm_fd = shm_open(argv[2], O_CREAT | O_RDWR, 0666);
            stack = mmap(0, sizeof(wordheap_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

            sem_destroy(&stack->full);
            sem_destroy(&stack->empty);
            pthread_mutex_destroy(&stack->mutex);
            shm_unlink(argv[2]);
            break;
        case 'h':
            usage(argv);
            break;
        default:
            fprintf(stderr, "-%c: opción desconocida.\n", option);
            exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}
