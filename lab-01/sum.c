#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc > 1) {
       int i;
       int suma = 0;

       for(i = 1; i < argc; i++)
            suma = suma + atoi(argv[i]);

       printf("%i\n", suma);
          
    }

    // Termina la ejecución del programa.
    exit(EXIT_SUCCESS);
}
