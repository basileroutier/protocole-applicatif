#include "createSocket.h"
/**
 * Create tcp socket and bind it in memory
 * Return the location of socket
*/
int tcpSocket(){
    struct sockaddr_in server_addr;

    int socketfd = socket(AF_INET, SOCK_STREAM, 0);

    if(socketfd==-1){
        perror("socket");
        exit(1);
    }
    printf("Socket created\n");

    char ip[256];
    memset(ip, 0, sizeof(ip));
    getIpAddress(ip);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(ip);
    
    displayLocalAdress(server_addr);

    if(bind(socketfd, (SA*) &server_addr, sizeof (server_addr)) !=0){
        perror("Error binding name to socket");
        exit(1);
    }
    printf("Bind name socket\n");

    if(listen(socketfd, backlognum)!=0){
        perror("Error listening for connections");
        exit(1);
    }
    printf("Listening for connection\n");
    
    return socketfd;
}