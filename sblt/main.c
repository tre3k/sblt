/*
 *  Copyright (c) 2019-2022 Kirill Pshenichnyi, Gatchina, LO, 188300 Russia
 *
 *  This file is part of sblt
 *
 *  sblt is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  sblt is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with sblt.  If not, see <https://www.gnu.org/licenses/>.
 *
 *     Author: Kirill Pshenichnyi <pshcyrill@mail.ru>
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "sblt.h"

int main(int argc,char **argv){
	int sockfd;
	struct sockaddr_un s_sun;

	if(argc < 2){
		printf("%s <value> (in %)\n",argv[0]);
		printf("\t <value> - absolute\n"
		       "\t+<value> - add value\n"
		       "\t-<value> - sub value\n"
		       "\t?        - get current value of brightness\n");
		exit(EXIT_SUCCESS);
	}

	struct packet pack;

	/* create unix socket */
	sockfd = socket(AF_UNIX,SOCK_STREAM,0);

	s_sun.sun_family = AF_UNIX;
	strcpy(s_sun.sun_path,SOCK_FILENAME);  // path to socket file

	/* connect to unix socket */
	if(connect(sockfd,(struct sockaddr *) &s_sun,sizeof(s_sun)) < 0){
		printf("Error connect, daemon not run?\n");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	/* send command and value */
	switch(argv[1][0]){
	case '+':
		pack.command = CMD_CNG;
		pack.value = atoi(argv[1]);
		break;

	case '-':
		pack.command = CMD_CNG;
		pack.value = atoi(argv[1]);
		break;

	case '?':
		pack.command = CMD_GET;
		pack.value = 0;
		break;

	default:
		pack.command = CMD_SET;
		pack.value = atoi(argv[1]);
		break;
	}

	send(sockfd,&pack,sizeof(pack),0);
	if(argv[1][0]=='?'){
		recv(sockfd,&pack,sizeof(pack),0);
		printf("current brightness: %d\n",pack.value);
	}
	close(sockfd);
	return 0;
}
