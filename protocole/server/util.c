#include "util.h"

/**
 * If the user is root, return 1, otherwise return 0.
 * 
 * @return The user id of the current user.
 */
int isRoot(){
    return getuid()==0;
}

/**
 * It gets the IP address of the machine and stores it in the variable ip
 * 
 * @param ip the ip address of the server
 */
void getIpAddress(char * ip){
    system("hostname -I > ip.txt");
    FILE *fp = fopen("ip.txt", "r");
    if(fp==NULL){
        perror("Can not open file ip.txt");
        exit(1);
    }
    fscanf(fp, "%s", ip);
}


/**
 * It gets the local address of the server and displays it
 * 
 * @param addre_in the address of the server
 */
void displayLocalAdress(struct sockaddr_in addre_in){
    char node[NI_MAXHOST];
    char serv[NI_MAXSERV];
    
    inet_pton(AF_INET, "127.0.0.1", &addre_in.sin_addr);
    int res = getnameinfo((struct sockaddr*)&addre_in, sizeof(addre_in),
                          node, sizeof(node),
                          serv, sizeof(serv), NI_NUMERICHOST | NI_NUMERICSERV);
    
    if (res) {
        printf("error: %d\n", res);
        printf("%s\n", gai_strerror(res));
    }
    else{
        getHostnameFromServer();
        printf("\033[0;34m--------------Port--------------\033[0m\n");
        printf("%s\n\n\n", serv);
    }
}


/**
 * This function prints the hostname and ip address of the server
 */
void getHostnameFromServer(){
    printf("\n\n");
    printf("\033[0;34m--------------Hostname--------------\033[0m\n");
    system("hostname");
    printf("\033[0;34m--------------Ip Adress---------------------\033[0m\n");
    system("hostname -I");
}

/**
 * This function displays the IP address and port number of the client that has connected to the server
 * 
 * @param client_addr This is the address of the client that is connecting to the server.
 */
void displayClientConnection(struct sockaddr_in client_addr){
    printf("IP address is: %s |", inet_ntoa(client_addr.sin_addr));
    printf("\tport is: %d\n", (int) ntohs(client_addr.sin_port));
}



/**
 * It checks if the string is a valid number
 * 
 * @param buff The string to be checked.
 * 
 * @return the number of characters in the string.
 */
int isCharValidNumber(char *buff){
    int i= 0;
    while(buff[i]!='\0'){
        if(isalpha(buff[i])){
            return 0;
        }
        i++;
    }
    return 1;
}

/**
 * It checks if the character is in the array.
 * 
 * @param buffCheck The character to check if it's in the array
 * @param content The string to be checked
 * 
 * @return the number of characters in the string.
 */

int isArrayContainCharacter(char buffCheck, char *content){
    int i=0;
    while(content[i]!='\0'){
        if(buffCheck==content[i]){
            return 1;
        }
        i++;
    }
    return 0;
}

/**
 * The above function checks if the string is a number or not.
 * 
 * @param buff The buffer that contains the string to be checked.
 * 
 * @return The number of digits in the string.
 */
int getNumberOfAlpha(char *buff){
    int i= 0;
    int number=0;
    while(buff[i]!='\0'){
        if(isdigit(buff[i])){
            number++;
        }else{
            return 0;
        }
        i++;
    }
    return number;
}

/**
 * It takes a char array and copies the contents of another char array into it
 * 
 * @param buffToModify The buffer that will be modified.
 * @param buffToGet The buffer that contains the data that you want to copy.
 * @param start the index of the first character to be copied
 * @param length the length of the buffer to be modified
 */
void convertCharArrayToAnother(char * buffToModify, char* buffToGet, int start, int length){
    int i=0;
    int j=start;
    while(j<length){
        buffToModify[i]=buffToGet[j];
        i++,j++;
    }
    buffToModify[i]='\0';
}

/**
 * It takes a string and returns the length of the string
 * 
 * @param buff The buffer to extract the length from.
 * 
 * @return The length of the message
 */
int extractLengthFromHeader(char *buff){
    char * endString;
    if(!isCharValidNumber(buff)){
        perror("Header not char number");
        exit(1);
    }
    long value = strtol(buff, &endString, 10);
    return value;
}

/**
 * It takes a buffer and a filename, and copies the filename from the buffer into the filename
 * 
 * @param buff The buffer that contains the header
 * @param fileName The name of the file to be sent.
 */
void extractFileNameFromHeader(char *buff, char *fileName){
    int i=0;
    while(buff[i]!='\0'){
        fileName[i]=buff[i];
        i++;
    }
    fileName[i]='\0';
}

/**
 * It checks if the string passed to it has a '.' in it. If it does, it returns 1, else it returns 0
 * 
 * @param buff The buffer that contains the string to be checked.
 * 
 * @return the number of files in the directory.
 */
int checkIfCharIsFileName(char *buff){
    int i=0;
    while(buff[i]!='\0'){
        if(buff[i]=='.'){
            return 1;
        }
        i++;
    }
    return 0;
}

/**
 * It removes the special characters from the header
 * 
 * @param buff the buffer that contains the header
 * @param lenght the length of the buffer
 */
void removeSpecialCharacterHeader(char *buff, int lenght){
    if(buff[lenght-2]=='\r' && buff[lenght-1]=='\n'){
        buff[lenght-2]='\0';
    }
}

/**
 * It takes a file descriptor and returns the filename associated with it
 * 
 * @param fp The file descriptor of the file you want to get the name of.
 * @param filename The name of the file to open.
 */
void getFileNameWithPointer(const int fp, char * filename){
    char fd_path[0xFFF];
    ssize_t n;

    sprintf(fd_path, "/proc/self/fd/%d", fp);
    n = readlink(fd_path, filename, 255);
    if (n < 0){
        perror("readlink for get filename");
        exit(1);
    }

    filename[n] = '\0';
}

/**
 * It verifies that the message received from the client is the same length as the message that was
 * supposed to be received
 * 
 * @param sockCli the socket of the client
 * @param buff the buffer to store the received data
 * @param lengthRecv the length of the message to receive
 */
void verifyMessagePeekRecv(int sockCli, char * buff, int lengthRecv){
    memset(buff, 0, MAX);
    int lengthPeekRecv;
    if((lengthPeekRecv = recv(sockCli, buff, lengthRecv, MSG_PEEK))==0){
        perror("Client disconnected");
        close(sockCli);
        exit(0);
    }
    removeSpecialCharacterHeader(buff, lengthPeekRecv);
    if(strlen(buff)!=lengthRecv){
        removeRecvWhenError(sockCli, lengthRecv);
        printf("Error length buff doesn't correspond to length to receive from client\n");
        exit(1);
    }
    memset(buff, 0, MAX);
}

/**
 * It removes a file
 * 
 * @param file the file pointer
 */
void removeFile(const int file){
    char filename[MAX];
    getFileNameWithPointer(file, filename);
    if(remove(filename)!=0){
        printf("Error when remove file : %s", filename);
        exit(1);
    }
}

/**
 * It removes the data from the socket buffer when the data is not what we want
 * 
 * @param sockCli the socket of the client
 * @param valRecv the number of bytes to be received
 */
void removeRecvWhenError(int sockCli, int valRecv){
    char buff[MAX];
    memset(buff, 0, MAX);
    int lengthPeekRecv;
    if((lengthPeekRecv = recv(sockCli, buff, valRecv, MSG_PEEK))==0){
        perror("Client disconnected");
        close(sockCli);
        exit(0);
    }
    recv(sockCli, buff, valRecv, 0);
    memset(buff, 0, MAX);
}

