#include<stdio.h>

struct datos {
        char nombre[32];
        char apellido[32];
        int edad;
        int estatura;
    };


void imprimir_datos(struct datos p) {
    printf(
        "Nombre: %s\n"
        "Apellido: %s\n"
        "Edad: %d años\n"
        "Estatura %d cm\n",
        p.nombre, p.apellido, p.edad, p.estatura
    );
}

void escribir_datos(struct datos p, FILE * fptr) {
    fprintf(
        fptr,
        "Nombre: %s\n"
        "Apellido: %s\n"
        "Edad: %d años\n"
        "Estatura %d cm\n",
        p.nombre, p.apellido, p.edad, p.estatura
    );
}

int main() {
    struct datos persona1;

    printf("Ingrese su nombre:\n> ");
    scanf("%s", persona1.nombre);
    printf("Ingrese su apellido:\n> ");
    scanf("%s", persona1.apellido);
    printf("Ingrese su edad:\n> ");
    scanf("%d", &persona1.edad);
    printf("Ingrese su estatura en cm:\n> ");
    scanf("%d", &persona1.estatura);

    FILE *fp1 = fopen("Datos.txt", "w");

    imprimir_datos(persona1);
    escribir_datos(persona1, fp1);

    fclose(fp1);

    return 0;
}