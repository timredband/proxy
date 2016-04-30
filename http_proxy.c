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
#include <pthread.h>
#include "utils.h"

#define MAXNAMELEN 256
#define DEBUG 0
#define PORT 80

void* create_thread(void *arg){
    int *sd = (int*)arg;
    int sock = *sd;
    char *host = malloc(1024);
    char *request_header = process_req_header(sock, host);
    printf("%s\n", request_header);
}

int main(int argc, char* argv[]){
	int servsock; /* server socket descriptor */

	if (argc != 1) {
		fprintf(stderr, "usage : %s\n", argv[0]);
		exit(1);
	}

	servsock = startserver();
	if (servsock == -1) {
		perror("Error on starting server: ");
		exit(1);
	}


    while(1){
        int *new_sd = (int*)malloc(sizeof(int));
        struct sockaddr_in addr;
        int len = sizeof(struct sockaddr_in);
        *new_sd = accept(servsock, (struct sockaddr*)&addr, &len); 
        char *host = malloc(1024);
        char *request_header = process_req_header(*new_sd, host);
        int send_sock = hooktoserver(host, PORT);//connect to the server from proxy
        int write_to_server;
        if(!(write_to_server = sendtext(send_sock, request_header))){
            perror("Error on sending from proxy: ");
            exit(1);
        }
        int *content_length = malloc(sizeof(int));
        char *response = process_http_response_header(send_sock, content_length);
        char *body = process_http_response_body(send_sock, *content_length);
        send_http_response_header_to_client(*new_sd, response);
        send_http_response_body_to_client(*new_sd, body);
        printf("%s\n", host);
        /*
        pthread_t thread;
        pthread_create(&thread, NULL, create_thread, new_sd);//TODO:thread needs to free pointer
        */
    }

}
