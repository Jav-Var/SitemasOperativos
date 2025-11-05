#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

#define MAX_PROCESOS 20
#define NUM_HILOS 10

struct datos_tipo {
    int dato;
    int p;
};

pthread_mutex_t mutex;

void *proceso(void *datos) { 
    struct datos_tipo *datos_proceso;
    datos_proceso = (struct datos_tipo *) datos;

    int a,p;
    a = datos_proceso -> dato;
    p = datos_proceso -> p;

    /* Bloquea el mutex */
    if (pthread_mutex_lock(&mutex) != 0) {
        perror("mutex unlock");
        exit(EXIT_FAILURE);
    }

    /* Imprime dato p + 1 veces */
    for (int i = 0; i <= p; i++) {
        printf("%i ", a);
    }
    fflush(stdout);
    sleep(1);

    /* Imprime p + 1 lineas */
    for (int i = 0; i <= p; i++) {
        printf("- ");
    }
    fflush(stdout);
    
    /* Desbloquea el mutex */
    if(pthread_mutex_unlock(&mutex) != 0) {
        perror("mutex unlock");
        exit(EXIT_FAILURE);
    }

    return NULL;
}

int main() {
    char *valor_devuelto;

    /* Inicializa el mutex */
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("mutex");
        exit(EXIT_FAILURE);
    }

    /* Variables para los hilos */
    struct datos_tipo hilo_datos[NUM_HILOS]; // Contiene los datos para cada hilo
    pthread_t id_hilo[NUM_HILOS]; 

    /* Prepara los datos para cada hilo */
    for (int i = 0; i < NUM_HILOS; i++) {
        hilo_datos[i].dato = i;
        hilo_datos[i].p = i+1;
    }

    /* Se lanzan los hilos */
    for (int i = 0; i < NUM_HILOS; i++) {
        if (pthread_create(&id_hilo[i], NULL, (void *) proceso, (void *)(&hilo_datos[i])) != 0) {
            perror("No se pudo crear el hilo");
            exit(EXIT_FAILURE);
        }
    }

    /* Esperar a que termine */
    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(id_hilo[i], (void**)&valor_devuelto);
    }

    /* Destruir el mutex */
    pthread_mutex_destroy(&mutex);

    return 0;
}