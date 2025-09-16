#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<math.h>

#define NUM_ITER 1000000000

int main() {
    pid_t pid;
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        double sum_pares = 0;
        for (int i = 0; i < NUM_ITER; i += 2) {
            sum_pares += 4.0 / (2.0*i + 1.0);
        }
        FILE * fp_archivo_pares = fopen("pares.txt", "w");
        fprintf(fp_archivo_pares, "%.10lf\n", sum_pares);
        fclose(fp_archivo_pares);
    } else {
        double sum_impares = 0;
        for (int i = 1; i < NUM_ITER; i += 2) {
            sum_impares += -4.0 / (2.0*i + 1.0);
        }
        FILE * fp_archivo_impares = fopen("impares.txt", "w");
        fprintf(fp_archivo_impares, "%.10lf\n", sum_impares);
        fclose(fp_archivo_impares);
    }

    if (pid > 0) {
        wait(NULL);
        double sum_pares, sum_impares;
        FILE * fp_archivo_pares = fopen("pares.txt", "r");
        FILE * fp_archivo_impares = fopen("impares.txt", "r");
        fscanf(fp_archivo_pares, "%lf", &sum_pares);
        fscanf(fp_archivo_impares, "%lf", &sum_impares);
        printf("impares = %.10lf\n", sum_impares);
        printf("pares = %.10lf\n", sum_pares);
        double pi = sum_pares + sum_impares;
        printf("PI = %.10lf\n", pi);
    }
    return 0;
}