#include "factoryClient.h"

char typeMsg[MAX_SIZE_ARRAY] = {'m', 'f', '\0'};
char contentMsg[MAX_SIZE_ARRAY] = {'s', 'c', 'e', '\0'};


/**
 * It checks if the client has disconnected
 * 
 * @param buff the buffer to store the received data
 * @param sockCli the socket of the client
 * @param number the number of bytes to be received
 */
void securityRecvClosedClient(char * buff, int sockCli,int number){
    if((recv(sockCli, buff, number, MSG_PEEK))==0){
            perror("Client disconnected");
            close(sockCli);
            exit(0);
    }
    buff[MAX_BYTE_RECV]='\0';
}

/**
 * 
 * 
 * @param sockCli the socket of the client
 * @param buff the buffer that contains the message
 * @param byteRecv the number of bytes received from the client
 * 
 * @return the number of alphabets in the message, the type of message and the content of the message.
 */
int checkMessagePeek(int sockCli, char * buff, int byteRecv){
    securityRecvClosedClient(buff,sockCli, byteRecv);
    char buffAlpha[MAX_SIZE_ARRAY];
    convertCharArrayToAnother(buffAlpha, buff, 0,MAX_BYTE_RECV_ALPHA);
    int numberOfAlpha = getNumberOfAlpha(buffAlpha);
    int index = byteRecv - 2;
    int containType = isArrayContainCharacter(buff[index], typeMsg);
    int containMsgCont = isArrayContainCharacter(buff[(index+1)], contentMsg);

    return numberOfAlpha+containType+containMsgCont;
}

/**
 * It checks if the message received is less than the maximum number of bytes to receive, if it is, it
 * checks if the message received is less than the maximum number of bytes to receive plus 2
 * 
 * @param sockCli the socket of the client
 * 
 * @return The total length of the message received.
 */
int checkRecvMessage(int sockCli){
    char buff[MAX];
    int totalLengthRecv = checkMessagePeek(sockCli, buff, MAX_BYTE_RECV);
    if(totalLengthRecv<MAX_BYTE_RECV){
        memset(buff, 0, MAX);
        totalLengthRecv = checkMessagePeek(sockCli, buff, (MAX_BYTE_RECV+2));
    }
    return totalLengthRecv;
}

/**
 * It receives a message from the client and calls the factoryMessageRecv function to process the
 * message
 * 
 * @param sockCli the socket of the client
 */
void receiveMessageClient(int sockCli){
    while(1){
        char buff[MAX];
        memset(buff, 0, sizeof(buff));
        int lengthBuff = checkRecvMessage(sockCli);
        if(lengthBuff==MAX_BYTE_RECV){
            lengthBuff = recv(sockCli, buff, MAX_BYTE_RECV, 0);
            if(lengthBuff!=MAX_BYTE_RECV){
                perror("Error length doens't correspond to max byte to receive\n");
                exit(1);
            }
            factoryMessageRecv(sockCli, buff);
        }else{
            memset(buff, 0, sizeof(buff));
            recv(sockCli, buff, sizeof(buff), 0);
        }
        
    }
}


/**
 * It gets a message from the factory
 * 
 * @param sockCli the socket of the client
 * @param buff the buffer that contains the message
 * @param buffTypeMsg the type of message (e.g. "A" for alpha, "N" for numeric, "S" for special)
 * @param buffContentMsg the content of the message
 * @param lengthRecv the length of the message received
 * 
 * @return The length of the message
 */
int getMessageFromFactory(int sockCli, char * buff, char * buffTypeMsg, char * buffContentMsg,int lengthRecv){
    removeSpecialCharacterHeader(buff, lengthRecv);
    char buffLength[MAX_BYTE_RECV_ALPHA];
    convertCharArrayToAnother(buffLength, buff, 0,MAX_BYTE_RECV_ALPHA);
    lengthRecv = extractLengthFromHeader(buffLength);
    if(lengthRecv>MAX){
        removeRecvWhenError(sockCli, lengthRecv);
        printf("Too much caracter for message length accept\n");
        exit(1);
    }

    int index = MAX_BYTE_RECV_ALPHA;
    convertCharArrayToAnother(buffTypeMsg, buff, index, (index+1));
    index++;
    convertCharArrayToAnother(buffContentMsg, buff, index, (index+1));
    return lengthRecv;
}

/**
 * It receives a message from the server and checks if it's a message or a file. If it's a message, it
 * calls the function checkMessageRecv. If it's a file, it calls the function factoryFileRecv
 * 
 * @param sockCli the socket of the client
 * @param buff the buffer that contains the message received
 */
void factoryMessageRecv(int sockCli, char * buff){
    char buffTypeMsg[2];
    char buffContentMsg[2];
    int lengthRecv = getMessageFromFactory(sockCli, buff, buffTypeMsg, buffContentMsg, strlen(buff));
    switch(buffTypeMsg[0]){
            case 'm':
                checkMessageRecv(sockCli, buff, lengthRecv, buffContentMsg);
                break;
            case 'f':
                switch(buffContentMsg[0]){
                    case 's':
                        memset(buff, 0, MAX);
                        int length = recv(sockCli, buff, lengthRecv, 0);
                        if(!checkIfCharIsFileName(buff)){
                            perror("Error file name");
                            exit(1);
                        }
                        removeSpecialCharacterHeader(buff, length);
                        char mode[]= "0664";
                        int fp = open(buff, O_WRONLY | O_CREAT | O_TRUNC,  strtol(mode, 0, 8));
                        if(fp<0){
                            perror("Error open file");
                            exit(1);
                        }
                        char nameFile[MAX];
                        strcpy(nameFile, buff);
                        factoryFileRecv(sockCli, buff,fp);
                        printf("Fichier créée > %s\n", nameFile);
                        close(fp);
                        memset(buff, 0, MAX);
                        break;
                    default:
                        removeRecvWhenError(sockCli, lengthRecv);
                        perror("File content not supported");
                        exit(1);
                }
                break;
            default:
                removeRecvWhenError(sockCli, lengthRecv);
                perror("Type of message not support");
                exit(1);
    }
}

/**
 * It checks the message received by the client and displays it on the terminal
 * 
 * @param sockCli the socket of the client
 * @param buff the buffer where the message will be stored
 * @param lengthRecv the length of the message to be received
 * @param contentMsg the message to be sent
 */
void checkMessageRecv(int sockCli, char * buff, int lengthRecv, char * contentMsg){
    memset(buff, 0, MAX);
    switch(contentMsg[0]){
        case 's':
            verifyMessagePeekRecv(sockCli, buff, lengthRecv);
            int length = recv(sockCli, buff, lengthRecv, 0);
            removeSpecialCharacterHeader(buff, length);
            if(strcmp(buff, "?exit?")==0){
                printf("\033[4;35m----Client disconnected----\033[0m\n");
                close(sockCli);
                exit(0);
            }
            printf("Message reçu > %s\n", buff);
            memset(buff, 0, MAX);
            break;
        default :
            removeRecvWhenError(sockCli, lengthRecv);
            perror("Message not support more than MAX size");
            exit(1);
    }
}

/**
 * It receives the file from the server and writes it to the file descriptor
 * 
 * @param sockCli the socket descriptor
 * @param buff the buffer that will be used to store the data received from the server
 * @param fp file descriptor of the file to be written to
 * 
 * @return The function return the number of byte to receive
 */
void factoryFileRecv(int sockCli, char * buff, const int fp){
    memset(buff, 0, MAX);
    int lengthBuff = checkRecvMessage(sockCli);
    if(lengthBuff==MAX_BYTE_RECV){
        lengthBuff = recv(sockCli, buff, MAX_BYTE_RECV, 0);
        if(lengthBuff!=MAX_BYTE_RECV){
            printf("Error length doens't correspond to max byte to receive\n");
            exit(1);
        }
        char buffTypeMsg[2];
        char buffContentMsg[2];
        int lengthRecv = getMessageFromFactory(sockCli, buff, buffTypeMsg, buffContentMsg, lengthBuff);
        switch(buffTypeMsg[0]){
                case 'f':
                    switch(buffContentMsg[0]){
                        case 'c':
                            verifyMessagePeekRecv(sockCli, buff, lengthRecv);
                            lengthBuff = recv(sockCli, buff, lengthRecv, 0);
                            removeSpecialCharacterHeader(buff, lengthBuff);
                            if(write(fp, buff, strlen(buff))<0){
                                perror("Error write file");
                                exit(1);
                            }
                            memset(buff, 0, MAX);
                            factoryFileRecv(sockCli, buff, fp);
                            break;
                        case 'e':
                            recv(sockCli, buff, lengthRecv, 0);
                            memset(buff, 0, MAX);
                            return;
                            break;
                        default:
                            removeRecvWhenError(sockCli, lengthRecv);
                            removeFile(fp);
                            perror("File content not supported");
                            exit(1);
                            break;
                    }
                    break;
                default:
                    removeRecvWhenError(sockCli, lengthRecv);
                    removeFile(fp);
                    perror("Type of message not support");
                    exit(1);
        }
    }else{
        removeRecvWhenError(sockCli, lengthBuff);
        removeFile(fp);
        perror("Error when receive file");
        exit(1);
    }
}