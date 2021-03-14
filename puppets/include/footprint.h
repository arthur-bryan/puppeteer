// MIT License
/*
 * This header file belongs to https://github.com/arthur-bryan/puppeteer
 * This file contains function declarations for:
 *      file_exists(const *char): verifies if the file exists
 *      create_dir(const char *): executes a command and grabs output
 *      hide_file(const char *, const char *): copies file to another location
 */
#ifndef FOOTPRINT_H
#define FOOTPRINT_H

#include <stdint.h>
#include <stdlib.h>

int8_t
file_exists(const char *);

uint8_t
create_dir(const char *);

uint8_t
hide_file(const char *, const char *);

#endif // FOOTPRINT_H
