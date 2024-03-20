#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void imprimirPalabrasArreglo(char arrayPalabras[], int iFin)
{
    int espacio = 0;

    if (arrayPalabras[0] == ' ' || arrayPalabras[0] == '\t')
       espacio = 1;

    int i;
    for (i = 0; i < iFin; i++)
    {
        if (arrayPalabras[i] != ' ' && arrayPalabras[i] != '\t')
        {
            printf("%c", arrayPalabras[i]);
            if (espacio == 1)
                espacio = 0;
        }
        else if (espacio == 0)
        {
            printf("\n");
            espacio = 1;
        }
    }

    // ultimo salto de linea si no hay espacio al final del texto
    if (arrayPalabras[iFin - 1] != ' ' && arrayPalabras[iFin - 1] != '\t')
        printf("\n");
}

int main()
{
    int c = 0;
    char palabras[1000];

    while ((char)(c) != EOF)
    {
        int i = 0;
        do
        {
            c = getc(stdin);
            palabras[i] = (char)(c);
            i = i + 1;
        } while ((char)(c) != '\n' && (char)(c) != EOF);

        if ((char)(c) != EOF)
        {
            // se borra el caracter ENTER
            palabras[i - 1] = '\0';
            i = i - 1;
            imprimirPalabrasArreglo(palabras, i);
        }
    }

    printf("\n");

    // Termina la ejecución del programa.
    exit(EXIT_SUCCESS);
}
