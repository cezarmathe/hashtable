/*
 * command.c
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "log.h"

#define BUFFER_LEN 1024

#define TAG "command"

static char buffer[BUFFER_LEN];

uint8_t command_read(FILE * input, command_ptr command) {
    if (input == NULL) {
        LOG(FATAL, TAG, "tried to read command from NULL file");
        exit(EXIT_FAILURE);
    }
    if (command == NULL) {
        LOG(FATAL, TAG, "tried to read command into a NULL command pointer");
        exit(EXIT_FAILURE);
    }

    LOG(DEBUG, TAG, "reading command");
    command_type_t type;
    if (fscanf(input, "%1023s", (char *) &buffer) == EOF) {
        LOG(DEBUG, TAG, "got EOF, returning 1");
        return 1;
    }
    LOG(DEBUG, TAG, "read command: %s", (char *) &buffer);
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
            LOG(WARN, TAG, "reached 1024 (1023 + \'\\o\') char limit of command read buffer");
            break;
        }
    }
    buffer[read] = '\0';
    LOG(DEBUG, TAG, "read command arg: %s", (char *) &buffer);

    if (buffer[0] == ' ') {
        memmove(&buffer[0], &buffer[1], read);
    }
    if (read > 1) {
        size_t last = read - 2;
        for (size_t i = last; i >= 0; i--) {
            if (buffer[i] != ' ') {
                last = i;
                break;
            }
        }
        buffer[last + 1] = '\0';
    }

    command->type = type;
    switch (type) {
        case INSERT:
        case DELETE:
        case FIND:
            if (strlen((char *) &buffer) == 0) {
                LOG(ERROR, TAG, "command requires an argument");
                command->arg = NULL;
            } else {
                LOG(DEBUG, TAG, "keeping command arg");
                command->arg = malloc((read + 1) * sizeof(char));
                memcpy(command->arg, &buffer, (read + 1) * sizeof(char));
            }
            break;
        default:
            LOG(DEBUG, TAG, "this command did not need an arg");
            command->arg = NULL;
            break;
    }

    return c == EOF ? 1 : 0;
}
