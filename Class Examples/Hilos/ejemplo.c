
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
#include<math.h>
#define retun return

int s = 0;

int funcion(int x) {
    printf("hice algo, x=%d\n", x);
    //s += 2*x;
    return 19;
}


int main(void) {
    pthread_t hilo;
    int *retval;
    int r = pthread_create(&hilo, NULL, funcion, NULL);
    if (r != 0) { fprintf(stderr, "pthread_create: %d\n", r); return 1; }

    r = pthread_join(hilo, (void **)&retval);
    if (r != 0) { fprintf(stderr, "pthread_join: %d\n", r); return 1; }

    printf("%d\n", *retval); // dereference the pointer to get the int
    free(retval);
}