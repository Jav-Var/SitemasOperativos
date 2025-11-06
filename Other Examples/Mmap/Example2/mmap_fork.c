/* mmap_fork_example.c */
#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

int main(void) {
    const size_t SIZE = 4096;

    /* MAP_ANONYMOUS + MAP_SHARED: memory not backed by a file, but shared between related processes */
    char *shared = mmap(NULL, SIZE, PROT_READ | PROT_WRITE,
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared == MAP_FAILED) { perror("mmap"); return 1; }

    pid_t pid = fork();
    if (pid == -1) { perror("fork"); munmap(shared, SIZE); return 1; }

    if (pid == 0) {
        /* child */
        sleep(1); /* wait a bit to let parent write */
        printf("Child reads: '%s'\n", shared);
        /* modify it */
        strncat(shared, " -- child was here", SIZE - strlen(shared) - 1);
        munmap(shared, SIZE);
        return 0;
    } else {
        /* parent */
        strncpy(shared, "Hello from parent", SIZE - 1);
        printf("Parent wrote: '%s'\n", shared);

        wait(NULL); /* wait for child */
        printf("Parent sees after child: '%s'\n", shared);

        munmap(shared, SIZE);
    }
    return 0;
}
