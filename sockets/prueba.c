#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define PORT 3535

int main() {
    struct sockaddr_in server;
    int fd, fd2;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;
    //bzero(&server.sin_zero, 8);


    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket error");
    }

    int r = bind(fd, (sockaddr*) &server, sizeof(sockaddr));
    if (r < 0) {
        perror("bind error");
    }


    r = listen(fd, BACKLOG);

    size_t size = sizeof(struct sockaddr_in);
    int fd2 = accept(fd, (sockaddr*) &client, &size);
    if (fd2 < 0) {
        perror();
    }5
    r = send(fd2, "hola cliente", 12, 0);
    
    char buffer[32];
    r = recv(fd2, buffer, 20, 0);
    buffer[r] = 0;
    printf("%s", buffer);
    close(fd2);
    close(fd);

    
    

    return 0;
}