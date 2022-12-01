
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
#include <sys/stat.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h> 

#include <net/if.h>
#include <ctype.h>
#include <unistd.h>

#include <arpa/inet.h>
#include "createSocket.h"
#include "util.h"
#define MAX 8192
#define MAX_SIZE_ARRAY 6
#define MAX_BYTE_RECV 6
#define MAX_BYTE_RECV_ALPHA 4
#define MAX_BYTE_RECV_LETTER 1


extern char typeMsg[MAX_SIZE_ARRAY];
extern char contentMsg[MAX_SIZE_ARRAY];

void securityRecvClosedClient(char * buff, int sockCli,int number);
int checkMessagePeek(int sockCli, char * buff, int byteRecv);
int checkRecvMessage(int sockCli);

void receiveMessageClient(int sockCli);

void factoryMessageRecv(int sockli, char * buff);
void checkMessageRecv(int sockCli, char * buff,int lengthRecv,  char * contentMsg);
void factoryFileRecv(int sockCli, char * buff, const int fp);

int getMessageFromFactory(int sockCli, char * buff, char * buffTypeMsg, char * buffContentMsg,int lengthRecv);