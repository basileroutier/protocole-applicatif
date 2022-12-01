#include "createSocket.h"



/**
 * It creates a socket, connects to the server, and returns the socket file descriptor
 * 
 * @param ipAdress The IP address of the server you want to connect to.
 * 
 * @return The socket file descriptor.
 */
int tcpSocketCreateAndConnection(char * ipAdress){
    struct sockaddr_in server_addr;

    int socketfd = socket(AF_INET, SOCK_STREAM, 0);

    if(socketfd==-1){
        perror("Error creation socket");
        exit(1);
    }
    printf("Socket created\n");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(ipAdress);

    if (connect(socketfd, (SA*)&server_addr, sizeof(server_addr)) != 0) {
        perror("Error connection server");
        exit(1);
    }
    printf("connected to server..\n");

    return socketfd;
}