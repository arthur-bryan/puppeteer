// MIT License
/*
 * This header file belongs to https://github.com/arthur-bryan/puppeteer
 * This file contains function declarations for:
 *      persistence(const char *): tries persistence methods till one succeeds
 *      create_desktop_autostart(const char *): creates a .desktop file
 *      create_cron_job(const char *): creates a cron job
 */
#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <stdint.h>

uint8_t
persistence(const char *);

uint8_t
create_desktop_autostart(const char *);

uint8_t
create_cron_job(const char *);

#endif // PERSISTENCE_H
