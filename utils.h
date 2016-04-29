#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <netdb.h>
#include <time.h> 
#include <errno.h>
#include <stdlib.h>

#define MAXNAMELEN 256
#define ERRNO -1
#define MAX_HEADER_LINE_SIZE 8192
#define MAX_HEADER_LINES 100

int startserver();
int hooktoserver(char *servhost, ushort servport); 
char *recvtext(int sd);
int req_done_processing(char *str1);
char *process_req_header(int sd);
char* process_req_line(int sd);
int sendtext(int sd, char *msg);
char* handle_init_request(int *sd);
char* store_init_request_header(int *sd);


#endif /*UTILS_H */
