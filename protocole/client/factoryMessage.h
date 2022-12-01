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
#include <fcntl.h>

#include <net/if.h>
#include <ctype.h>
#include <unistd.h>

#include <arpa/inet.h>

#define MAX 8192
#define HEAD_LENGTH 6
#define HEAD_ALPHA 4

extern char contentMsg[HEAD_LENGTH];
extern char *command[HEAD_LENGTH][2];
extern char currentCommand;
extern char currentContent;

void convertIntToCharWithLeadingZero(char* buff, int value);

void putHeaderToMessage(char* messageToSend, int value, char contentMessage);

void convertHeaderToMessage(char * messageToSend, char * messageInput);
void readKeyboard(int socketfd);

void factoryMessage(int socketfd, char * messageInput, int *quit);
int changeCurrentCommand(char * messageInput);
int checkUtilCommand(char * messageInput, int *quit);

void sendReadFile(int socketfd, char * fileName);

void sendHeaderContent(int socketfd, char * messageInput);

void displayHelpCommand();