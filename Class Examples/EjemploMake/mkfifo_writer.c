#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

// Debe ser el MISMO nombre que usa el lector
#define FIFO_PATH "/tmp/mi_fifo_especial"

int main() {
    int fd_fifo;

    // 1. Abrir el FIFO para ESCRITURA
    // IMPORTANTE: Esta llamada se BLOQUEARÁ
    // hasta que otro proceso (el lector) abra el mismo FIFO.
    printf("Buscando el lector...\n");
    fd_fifo = open(FIFO_PATH, O_WRONLY);
    if (fd_fifo == -1) {
        perror("open (write). ¿El lector está corriendo?");
        exit(EXIT_FAILURE);
    }
    printf("¡Lector encontrado! Escribe mensajes (o 'fin' para salir):\n");

    // 2. Bucle de escritura
    char buffer[128];
    while (1) {
        // Leer desde la entrada estándar (teclado)
        fgets(buffer, sizeof(buffer), stdin);
        
        // Quitar el salto de línea que añade fgets
        buffer[strcspn(buffer, "\n")] = 0;

        if (strcmp(buffer, "fin") == 0) {
            break; // Salir del bucle si el usuario escribe 'fin'
        }

        // 3. Escribir en el FIFO
        if (write(fd_fifo, buffer, strlen(buffer)) == -1) {
            perror("write");
            break; // Salir si el pipe se rompe
        }
    }

    // 4. Cerrar el FIFO
    // Al cerrar, el lector recibirá EOF (read() devolverá 0).
    printf("Cerrando el pipe...\n");
    close(fd_fifo);
    printf("Pipe cerrado. Saliendo.\n");

    return 0;
}