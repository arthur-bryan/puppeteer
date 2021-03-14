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
#ifndef UTSNAME_WINDOWS_H
#define UTSNAME_WINDOWS_H

#define UTSNAME_FIELD_MAXLEN 65

#include <stdint.h>

typedef struct w_utsname {
    char    sysname[UTSNAME_FIELD_MAXLEN];
    char    nodename[UTSNAME_FIELD_MAXLEN];
    char    release[UTSNAME_FIELD_MAXLEN];
    char    version[UTSNAME_FIELD_MAXLEN];
    char    machine[UTSNAME_FIELD_MAXLEN];
} w_utsname;

uint16_t
uname(struct w_utsname *);

#endif // UTSNAME_WINDOWS_H