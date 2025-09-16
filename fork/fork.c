#include<unistd.h>
#include<stdio.h>

int main() {
    pid_t pid;
    pid = fork();
    printf("PID: %d\n", pid);
    if (pid == 0) {
        printf("AAAa\n"); //
    } else {
        printf("ola 2\n"); //
    }
}