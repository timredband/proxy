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

#define MAXNAMELEN 256
#define DEBUG 0
#define PORT 80

extern char * recvtext(int sd);
extern int sendtext(int sd, char *msg);
extern int startserver();

void* create_thread(void *arg){
    int *sd = (int*)arg;

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
        pthread_t thread;
        pthread_create(&thread, NULL, create_thread, new_sd);//TODO:thread needs to free pointer


    }

}
