// client.c - cliente TCP simple
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>             // close
#include <arpa/inet.h>          // inet_pton, htons
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_IP "127.0.0.1"
#define PORT 3490
#define BUF_SIZE 1024

int main(void) {
    int sockfd;
    struct sockaddr_in serv_addr;
    char buf[BUF_SIZE];
    ssize_t n;

    // 1) Crear socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 2) Preparar dirección del servidor
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 3) Conectar al servidor
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Conectado a %s:%d\n", SERVER_IP, PORT);

    // 4) Enviar mensaje
    const char *msg = "Hola desde el cliente";
    if (send(sockfd, msg, strlen(msg), 0) < 0) {
        perror("send");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 5) Recibir respuesta
    n = recv(sockfd, buf, sizeof(buf)-1, 0);
    if (n < 0) {
        perror("recv");
    } else {
        buf[n] = '\0';
        printf("Respuesta recibida: %s\n", buf);
    }

    // 6) Cerrar socket
    close(sockfd);
    return 0;
}
