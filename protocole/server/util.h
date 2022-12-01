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
#define MAX 8192

int isRoot();

void getIpAddress(char * ip);

void displayClientConnection(struct sockaddr_in client_addr);
void getHostnameFromServer();
void displayLocalAdress(struct sockaddr_in addre_in);

int isCharValidNumber(char *buff);
int isArrayContainCharacter(char buffCheck, char *content);
int getNumberOfAlpha(char *buff);

void convertCharArrayToAnother(char * buffToModify,char* buffToGet, int start, int length);

int extractLengthFromHeader(char *buff);

void extractFileNameFromHeader(char *buff, char *fileName);

int checkIfCharIsFileName(char *buff);

void removeSpecialCharacterHeader(char *buff, int length);

void getFileNameWithPointer(const int fp, char * filename);

void verifyMessagePeekRecv(int sockCli, char * buff, int lengthRecv);

void removeFile(const int fp);

void removeRecvWhenError(int sockCli, int valRecv);

