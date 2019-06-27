/* sblt.h 
 * Autor: Kirill Pshenichny  
 */

#ifndef SBLT_H
#define SBLT_H

#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_FILENAME "/tmp/sbltd.sock"

enum{
     CMD_ECHO,
     CMD_SET,
     CMD_CNG,
     CMD_GET
};

struct packet{
  char command;
  int value;
};


#endif
