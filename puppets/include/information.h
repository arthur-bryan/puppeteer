// MIT License
/*
 * This header file belongs to https://github.com/arthur-bryan/puppeteer
 * This file contains struct declaration for:
 *      struct bot:  structure to store bot information like architecture and OS
 *
 * This file contains function declarations for:
 *      get_info(char *, char *, char*, char*): gets the infos as strings
 *      send_info(int16_t): send the bot info over socket
 */
#ifndef INFORMATION_H
#define INFORMATION_H

#include <stdint.h>

typedef struct host {
    char    op_system[65];
    char    architecture[65];
    char    release[65];
    char    hostname[65];
    char    username[65];
    uint8_t autorun_enabled;

} host_t;

host_t
get_host_info(void);

void
send_host_info(int16_t socket_fd, host_t host);

#endif // INFORMATION_H
