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

int startserver() {
	int sd; /* socket descriptor */

	char servhost[MAXNAMELEN]; /* full name of this host */
    servhost[MAXNAMELEN - 1] = '\0';
	ushort servport; /* port assigned to this server */

	/*
	 FILL HERE
	 create a TCP socket using socket()
	 */

    sd = socket(AF_INET, SOCK_STREAM, 0);


	/*
	 FILL HERE
	 bind the socket to some port using bind()
	 let the system choose a port
	 */
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(0);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sd, (struct sockaddr*) &sa, sizeof(sa)) == ERRNO){
        exit(0);
    }


	/* we are ready to receive connections */
	listen(sd, 5);

	/*
	 FILL HERE
	 figure out the full host name (servhost)
	 use gethostname() and gethostbyname()
	 full host name is remote**.cs.binghamton.edu
	 */

    if(gethostname(servhost, MAXNAMELEN) == ERRNO){
        exit(0);
    }
    struct hostent* h;
    h = gethostbyname(servhost);

	/*
	 FILL HERE
	 figure out the port assigned to this server (servport)
	 use getsockname()
	 */
    int len = sizeof(sa);
    if(getsockname(sd,(struct sockaddr*) &sa, &len) == ERRNO){
        exit(0);
    }
    servport = ntohs(sa.sin_port);

	/* ready to accept requests */
	printf("admin: started server on '%s' at '%hu'\n", h -> h_name, servport);
	return (sd);
}
/*----------------------------------------------------------------*/

/*----------------------------------------------------------------*/
int hooktoserver(char *servhost, ushort servport) {
	int sd; /* socket descriptor */

	ushort clientport; /* port assigned to this client */

	/*
	 FILL HERE
	 create a TCP socket using socket()
	 */
    sd = socket(AF_INET, SOCK_STREAM, 0);

	/*
	 FILL HERE
	 connect to the server on 'servhost' at 'servport'
	 use gethostbyname() and connect()
	 */
    struct sockaddr_in serv_addr;
    struct hostent *h = gethostbyname(servhost);
    memcpy(&serv_addr.sin_addr.s_addr, h -> h_addr, h -> h_length);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(servport);
    if(connect(sd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        exit(0);
    }
	/*
	 FILL HERE
	 figure out the port assigned to this client
	 use getsockname()
	 */
    struct sockaddr_in addr;
    int len = sizeof(struct sockaddr);
    if(getsockname(sd, (struct sockaddr *) &addr, &len) == ERRNO){
        exit(0);
    }
    clientport = ntohs(addr.sin_port);


	/* succesful. return socket descriptor */
	printf("admin: connected to server on '%s' at '%hu' thru '%hu'\n", servhost,
			servport, clientport);
	printf(">");
	fflush(stdout);
	return (sd);
}
/*----------------------------------------------------------------*/

/*----------------------------------------------------------------*/

char *recvtext(int sd) {
	char *msg;
    /*
	long len;

	if (!readn(sd, (char *) &len, sizeof(len))) {
		return (NULL);
	}
	len = ntohl(len);

	msg = NULL;
    */
    msg = (char *) malloc(MAX_HEADER_LINE_SIZE);
    if (!msg) {
        fprintf(stderr, "error : unable to malloc\n");
        return (NULL);
    }

    /* read the message text */
	char * ptr;

	ptr = buf;
	while (toberead > 0) {
		int byteread;

		byteread = read(sd, ptr, toberead);//TODO:Just read one byte at a time here. Set toberead to 1
		if (byteread <= 0) {
			if (byteread == -1)
				perror("read");
			return (0);
		}

		toberead -= byteread;
		ptr += byteread;
	}
	return (1);

	/* done reading */
	return (msg);
}

int sendtext(int sd, char *msg) {
	long len;

	/* write lent */
	len = (msg ? strlen(msg) + 1 : 0);
	len = htonl(len);
	write(sd, (char *) &len, sizeof(len));

	/* write message text */
	len = ntohl(len);
	if (len > 0)
		write(sd, msg, len);
	return (1);
}

char* handle_init_request(int *sd){




}

char* store_init_request_header(int *sd){
    char* header = (char*)malloc(MAX_HEADER_LINE_SIZE);



}
/*----------------------------------------------------------------*/

