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

int isValidIpAddress(char *ipAddress);
void secureRecvFromServer(int sockfd);
int fileExist(char * fileName);