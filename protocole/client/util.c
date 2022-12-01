#include "util.h"

/**
 * If the IP address is valid, the function returns 1, else it returns 0
 * 
 * @param ipAddress The IP address to validate.
 */
int isValidIpAddress(char *ipAddress)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
    return result != 0;
}

/**
 * It checks if the server has disconnected. If it has, it closes the socket and exits the program
 * 
 * @param sockfd The socket file descriptor
 */
void secureRecvFromServer(int sockfd){
    char buff[MAX];
    if((recv(sockfd, buff, 0, MSG_PEEK | MSG_DONTWAIT))==0){
        perror("Server disconnected");
        close(sockfd);
        exit(1);
    }
}


/**
 * It returns true if the file exists, false otherwise
 * 
 * @param fileName The name of the file to check.
 * 
 * @return The file descriptor of the file.
 */
int fileExist(char * fileName){
   int fd = open(fileName, O_RDONLY);
   return fd!=-1;
}