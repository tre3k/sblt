/* main.c - sbltd (Simple BackLight Daemon) 
 *  
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <string.h>

#include "sblt.h"

#define PID_FILENAME "/tmp/sbltd.pid"
#define BUFFER_SIZE 1024
#define LISTEN 10

/* function for write message to log file */
void log_message(char *message){
  printf("%s\n",message);
  return;
}

void read_config(){
  return;
}

int main(int argc,char **argv){
  pid_t sid, pid;                  // var for sid and pid
  int fd_pid;
  char buff[BUFFER_SIZE];

  int sockfd;                      // socket file descriptor
  struct sockaddr_un s_sun;
  int acp;                         // for accept
  
  
  /* create children process */
  pid = fork();
  if(pid < 0){
    log_message("Error fork");
    exit(EXIT_FAILURE);
  }

  /* write PID to file */
  if(pid > 0){
    printf("PID: %d\n",pid);
    fd_pid = open(PID_FILENAME, O_CREAT|O_RDWR);
    bzero(buff,BUFFER_SIZE);

    sprintf(buff,"%d\x00",pid);
    write(fd_pid,buff,strlen(buff));
    chmod(PID_FILENAME, S_IRUSR | S_IWUSR);

    close(fd_pid);
    exit(EXIT_SUCCESS); 
  }

  umask(0);
  sid = setsid();

  if(chdir("/") < 0){
    exit(EXIT_FAILURE);
  }

  /*
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
  */

  /* start daemon */

  sockfd = socket(AF_UNIX,SOCK_STREAM,0);   // create unix socket for server
  if(sockfd < 0){                           // if error
    log_message("Error create socket");
    exit(EXIT_FAILURE);
  }

  /* bind and listen */
  s_sun.sun_family = AF_UNIX;
  strcpy(s_sun.sun_path,SOCK_FILENAME);
  unlink(SOCK_FILENAME);
  if(bind(sockfd,(struct sockaddr *)&s_sun,sizeof(s_sun)) < 0){
    log_message("Error bind");
    exit(EXIT_FAILURE);
  }
  if(listen(sockfd,LISTEN) < 0){
    log_message("Error listen");
    exit(EXIT_FAILURE);
  }

  log_message("Start main loop");
  /* main loop */
  while(1){
    acp = accept(sockfd,NULL,NULL);
    if(acp < 0){
      log_message("Error accept");
      exit(EXIT_FAILURE);
    }
    bzero(buff,BUFFER_SIZE);
    recv(sockfd,buff,sizeof(buff),0);
  }
  
  
  return 0;
}
