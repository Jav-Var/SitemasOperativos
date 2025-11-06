#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h> // Para Memoria Compartida (Shared Memory)
#include <sys/sem.h> // Para Semáforos System V
#include <sys/wait.h>

// Definición de las operaciones del semáforo
// struct sembuf { short sem_num; short sem_op; short sem_flg; };

// Operación de 'lock' (P) -> Decrementa el semáforo (espera si es 0)
struct sembuf op_lock = {0, -1, 0}; // Semáforo 0, Operación -1, Sin flags

// Operación de 'unlock' (V) -> Incrementa el semáforo
struct sembuf op_unlock = {0, 1, 0}; // Semáforo 0, Operación +1, Sin flags


// Clave para identificar los recursos IPC (Semáforo y Memoria)
// ftok genera una clave única basada en un path y un id.
#define IPC_KEY_PATH "."
#define IPC_KEY_ID 'S'


int main() {
    int shmid, semid;
    int *contador_compartido;
    key_t key;

    // 1. Obtener una clave única para los recursos IPC
    key = ftok(IPC_KEY_PATH, IPC_KEY_ID);
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // 2. Crear el segmento de Memoria Compartida (para 1 entero)
    shmid = shmget(key, sizeof(int), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // 3. Crear el Semáforo (un solo semáforo en el conjunto)
    semid = semget(key, 1, 0666 | IPC_CREAT);
    if (semid == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // 4. Inicializar el Semáforo a 1 (estado "abierto" o "unlocked")
    // (semctl con SETVAL solo funciona la primera vez)
    union semun { int val; struct semid_ds *buf; unsigned short *array; } arg;
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("semctl (SETVAL)");
        // No salimos, quizás ya estaba inicializado.
    }

    // 5. "Adjuntar" la memoria compartida al espacio de memoria de este proceso
    // Ahora 'contador_compartido' es un puntero a esa memoria.
    contador_compartido = (int *)shmat(shmid, NULL, 0);
    if (contador_compartido == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    
    // Inicializar el contador a 0
    *contador_compartido = 0;
    printf("Contador inicializado a 0.\n");


    // 6. Crear un proceso hijo
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // --- CÓDIGO DEL HIJO ---
        printf("[Hijo %d] Iniciando...\n", getpid());
        for (int i = 0; i < 5; i++) {
            // --- INICIO SECCIÓN CRÍTICA ---
            semop(semid, &op_lock, 1); // 7. Pedir el candado (Lock)
            
            // (Simular trabajo)
            int val_actual = *contador_compartido;
            printf("[Hijo %d] Leyó %d\n", getpid(), val_actual);
            usleep(10000); // Simular un retardo
            *contador_compartido = val_actual + 1;
            printf("[Hijo %d] Escribió %d\n", getpid(), *contador_compartido);

            semop(semid, &op_unlock, 1); // 8. Soltar el candado (Unlock)
            // --- FIN SECCIÓN CRÍTICA ---
            usleep(5000);
        }
        printf("[Hijo %d] Terminando.\n", getpid());
        exit(0);

    } else {
        // --- CÓDIGO DEL PADRE ---
        printf("[Padre %d] Iniciando...\n", getpid());
        for (int i = 0; i < 5; i++) {
            // --- INICIO SECCIÓN CRÍTICA ---
            semop(semid, &op_lock, 1); // 7. Pedir el candado (Lock)
            
            // (Simular trabajo)
            int val_actual = *contador_compartido;
            printf("[Padre %d] Leyó %d\n", getpid(), val_actual);
            usleep(8000); // Simular un retardo
            *contador_compartido = val_actual + 1;
            printf("[Padre %d] Escribió %d\n", getpid(), *contador_compartido);

            semop(semid, &op_unlock, 1); // 8. Soltar el candado (Unlock)
            // --- FIN SECCIÓN CRÍTICA ---
            usleep(7000);
        }
        printf("[Padre %d] Terminando.\n", getpid());
    }

    // 9. Esperar a que el hijo termine
    wait(NULL);

    printf("\n--- RESULTADO FINAL ---\n");
    printf("Valor final del contador: %d\n", *contador_compartido);
    printf("(Debería ser 10. Si no lo es, la sincronización falló).\n");

    // 10. Limpieza de recursos IPC
    shmdt(contador_compartido); // Des-adjuntar memoria
    shmctl(shmid, IPC_RMID, NULL); // Marcar memoria para borrar
    semctl(semid, 0, IPC_RMID, NULL); // Marcar semáforo para borrar

    return 0;
}