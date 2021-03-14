// MIT License
/*
 * This header file belongs to https://github.com/arthur-bryan/puppeteer
 *
 * Functions based on verify, copy and create files.
 *
 * Copyright (c) 2021 Arthur Bryan <arthurbryan2030@gmail.com>
 */
#include "../include/footprint.h"

#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>


/* Verifies if the file exists getting its info by the filename
 *
 * Parameters:
 *      char *filename: string containing the filename
 *
 * Returns:
 *      Return 0 if fails to get the file info, 1 otherwise
 */
int8_t
file_exists(const char *filename) {
    struct stat     buffer;
    return (stat(filename, &buffer) == 0);
}

/* Creates a directory (if not created)
 *
 * Parameters:
 *      const char *path: string containing the dir name
 *
 * Returns:
 *      Return 0 if fails to create, 1 otherwise
 */
uint8_t
create_dir(const char *path) {
    return execl("/usr/bin/mkdir", "mkdir", "-p", path, NULL) != -1;
}

/* Copies the the file to the destination path
 *
 * Parameters:
 *      char *filename: the name of the file to be copied
 *      char *destination_path: the directory that the file will be copied to
 *
 * Returns:
 *      Return 1 if the file was copied, 1 otherwise
 */
uint8_t
hide_file(const char *filename, const char *destination_path) {
    struct stat file_struct;
    FILE *file;
    uint32_t file_size;

    file = fopen(filename, "rb");
    stat(filename, &file_struct);
    file_size = file_struct.st_size;
    if (file == NULL) {
        return 0;
    }
    char *data_buffer = calloc(file_size, sizeof(char));
    if (data_buffer == NULL) {
        fclose(file);
        return 0;
    }
    if (fread(data_buffer, sizeof(char), file_size, file) != file_size) {
        free(data_buffer);
        fclose(file);
        return 0;
    }
    FILE *destination_file;
    destination_file = fopen(destination_path, "wb");
    if (destination_file == NULL) {
        fclose(file);
        free(data_buffer);
        return 0;
    }
    if (fwrite(data_buffer, sizeof(char), file_size, destination_file) !=
        file_size) {
        free(data_buffer);
        fclose(file);
        fclose(destination_file);
        return 0;
    }
    if (chmod(destination_path, (00777)) != 0) {
        fclose(file);
        fclose(destination_file);
        free(data_buffer);
        return 0;
    }
    fclose(file);
    fclose(destination_file);
    free(data_buffer);
    return 1;
}
