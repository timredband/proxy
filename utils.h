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
#define SIZEOF_CONTENTLENGTH 15
#define MAX_STR_LEN 100


int startserver();
int hooktoserver(char *servhost, ushort servport); 
char *recvtext(int sd);
int req_done_processing(char *str1);
char *process_req_header(int sd, char *host);
char* process_req_line(int sd);
int sendtext(int sd, char *msg);
char* handle_init_request(int *sd);
char* store_init_request_header(int *sd);
char* process_first_line( int sd, char* host);
char* process_http_response_header(int sd, int *content_length);//returns header including status line and writes content length to content length arg
int check_if_content_length_line(char* line);
int get_content_length(char* line);//returns content length in http response
char* process_http_response_body(int sd, int len);
int readn(int sd, char *buf, int n);
char *process_http_response_body(int sd, int len);

#endif /*UTILS_H */
