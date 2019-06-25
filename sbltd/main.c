#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <string.h>
#include <sys/socket.h>

#define PID_FILENAME "/tmp/sbltd.pid"
#define SOCK_FILENAME "/tmp/sbltd.sock"
#define BUFFER_SIZE 1024

int main(int argc,char **argv){
  pid_t sid, pid;                  // var for sid and pid
  int fd_pid;
  char buff[BUFFER_SIZE];

  int sockfd;                      // socket file descriptor
  
  /* create children process */
  pid = fork();
  if(pid < 0){
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
  
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  /* start daemon */


  
  
  return 0;
}
