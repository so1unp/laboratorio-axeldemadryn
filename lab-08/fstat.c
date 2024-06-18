#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <archivo>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *archivo = argv[1];
    struct stat info;

    if (stat(archivo, &info) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    printf("ID del usuario propietario: %d\n", info.st_uid);

    // Tipo de archivo
    if (S_ISREG(info.st_mode))
        printf("Tipo de archivo: Archivo regular\n");
    else if (S_ISDIR(info.st_mode))
        printf("Tipo de archivo: Directorio\n");
    else if (S_ISLNK(info.st_mode))
        printf("Tipo de archivo: Enlace simbólico\n");
    else
        printf("Tipo de archivo: Desconocido\n");

    printf("Número de i-nodo: %ld\n", (long) info.st_ino);
    printf("Tamaño en bytes: %ld\n", (long) info.st_size);
    printf("Número de bloques que ocupa en disco: %ld\n", (long) info.st_blocks);

    // Tiempo de última modificación y acceso
    printf("Tiempo de última modificación: %s", ctime(&info.st_mtime));
    printf("Tiempo de último acceso: %s", ctime(&info.st_atime));

    return 0;
}
