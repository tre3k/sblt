/*
 *  Copyright (c) 2019-2025 Kirill Pshenichnyi, Gatchina, LO, 188300 Russia
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

#ifndef _SBLT_H_
#define _SBLT_H_

#include <sys/socket.h>
#include <sys/un.h>

#define PID_FILENAME "/tmp/sbltd.pid"
#define SOCK_FILENAME "/tmp/sbltd.sock"

#define BRIGHTNESS_DEV "/sys/class/backlight/intel_backlight/"
#define BRIGHTNESS_PATH BRIGHTNESS_DEV "brightness"
#define MAX_BRIGHTNESS_PATH BRIGHTNESS_DEV "max_brightness"

enum {
        CMD_ECHO,
        CMD_SET,
        CMD_CNG,
        CMD_GET
};

struct packet {
        char command;
        int value;
};

#endif
