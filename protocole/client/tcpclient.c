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

#include "util.h"
#include "createSocket.h"
#include "factoryMessage.h"


/**
 * 
 * 
 * @param argc The number of arguments passed to the program.
 * @param argv argv is an array of pointers to the command line arguments.
 */
int main(int argc, char *argv[]){

    if(argc!=2){
        printf("Usage: %s <ip address>\n", argv[0]);
        exit(1);
    }
    if(!isValidIpAddress(argv[1])){
        printf("Invalid ip address");
        exit(1);
    }
    char * ipAdress = argv[1];

    int socketfd = tcpSocketCreateAndConnection(ipAdress);
    displayHelpCommand();
    
    readKeyboard(socketfd);

    close(socketfd);
}
