#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>        
#include <fcntl.h>  
#include <unistd.h>
#include<string.h>

#define HEIGHT  25  // Altura en caracteres de la pizarra
#define WIDTH   25  // Ancho en caracteres de la pizarra

struct canvas {
    char canvas[HEIGHT*WIDTH];
};

typedef struct canvas canvas_t;

void usage(char *argv[])
{
    fprintf(stderr, "Uso: %s comando parametro\n", argv[0]);
    fprintf(stderr, "Comandos:\n");
    fprintf(stderr, "\t-w canvas mensaje x y [h|v]: escribe el mensaje en el canvas indicado en la posición (x,y) de manera [h]orizontal o [v]ertical.\n");
    fprintf(stderr, "\t-p canvas: imprime el canvas indicado.\n");
    fprintf(stderr, "\t-c canvas: crea un canvas con el nombre indicado.\n");
    fprintf(stderr, "\t-d canvas: elimina el canvas indicado.\n");
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

    int fd;

    canvas_t *canvas ;

    int i;

    char option = argv[1][1];

    switch(option) {
        case 'w':
            printf("Escribe %s en el canvas %s en la posición (%d, %d).\n", argv[3], argv[2], atoi(argv[4]), atoi(argv[5]));
            fd = shm_open(argv[2], O_RDWR, 0666);
            ftruncate(fd, sizeof(canvas_t));
            canvas = mmap(NULL, sizeof(canvas_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

            char *message = argv[3];
            int x = atoi(argv[4]);
            int y = atoi(argv[5]);
            char mode = argv[6][0];

            if (mode == 'h') {
         
            for (i = 0; i < strlen(message); i++) 
                canvas->canvas[y * WIDTH + x + i] = message[i];

            } else if (mode == 'v') {
            
            for (i = 0; i < strlen(message); i++)  
                canvas->canvas[(y + i) * WIDTH + x] = message[i];
  
            }           

            break;
        case 'p':
            printf("Imprime canvas.\n");
            fd = shm_open(argv[2], O_CREAT | O_RDWR, 0666);
            ftruncate(fd, sizeof(canvas_t));
            canvas = mmap(NULL, sizeof(canvas_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

            int j;
            for (i = 0; i < HEIGHT; i++) {
                for (j = 0; j < WIDTH; j++) {
                   putchar(canvas->canvas[i * WIDTH + j]);
                }
                putchar('\n');
            }
            break;
        case 'c':
            printf("Crea canvas.\n");
            fd = shm_open(argv[2], O_CREAT | O_RDWR, 0666);
            close(fd);
            break;
        case 'd':
            printf("Borra canvas.\n");
            shm_unlink(argv[2]);
            break;
        case 'h':
            usage(argv);
            break;
        default:
            fprintf(stderr, "Comando desconocido\n");
            exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
