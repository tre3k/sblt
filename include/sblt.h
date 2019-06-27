#ifndef SBLT_H
#define SBLT_H

#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_FILENAME "/tmp/sbltd.sock"
#define BUFFER_SIZE 1024                    // packet size

enum{
     CMD_ECHO,
     CMD_SET,
     CMD_ADD,
     CMD_SUB,
     CMD_GET
};

struct packet{
  char command;
  int value;
};


#endif
