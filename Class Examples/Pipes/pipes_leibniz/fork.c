#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<math.h>
#include<stdlib.h>

#define NUM_ITER 1000000000

int main() {
    int pipefd[2];
    int r = pipe(pipefd);

    pid_t pid;
    pid = fork();

    if (pid == -1) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        // Child Process
        close(pipefd[0]);
        double sum_pares = 0;
        for (int i = 0; i < NUM_ITER; i += 2) {
            sum_pares += 4.0 / (2.0*i + 1.0);
        }
        write(pipefd[1], &sum_pares, sizeof(double));
        close(pipefd[1]);
        exit(0);
    } 

    // Parent process
    close(pipefd[1]);
    double sum_impares = 0;
    for (int i = 1; i < NUM_ITER; i += 2) {
        sum_impares += -4.0 / (2.0*i + 1.0);
    }

//    wait(NULL);
    double sum_pares;
    read(pipefd[0], &sum_pares, sizeof(double));
    close(pipefd[0]);   

    printf("impares = %.10lf\n", sum_impares);
    printf("pares = %.10lf\n", sum_pares);
    double pi = sum_pares + sum_impares;
    printf("PI = %.10lf\n", pi);
    return 0;
}