/*
 * command.c
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"

#define BUFFER_LEN 1024

static char buffer[BUFFER_LEN];

uint8_t command_read(FILE * input, command_ptr command) {
    if (input == NULL) {
        fprintf(stderr, "fatal: tried to read command from NULL file\n");
        exit(EXIT_FAILURE);
    }
    if (command == NULL) {
        fprintf(stderr, "fatal: tried to read command into a NULL command pointer\n");
        exit(EXIT_FAILURE);
    }

    command_type_t type;
    if (fscanf(input, "%1023s", &buffer) == EOF) {
        return 1;
        fprintf(stderr, "fatal: encountered EOF while reading command type\n");
        exit(EXIT_FAILURE);
    }
    if (strcmp((const char *) &buffer, "i") == 0) {
        type = INSERT;
    } else if (strcmp((const char *) &buffer, "d") == 0) {
        type = DELETE;
    } else if (strcmp((const char *) &buffer, "f") == 0) {
        type = FIND;
    } else if (strcmp((const char *) &buffer, "l") == 0) {
        type = LIST;
    } else {
        type = UNKNOWN;
    }

    size_t read = 0;
    int c;
    while ((c = fgetc(input)) != EOF) {
        if (c == '\n') {
            break;
        }
        buffer[read] = (char) c;
        read += 1;
        if (read == BUFFER_LEN - 2) {
            fprintf(stderr, "warn: reached 1024 (1023 + \'\\o\') char limit of command read buffer\n");
            break;
        }
    }
    buffer[read] = '\0';

    if (buffer[0] == ' ') {
        memmove(&buffer[0], &buffer[1], read);
    }

    command->type = type;
    switch (type) {
        case INSERT:
        case DELETE:
        case FIND:
            command->arg = malloc((read + 1) * sizeof(char));
            memcpy(command->arg, &buffer, (read + 1) * sizeof(char));
            break;
        default:
            command->arg = NULL;
            break;
    }

    return c == EOF ? 1 : 0;
}
