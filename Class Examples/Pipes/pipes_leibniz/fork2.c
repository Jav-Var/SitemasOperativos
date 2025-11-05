#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

int main() {
    int pipefd_1[2], pipefd_2[2];
    int status_1 = pipe(pipefd_1);
    int status_2 = pipe(pipefd_2);

    pid_t pid_1, pid_2;
    int pipefd_1[2], pipefd_2[2]; 

    int status;

    pid_1 = fork();
    if (pid_1 < 0) {
        perror("aaaaaaa");
        exit(-1);
    }

    if (pid_1 == 0) {
        // Hijo 1 
        exit(0);
    }
    
    pid_2 = fork();
    if (pid_2 < 0) {
        perror("aaaa");
        exit(-1);
    }

    if (pid_2 == 0) {
        // Hijo 2
        exit(0);       
    }

    // Padre



    return 0;
}