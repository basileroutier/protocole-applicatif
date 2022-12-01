#include "createSocket.h"
#include "util.h"
#include "factoryClient.h"

/**
 * The above function is a server that receives a message from a client and displays it.
 * 
 * @return The return value of the function is the file descriptor for the new socket.
 */
int main(){
    if(!isRoot()){
        perror("Application must run in admin");
        exit(1);
    }
    struct sockaddr_in client_addr;
    socklen_t len_clien_addr = sizeof (client_addr);;
    int socketfd = tcpSocket();
    int conCli;
    while(1){
        conCli = accept(socketfd, (SA*)&client_addr, &len_clien_addr);
        if(conCli<0){
            perror("Server acception client failed");
            exit(1);
        }
        if(fork()==0){
            close(socketfd);
            displayClientConnection(client_addr);
            receiveMessageClient(conCli);
            exit(0);
        }else{
            signal(SIGCHLD, SIG_IGN);
        }
        close(conCli);

    }
    close(conCli);
    return 0;
}
