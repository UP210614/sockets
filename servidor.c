#include <stdio.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
int main (){

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        printf("Error al crear el socket\n");
        close(sockfd);
        return 0;
    }
    printf("El socket se creó correctamente\n");

    struct sockaddr_in address = {
        AF_INET,
        htons(9999),
        0
    };

    if(bind(sockfd, &address, sizeof(address)) == -1){
        printf("Error al enlazar el socket\n");
        close(sockfd);
        return 0;
    }

    printf("El socket se enlazo correctamente\n");


    if(listen(sockfd,10) == -1){
        printf("Error al poner en escucha el socket\n");
        close(sockfd);
        return 0;
    }

    printf("El socket esta en escucha\n");
    
    int clientfd = accept(sockfd, 0, 0);
    if(clientfd == -1){
        printf("Error al aceptar el cliente\n");
        close(sockfd);
        return 0;
    }

    printf("Conexion con cliente aceptada\n");


    struct pollfd fds[2] = {
        {
            0,
            POLLIN,
            0,
        },
        {
            clientfd,
            POLLIN,
            0
        }
    };

    poll(fds,2,50000);

    char buffer[256] = { 0 };
    for( ; ; ){
        if(fds[0].revents & POLLIN){
            if(read(0, buffer, 255) == -1){
                printf("Error al leer\n");
                close(sockfd);
                return 1;
            }
            if(send(clientfd, buffer, 255, 0) == -1){
                printf("Error al envíar\n");
                close(sockfd);
                return 1;
            }
        }else if(fds[1].revents & POLLIN){
            if(recv(clientfd, buffer, 255, 0) == 0){
                printf("Error al recibir desde el cliente\n");
                close(sockfd);
                return 1;
            }

            printf("%s\n",buffer);
        }
    }
    
    close(sockfd);
    return 0;
}