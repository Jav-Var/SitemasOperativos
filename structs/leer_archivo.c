#include <stdio.h>
#include <stdlib.h>

void mostrar_datos(FILE *fptr) {
    int c;
    while ((c = fgetc(fptr)) != EOF) {
        putchar(c);
    }
}

int main(void) {
    FILE *fptr = fopen("Datos.txt", "r"); 
    if (!fptr) {
        perror("fopen Datos.txt");
        return 1;
    }

    mostrar_datos(fptr);

    if (ferror(fptr)) perror("read error");
    fclose(fptr);
    return 0;
}
