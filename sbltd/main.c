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


/* need get this values from config */
#define PID_FILENAME "/tmp/sbltd.pid"
#define LISTEN 10
//need autofind this paths
#define BRIGHTNESS_PATH "/sys/class/backlight/intel_backlight/brightness"
#define MAX_BRIGHTNESS_PATH "/sys/class/backlight/intel_backlight/max_brightness"
#define LOG_PATH "/var/log/sbltd.log"


/* function for write message to log file */
void log_message(char *message){
  /* set time */
  /* write message to file */

  return;
}

void read_config(){
  return;
}

int get_max_value_brightness(void){
  int fd = open(MAX_BRIGHTNESS_PATH,O_RDONLY);
  if(fd < 0){
    log_message("Error get max value brightness");
  }
  char buff[16];
  char tmp;
  int i;

  i=0;
  while(read(fd,&tmp,1) > 0){
    if(i>15) break;
    buff[i] = tmp;
    i++;
  }
  
  close(fd);
  return atoi(buff);
}

void set_brightness(int value){
  int fd = open(BRIGHTNESS_PATH,O_WRONLY);
  if(fd < 0){
    log_message("Error open brightness");
  }

  char buff[16];
  if(write(fd,buff,strlen(buff)) < 0){
    log_message("Error write to brightness");
  }
  
  close(fd);
  return;
}

int main(int argc,char **argv){
  pid_t sid, pid;                  // var for sid and pid
  int fd_pid;
  char buff[BUFFER_SIZE];

  int sockfd;                      // socket file descriptor
  struct sockaddr_un s_sun;
  int client_sock;                 // for accept

  int p_value,value;
  int max_value = get_max_value_brightness();

  
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

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

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
    close(sockfd);
    exit(EXIT_FAILURE);
  }


  log_message("Start main loop");
  /* main loop */
  while(1){
    client_sock = accept(sockfd,NULL,NULL);
    if(client_sock < 0){
      log_message("Error accept");
      close(sockfd);
      close(client_sock);
      exit(EXIT_FAILURE);
    }

    /* recevery value */
    bzero(buff,BUFFER_SIZE);
    recv(client_sock,buff,BUFFER_SIZE,0);

    p_value = atoi(buff);
    value = p_value*max_value/100;
    if(value > max_value) value = max_value;

    set_brightness(value);
  }
  
  
  return 0;
}
