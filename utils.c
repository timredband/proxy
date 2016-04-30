#include "utils.h"

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
	fflush(stdout);
	return (sd);
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/

int readn(int sd, char *buf, int n) {
    int toberead, totaltoread;
    char * ptr;

    totaltoread = n;
    ptr = buf;
    toberead = READ_SIZE;
    while (totaltoread > 0) {
        int byteread;

        byteread = read(sd, ptr, toberead);
        if (byteread <= 0) {
            if (byteread == -1)
                perror("read");
            return (0);
        }

        totaltoread -= byteread;
        ptr += byteread;
    }
    return (1);
}

char *process_http_response_body(int sd, int len) {
    char *msg;
    msg = NULL;
    int test_MALLOC = 1000000;
    if (len > 0) {
        msg = (char *) malloc(test_MALLOC);
        if (!msg) {
            fprintf(stderr, "error : unable to malloc\n");
            return (NULL);
        }

        /* read the message text */
        if (!readn(sd, msg, len)) {
            free(msg);
            return (NULL);
        }
    }

    /* done reading */
    return (msg);
}

int req_done_processing(char *str1){
    char end[] = "\r\n";
    int ret = strcmp(str1, end);
    return !ret;
}

char* process_http_response_header(int sd, int *content_length){
    char *header;
    char *line;
    char *buildheader;
    header = process_req_line(sd);
    int done = 0;
    int is_content_length_line = 0;
    int cont_length = 0;
    while(!done){
        line = process_req_line(sd);
        buildheader = malloc(strlen(header) + 1 + strlen(line) + 1);
        memcpy(buildheader, header, strlen(header) + 1);
        free(header);
        strcat(buildheader, line);
        if(is_content_length_line = check_if_content_length_line(line)){
            cont_length = get_content_length(line);
            *content_length = cont_length;
        }
        done = req_done_processing(line);
        free(line);
        header = malloc(strlen(buildheader) + 1);
        memcpy(header, buildheader, strlen(buildheader) + 1);
        free(buildheader);
    }
    return header;
}

int send_http_response_header_to_client(int sd, char* header){
    return sendtext(sd, header);
}
int send_http_response_body_to_client(int sd, char* body){
    return sendtext(sd, body);
}

//returns 1 if this line holds the content length header line
int check_if_content_length_line(char* line){
    int is_content_length = 0;
    char con_line[] = "Content-Length:";
    is_content_length = strncmp(con_line, line, SIZEOF_CONTENTLENGTH);
    return !is_content_length;
}

int get_content_length(char* line){
    char length_as_string[MAX_STR_LEN];
    int start_of_length = 16;
    int end_of_length = strlen(line) - 2;//2 is \r, \n so "123\r\n" this will be 3
    int i;
    int index = start_of_length;
    for(i = 0; i < end_of_length; i++){
        length_as_string[i] = line[index];
        index += 1;
    }
    length_as_string[index] = '\0';
    int length_as_int;
    sscanf(length_as_string, "%d", &length_as_int);
    return length_as_int;
}

char *process_req_header(int sd, char *host){
    char *header;
    char *line;
    header = process_first_line(sd, host);
    int done = 0;
    while(!done){
        line = process_req_line(sd);
        strcat(header, line);
        done = req_done_processing(line);
        free(line);
    }
    printf("%s\n", header);
    return header;
}

//strips out the hostname
char *process_first_line(int sd, char* host){
    char *msg;
    msg = (char *) malloc(MAX_HEADER_LINE_SIZE);
    if (!msg) {
        fprintf(stderr, "error : unable to malloc\n");
        return (NULL);
    }
    /* read the message text */
	char * ptr;
	ptr = msg;
    int toberead = 1;
    char last_byte;
    int counter = 0;
    int first_space = 0;
    int slash_counter = 0;
    int space_done = 0;
    int host_counter = 0;
	while (toberead > 0) {
		int byteread;
		byteread = read(sd, ptr, toberead);
		if (byteread < 0) {
			if (byteread == -1)
				perror("read");
			return (0);
		} else if(!byteread){
            return msg;
        }
        last_byte = *ptr;
        if(slash_counter == 2 && last_byte != '/'){
            host[host_counter] = last_byte;
            host_counter += 1;
        }
        if(first_space && !space_done){
            if(last_byte == '/'){
                slash_counter += 1;
            }
            ptr -= 1;
            if(slash_counter == 3){
                space_done = 1;
                ptr += 1;
            }
        }
        if(last_byte == ' ' && !first_space && !space_done) first_space = 1;
        if(last_byte == '\n' && *(ptr - 1) == '\r') return msg;
		ptr += byteread;
        counter += 1;
	}
	/* done reading */
	return (msg);
}

char* process_req_line(int sd){
    char *msg;
    msg = (char *) malloc(MAX_HEADER_LINE_SIZE);
    if (!msg) {
        fprintf(stderr, "error : unable to malloc\n");
        return (NULL);
    }
    /* read the message text */
	char * ptr;
	ptr = msg;
    int toberead = 1;
    char last_byte;
    int counter = 0;
    int carriage_flag = 0;
	while (toberead > 0) {
		int byteread;
		byteread = read(sd, ptr, toberead);
		if (byteread < 0) {
			if (byteread == -1)
				perror("read");
			return (0);
		} else if(!byteread){
            return msg;
        }
        last_byte = msg[counter];
        if(last_byte == '\n' && carriage_flag){
            ptr += 1;
            msg[counter + 1] = '\0';
            return msg;
        }
        if(last_byte == '\r') carriage_flag = 1;
		ptr += byteread;
        counter += 1;
	}
	/* done reading */
	return (msg);
}

int sendtext(int sd, char *msg) {
	long len;

	len = (msg ? strlen(msg) + 1 : 0);
    /*
	len = htonl(len);
	write(sd, (char *) &len, sizeof(len));

	len = ntohl(len);
    */
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

