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

  
  sockfd = socket(AF_UNIX,SOCK_STREAM,0);

  s_sun.sun_family = AF_UNIX;
  strcpy(s_sun.sun_path,SOCK_FILENAME);

  if(connect(sockfd,(struct sockaddr *) &s_sun,sizeof(s_sun)) < 0){
    printf("Error connect\n");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  //sprintf(buff,"hello!\n");
  //send(sockfd,buff,sizeof(buff),0);
  //bzero(buff,sizeof(buff));
  printf("recv\n");
  
  recv(sockfd,buff,sizeof(buff),0);
  printf("buff %s size: %d\n",buff,sizeof(buff));
  
  
  close(sockfd);
  
  
  return 0;
}
