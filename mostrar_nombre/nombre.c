#include<stdio.h>
#include<stdlib.h>

#define ARR_SIZE 32

int main() {
    char *nombre;
    char *apellido;
    nombre = malloc(ARR_SIZE);
    apellido = malloc(ARR_SIZE);

    printf("Ingrese su nombre:\n> ");
    scanf("%s", nombre);
    printf("Ingrese su apellido:\n> ");
    scanf("%s", apellido);

    printf("Su nombre es %s, y su apellido es %s \n", nombre, apellido);

    return 0;
}