/*
 *  Copyright (c) 2019-2025 Kirill Pshenichnyi, Gatchina, LO, 188300 Russia
 *
 *  This file is part of sbltd
 *
 *  sbltd is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  sbltd is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with sbltd.  If not, see <https://www.gnu.org/licenses/>.
 *
 *     Author: Kirill Pshenichnyi <pshcyrill@mail.ru>
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "sblt.h"

/* function for write message to log file */
void log_message(char *message) {
        /* Just out to standard output. You can use journalctl */
        printf("sbltd: %s\n", message);
        return;
}

void read_config() { return; }

int get_max_value_brightness(void) {
        int fd = open(MAX_BRIGHTNESS_PATH, O_RDONLY);
        if (fd < 0) {
                log_message("Error get max value brightness");
        }
        char buff[16];
        char tmp;
        int i;

        i = 0;
        while (read(fd, &tmp, 1) > 0) {
                if (i > 15) break;
                buff[i] = tmp;
                i++;
        }

        close(fd);
        return atoi(buff);
}

int get_brightness(void) {
        int fd = open(BRIGHTNESS_PATH, O_RDONLY);
        if (fd < 0) {
                log_message("Error get value of brightness");
        }
        char buff[16];
        char tmp;
        int i;

        i = 0;
        while (read(fd, &tmp, 1) > 0) {
                if (i > 15) break;
                buff[i] = tmp;
                i++;
        }

        close(fd);
        return atoi(buff);
}

void set_brightness(int value) {
        char buff[16];
        int fd = open(BRIGHTNESS_PATH, O_WRONLY);

        if (fd < 0) {
                log_message("Error open brightness");
        }

        sprintf(buff, "%d", value);
        if (write(fd, buff, strlen(buff)) < 0) {
                log_message("Error write to brightness");
        }

        close(fd);
        return;
}

int main(int argc, char **argv) {
        const unsigned int BUFFER_SIZE = 128;
        const unsigned int LISTEN = 10;

        bool toggle = true;
        int saved_toggle_value;
        pid_t pid;
        int fd_pid;
        char buff[BUFFER_SIZE];

        int sockfd;
        struct sockaddr_un s_sun;
        int client_sock;

        int value;
        int max_value = get_max_value_brightness();

        struct packet pack;

        /* create children process */
        pid = fork();
        if (pid < 0) {
                log_message("Error fork");
                exit(EXIT_FAILURE);
        }

        /* write PID to file */
        if (pid > 0) {
                printf("PID: %d\n", pid);
                fd_pid = open(PID_FILENAME, O_CREAT | O_RDWR);
                bzero(buff, BUFFER_SIZE);

                sprintf(buff, "%d\n", pid);
                write(fd_pid, buff, strlen(buff));
                chmod(PID_FILENAME, S_IRUSR | S_IWUSR);

                close(fd_pid);
                exit(EXIT_SUCCESS);
        }

        umask(0);
        setsid();

        if (chdir("/") < 0) {
                exit(EXIT_FAILURE);
        }

        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        /* start daemon */
        sockfd =
            socket(AF_UNIX, SOCK_STREAM, 0);  // create unix socket for server
        if (sockfd < 0) {                     // if error
                log_message("Error create socket");
                exit(EXIT_FAILURE);
        }

        /* bind and listen */
        s_sun.sun_family = AF_UNIX;
        strcpy(s_sun.sun_path, SOCK_FILENAME);

        unlink(SOCK_FILENAME);

        if (bind(sockfd, (struct sockaddr *)&s_sun, sizeof(s_sun)) < 0) {
                log_message("Error bind");
                exit(EXIT_FAILURE);
        }

        if (listen(sockfd, LISTEN) < 0) {
                log_message("Error listen");
                close(sockfd);
                exit(EXIT_FAILURE);
        }

        log_message("Start main loop");
        /* main loop */
        while (1) {
                client_sock = accept(sockfd, NULL, NULL);
                if (client_sock < 0) {
                        log_message("Error accept");
                        close(sockfd);
                        close(client_sock);
                        exit(EXIT_FAILURE);
                }

                /* recevery value */
                recv(client_sock, &pack, sizeof(pack), 0);

                value = pack.value * max_value / 100;
                if (value > max_value) value = max_value;

                switch (pack.command) {
                        case CMD_TOGGLE:
                                if (toggle) {
                                        saved_toggle_value = get_brightness();
                                        set_brightness(0);
                                } else {
                                        if (saved_toggle_value != 0) {
                                                set_brightness(
                                                    saved_toggle_value);
                                        } else {
                                                set_brightness(max_value);
                                        }
                                }
                                toggle = !toggle;
                                break;

                        case CMD_SET:
                                set_brightness(value);
                                break;

                        case CMD_CNG:
                                set_brightness(get_brightness() + value);
                                break;

                        case CMD_GET:
                                pack.command = CMD_GET;
                                pack.value = 100 * get_brightness() / max_value;
                                send(client_sock, &pack, sizeof(pack), 0);
                                break;
                }
        }

        return 0;
}
