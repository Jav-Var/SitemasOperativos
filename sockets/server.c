#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define PORT 3535
#define BACKLOG 5

int main(void) {
    int fd, fd2;
    struct sockaddr_in server, client;
    socklen_t client_len;

    /* Crear socket TCP (stream). El original usaba SOCK_DGRAM (UDP) pero luego
       llamaba a listen/accept que son para TCP. */
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* Permitir reutilizar el puerto rápidamente durante pruebas */
    int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("setsockopt");
        /* no fatal: solo informamos */
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("bind");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (listen(fd, BACKLOG) == -1) {
        perror("listen");
        close(fd);
        exit(EXIT_FAILURE);
    }

    client_len = sizeof(client);
    fd2 = accept(fd, (struct sockaddr *)&client, &client_len);
    if (fd2 == -1) {
        perror("accept");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Conexi\xc3\xb3n aceptada de %s:%d\n",
           inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    while(1){
    const char *msg = "hola cliente\n";
    ssize_t sent = send(fd2, msg, strlen(msg), 0);
    if (sent == -1) {
        perror("send");
    }

    char buffer[256];
    ssize_t rec = recv(fd2, buffer, sizeof(buffer) - 1, 0);
    if (rec == -1) {
        perror("recv");
    } else {
        buffer[rec] = '\0';
        printf("Recibido: %s\n", buffer);
    }
    }

    close(fd2);
    close(fd);
    return 0;
}
