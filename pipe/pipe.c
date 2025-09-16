#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>

int main() {
    int pipefd[2];
    
    int r = pipe(pipefd);
    if (r < 0) {
        perror("Error in pipe creation");
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork error");
    }
    if (pid == 0) { // Hijo
        close(pipefd[0]); // Cierra el extremo de lectura
        write(pipefd[1], "hola", 4);
        close(pipefd[1]);
        exit(0); 
    }
    else {
        close(pipefd[1]); // Cierra el extremo de escritura
        char buffer[32];

        r = read(pipefd[0], buffer, 4); //Read
        buffer[r] = '\0';
        printf("Proceso padre lee: %s\n", buffer);
        close(pipefd[0]);
        exit(0);
    }

    return 0;
}