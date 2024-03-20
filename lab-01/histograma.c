#include <stdio.h>
#include <stdlib.h>

int longitudPalabraMasGrandeArreglo(char palabras[], int iFin)
{
    int contLongitudPalabra = 0;
    int longPalabraMasGrande = 0;
    
    int i;
    for (i = 0; i < iFin; i++)
    {
        if (palabras[i] != ' ' && palabras[i] != '\t')
            contLongitudPalabra = contLongitudPalabra + 1;
        else if (contLongitudPalabra > longPalabraMasGrande)
            longPalabraMasGrande = contLongitudPalabra;

        if (palabras[i] == ' '|| palabras[i] == '\t')
            contLongitudPalabra = 0;
    }

    return longPalabraMasGrande;
}

void histograma(char palabras[], int iFin)
{
    int longPalabraMasGrande = longitudPalabraMasGrandeArreglo(palabras, iFin);

    int j;
    for (j = 1; j <= longPalabraMasGrande; j++)
    {
        int contLongitudPalabra = 0;
        printf("%d ", j);

        int i;
        for (i = 0; i < iFin; i++)
        {
            if (palabras[i] != ' ' && palabras[i] != '\t')
                contLongitudPalabra = contLongitudPalabra + 1;
            else if (contLongitudPalabra == j)
                printf("*");

            if (palabras[i] == ' ' || palabras[i] == '\t')
                contLongitudPalabra = 0;
        }
        printf("\n");
    }
}

int main()
{
    int c = 0;
    int i = 0;
    char palabras[2000];

    while ((char)(c) != EOF)
    {
        c = getc(stdin);

        // mientras no se aprete ENTER se guarda el texto en un arreglo auxiliar
        if ((char)(c) != '\n')
        {
            palabras[i] = (char)(c);
            i = i + 1;
        }
        else
        {
            // separa cada texto ingresado con un espacio
            palabras[i] = ' ';
            i = i + 1;
           
        }
    }

    printf("\n");
    histograma(palabras, i);
    
    // Termina la ejecución del programa.
    exit(EXIT_SUCCESS);
}
