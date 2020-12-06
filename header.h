#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BUFSIZE 512


int sock;
char buf[BUFSIZE];
char response[50000] = "";

struct hostent *remoteHost; //웹주소를 ip로 바꾸기 위해
struct sockaddr_in client_addr;

// Method ... in method.h 
void getQueryResult();
char *substring(char *input, int i_begin, int i_end);

void setSocket(char *address);
void connectServer();
void receiveResponse();

void getIsInjection(char *path, char *cookie);
void getColumnNumber(char *path, char *cookie);
void getTableList(char *path, char *cookie);
void getTableColumn(char *path, char *cookie, char *table);
