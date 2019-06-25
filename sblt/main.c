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

  /*
  if(argc < 2){
    printf("%s <percent value>\n",argv[0]);
    exit(EXIT_SUCCESS);
  }
  */
  
  sockfd = socket(AF_UNIX,SOCK_STREAM,0);

  s_sun.sun_family = AF_UNIX;
  strcpy(s_sun.sun_path,SOCK_FILENAME);

  if(connect(sockfd,(struct sockaddr *) &s_sun,sizeof(s_sun)) < 0){
    printf("Error connect\n");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  sprintf(buff,"%s",argv[1]);
  send(sockfd,buff,BUFFER_SIZE,0);
  
  close(sockfd);
  
  
  return 0;
}
