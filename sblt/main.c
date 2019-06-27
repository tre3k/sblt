/* main.c - sblt (Simple BackLight Client)
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "sblt.h"

int main(int argc,char **argv){
  int sockfd;
  struct sockaddr_un s_sun;
  char buff[BUFFER_SIZE];

  if(argc < 2){
    printf("%s <value> (in %)\n",argv[0]);
    printf("\t <value> - absolute\n"
	   "\t+<value> - add value\n"
	   "\t-<value> - sub value\n");
    exit(EXIT_SUCCESS);
  }
  
  struct packet pack;
  
  sockfd = socket(AF_UNIX,SOCK_STREAM,0);

  s_sun.sun_family = AF_UNIX;
  strcpy(s_sun.sun_path,SOCK_FILENAME);

  if(connect(sockfd,(struct sockaddr *) &s_sun,sizeof(s_sun)) < 0){
    printf("Error connect\n");
    close(sockfd);
    exit(EXIT_FAILURE);
  }



  switch(argv[1][0]){
  case '+':
    pack.command = CMD_ADD;
    pack.value = atoi(argv[1]);
    
    break;

  default:
    pack.command = CMD_SET;
    pack.value = atoi(argv[1]);
    break;
  }
  
  
  send(sockfd,&pack,sizeof(pack),0);
  close(sockfd);
  
  return 0;
}
