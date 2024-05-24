#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> 
#include <mqueue.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define USERNAME_MAXSIZE    15  // Máximo tamaño en caracteres del nombre del remitente.
#define TXT_SIZE            100 // Máximo tamaño del texto del mensaje.

/**
 * Estructura del mensaje:
 * - sender: nombre del usuario que envió el mensaje.
 * - text: texto del mensaje.
 */
struct msg {
    char sender[USERNAME_MAXSIZE];
    char text[TXT_SIZE];
};

typedef struct msg msg_t;

/**
 * Imprime información acerca del uso del programa.
 */
void usage(char *argv[])
{
    fprintf(stderr, "Uso: %s comando parametro\n", argv[0]);
    fprintf(stderr, "Comandos:\n");
    fprintf(stderr, "\t-s queue mensaje: escribe el mensaje en queue.\n");
    fprintf(stderr, "\t-r queue: imprime el primer mensaje en queue.\n");
    fprintf(stderr, "\t-a queue: imprime todos los mensaje en queue.\n");
    fprintf(stderr, "\t-l queue: vigila por mensajes en queue.\n");
    fprintf(stderr, "\t-c queue: crea una cola de mensaje queue.\n");
    fprintf(stderr, "\t-d queue: elimina la cola de mensajes queue.\n");
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

    mqd_t mq;
    struct msg message;
    struct mq_attr attr;

    char option = argv[1][1];

    switch(option) {
        case 's':
            printf("Enviar %s a la cola %s\n", argv[3], argv[2]);
            struct msg new_message;
    
            mq = mq_open(argv[2], O_WRONLY);

            strncpy(new_message.sender, getlogin(), TXT_SIZE - 1);
            strncpy(new_message.text, argv[3], TXT_SIZE - 1);

            mq_send(mq, (const char *)&new_message, sizeof(struct msg), 0);

            mq_close(mq);
            break;
        case 'r':
            printf("Recibe el primer mensaje en %s\n", argv[2]);
    
            mq = mq_open(argv[2], O_RDONLY | O_NONBLOCK);

            if (mq == (mqd_t)-1) { 
              perror("mq_open");
              exit(EXIT_FAILURE);
            }

            msg_t received_message;
            if(mq_receive(mq, (char *)&received_message, sizeof(msg_t), NULL) == -1){
              perror("mq_receive");
              exit(EXIT_FAILURE);
            }

            mq_receive(mq, (char *)&message, sizeof(struct msg), NULL);

            printf("Mensaje recibido de %s: %s\n", message.sender, message.text);

            mq_close(mq);
            break;
        case 'a':
            printf("Imprimir todos los mensajes en %s\n", argv[2]);
            ssize_t bytes_read;
    
            mq = mq_open(argv[2], O_RDONLY);

            while ((bytes_read = mq_receive(mq, (char *)&message, sizeof(struct msg), NULL)) > 0) {
               printf("De %s: %s\n", message.sender, message.text);
            }

            mq_close(mq);
            break;
        case 'l':
            printf("Escucha indefinidamente por mensajes\n");
    
            mq = mq_open(argv[2], O_RDONLY);

            mq_receive(mq, (char *)&message, sizeof(struct msg), NULL);

            mq_close(mq);
            break;
        case 'c':
            printf("Crea la cola de mensajes %s\n", argv[2]);
            attr.mq_flags = 0;
            attr.mq_maxmsg = 10; // Máximo número de mensajes en la cola
            attr.mq_msgsize = sizeof(struct msg); // Tamaño máximo de cada mensaje

            mq = mq_open(argv[2], O_CREAT, 0664, &attr);
            mq_close(mq);
            break;
        case 'd':
            printf("Borra la cola de mensajes %s\n", argv[2]);
            mq_unlink(argv[2]);
            break;
        case 'i':
            mq = mq_open(argv[2], O_RDONLY);

            mq_getattr(mq, &attr);

            printf("Información de la cola %s:\n", argv[2]);
            printf("Máximo número de mensajes en la cola: %ld\n", attr.mq_maxmsg);
            printf("Tamaño máximo de cada mensaje: %ld\n", attr.mq_msgsize);
            printf("Número actual de mensajes en la cola: %ld\n", attr.mq_curmsgs);

            mq_close(mq);
            break;
        case 'h':
            usage(argv);
            break;
        default:
            fprintf(stderr, "Comando desconocido: %s\n", argv[1]);
            exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}
