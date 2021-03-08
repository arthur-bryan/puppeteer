// MIT License
/*
 * This header file belongs to https://github.com/arthur-bryan/puppeteer
 * This file contains function declarations for:
 *      min(size_t, size_t): gets the minimum between two values
 *      write_all(FILE *, void *, size_t): writes the buffer to the file
 *      receive_file(int16_t): receives file over a connected socket
 */
#ifndef DOWNLOAD_FILE_H
#define DOWNLOAD_FILE_H

#include <stdio.h>
#include <stdint.h>

#define DATA_BLOCK_SIZE 3000000    // 3 MB

size_t
min(size_t, size_t);

int8_t
write_all(FILE *, void *, size_t);

int8_t
receive_file(int16_t);

#endif //DOWNLOAD_FILE_H