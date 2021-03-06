// MIT License
/*
 * This header file belongs to https://github.com/arthur-bryan/puppeteer
 * This file contains function declarations for:
 *      min(size_t, size_t): gets the minimum between two values
 *      send_file(int16_t): sends file over a connected socket
 */
#ifndef SEND_FILE_H
#define SEND_FILE_H

#include <stdio.h>  // fopen(), fread(), fclose(), size_t
#include <stdint.h> // integer types

#define DATA_BLOCK_SIZE 3000000  // 3 MB

size_t
min(size_t, size_t);

int8_t
send_file(int16_t);

#endif  // SEND_FILE_H
