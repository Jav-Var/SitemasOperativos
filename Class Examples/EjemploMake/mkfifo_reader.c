#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

// El nombre del pipe en el sistema de archivos
#define FIFO_PATH "/tmp/mi_fifo_especial"

int main() {
    int fd_fifo;
    char buffer[128];
    int r;

    // 1. Crear el FIFO (Tubería con Nombre)
    // mkfifo devuelve 0 si tiene éxito.
    // Si falla, comprobamos si es porque 'File exists' (EEXIST),
    // lo cual no es un error para nosotros, significa que otro
    // proceso (quizás una ejecución anterior) ya lo creó.
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        if (errno != EEXIST) {
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
        printf("FIFO ya existe, abriendo...\n");
    } else {
        printf("FIFO creado en %s\n", FIFO_PATH);
    }

    // 2. Abrir el FIFO para LECTURA
    // IMPORTANTE: Esta llamada se BLOQUEARÁ
    // hasta que otro proceso abra el mismo FIFO para ESCRITURA.
    printf("Esperando a que un escritor se conecte...\n");
    fd_fifo = open(FIFO_PATH, O_RDONLY);
    if (fd_fifo == -1) {
        perror("open (read)");
        exit(EXIT_FAILURE);
    }
    printf("¡Escritor conectado!\n");

    // 3. Bucle de lectura
    // read() también es BLOQUEANTE. Esperará hasta que haya datos.
    while ((r = read(fd_fifo, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[r] = '\0'; // Asegurar fin de string
        printf("Recibido: [%s]\n", buffer);
    }

    // 4. Fin de la comunicación
    // read() devuelve 0 cuando el escritor cierra su extremo del pipe (EOF).
    if (r == 0) {
        printf("El escritor ha cerrado la conexión.\n");
    } else {
        perror("read"); // r < 0, fue un error
    }

    // 5. Limpieza
    close(fd_fifo);
    unlink(FIFO_PATH); // Borrar el archivo FIFO del sistema
    printf("FIFO cerrado y eliminado. Saliendo.\n");

    return 0;
}

// Nota: para compilar se utiliza: gcc mkfifo_reader.c -o reader 
// y para ejecutar se debe correr primero el lector en una terminal:
// ./reader
// Luego, en otra terminal se compila: gcc mkfifo_writer.c -o writer
// Luego, en otra terminal, se ejecuta el escritor: ./writer

/*Sección 1: Comandos de usuario (ej. ls, grep).

Sección 2: Llamadas al Sistema (El Kernel. Ej: fork, pipe, read, write, open, shmget).

Sección 3: Funciones de Biblioteca (La Glibc. Ej: printf, malloc, fopen, pthread_create).

Sección 7: Conceptos, estándares y protocolos (Ej: socket, pthreads, fifo).
*/
/*Si escribes man pipe:

    Probablemente te salga pipe(1), que es el comando | de la shell.

    Tú necesitas la llamada al sistema: man 2 pipe

Si buscas printf:

    Necesitas la función de biblioteca: man 3 printf

Si buscas socket:

    La llamada es man 2 socket.

    El concepto general (protocolos, etc.) es man 7 socket.

Si buscas hilos (Threads):

    El concepto POSIX es man 7 pthreads.

    La función específica es man 3 pthread_create*/
//