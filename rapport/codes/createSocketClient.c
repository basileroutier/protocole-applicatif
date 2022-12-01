#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <assert.h>
#include <netinet/in.h>

#include <net/if.h>
#include <ctype.h>
#include <unistd.h>

#include <arpa/inet.h>
#define PORT 8080
#define SA struct sockaddr

// Function to create socket and connect to server.
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