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
#define RECV_BUFF_LEN 32

int main() {
    struct sockaddr_in cliente;
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("error al crear socket");
        exit(EXIT_FAILURE);
    }
    memset(&cliente, 0, sizeof(cliente));
    cliente.sin_family = AF_INET;
    cliente.sin_port = htons(PORT);
    cliente.sin_addr.s_addr = inet_addr("127.0.0.1");
    socklen_t socket_size = sizeof(cliente);
    
    if (connect(fd, &cliente, socket_size) == -1) {
        perror("connect error");
        exit(EXIT_FAILURE);
    }

    char buffer[RECV_BUFF_LEN];
    ssize_t size_rec = recv(fd, buffer, RECV_BUFF_LEN, 0);
    buffer[size_rec] = '\0';
    printf("%s\n", buffer);
    
    ssize_t size_send = send(fd, "Hola server", 13, 0);
    
    printf("Sent %zd bytes\n", size_send);
    
    close(fd);
    

}