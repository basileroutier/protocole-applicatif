#include "factoryMessage.h"
#include "util.h"


char contentMsg[HEAD_LENGTH] = {'s', 'c', 'e', '\0'};
char *command[HEAD_LENGTH][2] = {{"?message?", "envoie un message au serveur"}, {"?file?", "envoie un fichier au serveur"}, {"?exit?", "quitte le programme"},{"?help?", "La commande ?help? vous permet de connaitre les commandes pour changer de mode d'envoi."}, {"\0", "\0"}};
char currentCommand = 'm';
char currentContent = 's';

/**
 * It takes an integer and converts it to a string with leading zeros
 * 
 * @param buff the buffer to write the converted value to
 * @param value the integer value to be converted to a string
 */
void convertIntToCharWithLeadingZero(char* buff, int value){
    for(int i=0;i<HEAD_ALPHA;i++){
        buff[i] = '0';
    }
    sprintf(buff, "%04d", value);
}

/**
 * It converts an integer to a char array with leading zeros, then adds the current command and the
 * content message to the char array
 * 
 * @param message the message to be sent
 * @param value the value of the header
 * @param contentMessage the message that will be sent to the server
 */
void putHeaderToMessage(char* message, int value, char contentMessage){
    convertIntToCharWithLeadingZero(message, value);
    int index = HEAD_ALPHA;
    message[index] = currentCommand;
    index++; // ++index dans le tableau ne foncitonne pas bizarement :'(
    message[index] = contentMessage;
}

/**
 * It takes a message and a header and combines them into a single message
 * 
 * @param messageToSend the message that will be sent to the server
 * @param messageInput The message that you want to send.
 */
void convertHeaderToMessage(char * messageToSend, char * messageInput){
    int i = HEAD_LENGTH;
    int j=0;
    while(messageInput[j]!='\0' && i<MAX){
        messageToSend[i]=messageInput[j];
        i++, j++;
    }
}

/**
 * It reads the keyboard and sends the message to the server
 * 
 * @param socketfd the socket file descriptor
 */
void readKeyboard(int socketfd){    
    int quit=0;
    printf("\nPar défaut, vous envoyer des messages\n");
    printf("\tTester la commande ?file? pour permettre d'envoyer des fichiers\n\n");
    while(!quit){
        char buffSend[MAX];
        memset(buffSend, 0, sizeof(buffSend));
        secureRecvFromServer(socketfd);
        if(currentCommand=='m'){
            printf("Entrez votre message (ou changer de mode d'envoi):\n");
        }else{
            printf("Entrez le nom du fichier (ou changer de mode d'envoi):\n");
        }
        fgets(buffSend, MAX, stdin);
        buffSend[strlen(buffSend)-1] = '\0';
        if(strlen(buffSend)>(MAX-HEAD_LENGTH-1)){
            printf("Message length to bigger. Please enter 8192 max character");
            continue;
        }
        if(strlen(buffSend)<1){
            printf("\nMessage couldn't be empty\n");
            clearerr(stdin);
            continue;
        }
        factoryMessage(socketfd,buffSend, &quit);
    }
    currentCommand = 'm';
    currentContent = 's';
    sendHeaderContent(socketfd, "?exit?");
    printf("Vous vous êtes déconnecté du serveur\n");
}

/**
 * This function is used to send message to server
 * 
 * @param socketfd the socket file descriptor
 * @param messageInput the message that the user input
 * @param quit a pointer to a boolean variable that indicates whether the client should quit or not.
 */
void factoryMessage(int socketfd, char * messageInput, int *quit){
    if(checkUtilCommand(messageInput, quit)){
        return;
    }
    if(changeCurrentCommand(messageInput)){
        return;
    }
    
    char message[MAX];
    putHeaderToMessage(message, strlen(messageInput), currentContent);
    switch (currentCommand)
    {
    case 'm':
        convertHeaderToMessage(message, messageInput);
        int byteSend = send(socketfd, message, strlen(message),0);
        if(byteSend<0){
            perror("Error sending to server");
            close(socketfd);
            exit(1);
        }
        printf("\t* Message sent to server : %s\n", message);
        memset(message, 0, sizeof(message));
        break;
    case 'f':
        sendReadFile(socketfd, messageInput);
        memset(messageInput, 0, strlen(messageInput));
        break;
    default:
        break;
    }
}

/**
 * It opens the file, sends the header, sends the content, sends the end of the file, and closes the
 * file
 * 
 * @param socketfd the socket file descriptor
 * @param fileName the name of the file to be sent
 * 
 * @return The content of the file
 */
void sendReadFile(int socketfd, char * fileName){
    int fp = open(fileName, O_RDONLY);
    if(fp<0){
        perror("Le fichier n'existe pas");
        return;
    }
    sendHeaderContent(socketfd, fileName);
    currentContent = contentMsg[1];
    char message[MAX];
    memset(message, 0, sizeof(message));
    while(read(fp, message, MAX)>0){
        sendHeaderContent(socketfd, message);
        memset(message, 0, sizeof(message));
    }
    currentContent = contentMsg[2];
    sendHeaderContent(socketfd, "end");
    currentContent = contentMsg[0];
    printf("File send\n");
    close(fp);
}

/**
 * It changes the current command and the current content
 * 
 * @param messageInput the message that the user has entered
 * 
 * @return 1 if the command is valid, 0 otherwise.
 */
int changeCurrentCommand(char * messageInput){
    if(strcmp(messageInput, command[0][0])==0){
        currentCommand = 'm';
        currentContent = 's';
        printf("\n\033[44mChangement vers message\033[0m\n");
        return 1;
    }else if(strcmp(messageInput, command[1][0])==0){
        currentCommand = 'f';
        currentContent = 's';
        printf("\n\033[44mChangement vers fichier (n'oubliez pas le path pour l'envoye de fichier)\033[0m\n");
        return 1;
    }
    return 0;
}

/**
 * It checks if the user input is a utility command, and if it is, it executes the command
 * 
 * @param messageInput The message that the user inputted
 * @param quit a pointer to the quit variable in the main function.
 * 
 * @return the value of the quit variable.
 */
int checkUtilCommand(char * messageInput, int * quit){
    if(strcmp(messageInput, command[3][0])==0){
        printf("\n");
        displayHelpCommand();
        return 1;
    }else if(strcmp(messageInput, command[2][0])==0){
        return (*quit = 1);
    }
    return 0;
}

/**
 * It displays the help command
 */
void displayHelpCommand(){
    printf("\n\033[44m%s\033[0m\n", command[3][1]);
    for(int i=0;i<3;i++){
        printf("\t* %s: %s.\n", command[i][0], command[i][1]);
    }
}


/**
 * It takes in a socket file descriptor and a message, and sends the message to the server
 * 
 * @param socketfd the socket file descriptor
 * @param messageInput the message that will be sent to the server
 */
void sendHeaderContent(int socketfd, char * messageInput){
    char message[MAX];
    memset(message, 0, sizeof(message));
    putHeaderToMessage(message, strlen(messageInput), currentContent);
    convertHeaderToMessage(message, messageInput);
    int byteSend = send(socketfd, message, strlen(message),0);
    if(byteSend<0){
        perror("Error sending to server");
        close(socketfd);
        exit(1);
    }
}
