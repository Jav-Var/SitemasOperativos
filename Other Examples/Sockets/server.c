// server.c - servidor TCP simple
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>             // close
#include <arpa/inet.h>          // inet_pton, htons
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 3490
#define BACKLOG 5
#define BUF_SIZE 1024

int main(void) {
    int listen_fd = -1, client_fd = -1;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buf[BUF_SIZE];
    ssize_t n;

    // 1) Crear socket
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Permitir reusar puerto rápidamente (opcional, útil en desarrollo)
    int opt = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    // 2) Preparar la dirección del servidor (IPv4, cualquier interfaz)
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // escuchar en todas las interfaces
    server_addr.sin_port = htons(PORT);       // puerto en orden de red

    // 3) Bind -> asignar dirección al socket
    if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    // 4) Escuchar conexiones entrantes
    if (listen(listen_fd, BACKLOG) < 0) {
        perror("listen");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    printf("Servidor escuchando en el puerto %d...\n", PORT);

    // 5) Aceptar una conexión (bloqueante)
    client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0) {
        perror("accept");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    // Mostrar IP cliente
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    printf("Conexión aceptada desde %s:%d\n", client_ip, ntohs(client_addr.sin_port));

    // 6) Recibir datos
    n = recv(client_fd, buf, sizeof(buf)-1, 0);
    if (n < 0) {
        perror("recv");
    } else {
        buf[n] = '\0'; // asegurar terminador
        printf("Mensaje recibido: %s\n", buf);
    }

    // 7) Enviar respuesta
    const char *response = "Hola desde el servidor";
    if (send(client_fd, response, strlen(response), 0) < 0) {
        perror("send");
    }

    // 8) Cerrar sockets
    close(client_fd);
    close(listen_fd);
    printf("Servidor finalizado.\n");
    return 0;
}
