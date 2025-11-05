#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<sys/shm.h>
#include<pthread.h>


int main() {
    int key = 123;    
    int shmId = shmget(key, sizeof(double), 0666|IPC_CREAT);
    if (shmId == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    double *qp = shmat(shmId, 0, 0);
    *qp = 2.7182818;
    printf("%f\n", *qp);

    shmdt(qp);
    return 0;
}