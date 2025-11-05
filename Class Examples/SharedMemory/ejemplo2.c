/*
  c_shared_alt_sum.c
  Approximate S = sum_{n=1}^infty (-1)^n x^n / n by truncating at K terms,
  computed in parallel by M worker processes writing partial sums into shared memory.

  Usage:
    ./c_shared_alt_sum K x M
    K : number of terms to use (positive integer)
    x : base (long double; series converges for |x| < 1, and conditionally at x = 1; diverges at x = -1)
    M : number of worker processes (positive integer)

  Example:
    ./c_shared_alt_sum 1000000 0.9 4
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <math.h>
#include <errno.h>
#include <string.h>

#define _GNU_SOURCE

#ifndef MAP_ANONYMOUS   // for systems that use MAP_ANON instead
#define MAP_ANONYMOUS MAP_ANON
#endif

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr,
                "Usage: %s K x M\n"
                "  K: number of terms (positive integer)\n"
                "  x: base (long double)\n"
                "  M: number of worker processes (positive integer)\n",
                argv[0]);
        return 1;
    }

    long K = atol(argv[1]);
    long double x = strtold(argv[2], NULL);
    int M = atoi(argv[3]);

    if (K <= 0 || M <= 0) {
        fprintf(stderr, "K and M must be positive.\n");
        return 1;
    }

    // Domain checks
    if (fabsl(x) > 1.0L) {
        fprintf(stderr, "Series diverges for |x| > 1 (|x| = %.6Lf). Aborting.\n", fabsl(x));
        return 1;
    }
    if (x == -1.0L) {
        fprintf(stderr, "Series diverges for x = -1 (harmonic series). Aborting.\n");
        return 1;
    }

    // Create shared memory for M partial sums
    size_t bytes = (size_t)M * sizeof(long double);
    long double *partials = mmap(NULL, bytes, PROT_READ | PROT_WRITE,
                                 MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (partials == MAP_FAILED) {
        fprintf(stderr, "mmap failed: %s\n", strerror(errno));
        return 1;
    }

    for (int i = 0; i < M; ++i) partials[i] = 0.0L;

    // Distribute contiguous chunks
    long per = K / M;
    long rem = K % M;

    for (int worker = 0; worker < M; ++worker) {
        pid_t pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork failed: %s\n", strerror(errno));
            break;
        } else if (pid == 0) {
            // Child process
            long start = (long)worker * per + (worker < rem ? worker : rem) + 1;
            long count = per + (worker < rem ? 1 : 0);
            long end = start + count - 1;
            long double partial = 0.0L;

            if (count > 0) {
                long double x_pow = powl(x, (long double)start);
                long double sign = (start % 2 == 0) ? 1.0L : -1.0L;
                for (long n = start; n <= end; ++n) {
                    partial += sign * x_pow / (long double)n;
                    x_pow *= x;
                    sign = -sign;
                }
            }

            partials[worker] = partial;
            _exit(0);
        }
    }

    // Parent process waits for all children
    int status;
    while (wait(&status) > 0) {}

    // Combine partials
    long double total = 0.0L;
    for (int i = 0; i < M; ++i) total += partials[i];

    printf("Distributed result with %d workers (K=%ld, x=%.18Lf):\n", M, K, x);
    printf("  sum_{n=1}^{K} (-1)^n x^n / n  = %.18Lf\n", total);

    // Direct computation for verification
    long double direct = 0.0L;
    long double xp = x;
    for (long n = 1; n <= K; ++n) {
        long double sign = (n % 2 == 0) ? 1.0L : -1.0L;
        direct += sign * xp / (long double)n;
        xp *= x;
    }
    printf("Direct single-process result:          %.18Lf\n", direct);
    printf("Absolute difference:                   %.18Lf\n", fabsl(total - direct));

    // Closed-form for |x| < 1
    if (fabsl(x) < 1.0L) {
        long double closed = -logl(1.0L + x);
        printf("Closed-form for infinite sum:          %.18Lf ( -log(1+x) )\n", closed);
        printf("Difference to closed-form (truncated): %.18Lf\n", fabsl(total - closed));
    }

    if (munmap(partials, bytes) != 0) {
        fprintf(stderr, "munmap failed: %s\n", strerror(errno));
    }

    return 0;
}
