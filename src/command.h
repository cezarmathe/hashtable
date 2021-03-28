/*
 * command.h
 */

#include <stdint.h>
#include <stdio.h>

#ifndef HASHTABLE_COMMAND_H
#define HASHTABLE_COMMAND_H

/**
 * A command type.
 */
typedef enum command_type {
    UNKNOWN = 0,
    INSERT  = 1,
    DELETE  = 2,
    FIND    = 3,
    LIST    = 4
} command_type_t;

/**
 * A command.
 */
typedef struct command {
    // the command type
    command_type_t type;
    // the command arg
    char * arg;
} command_t;

/**
 * Pointer to a command.
 */
typedef command_t * command_ptr;

/**
 * Read a command.
 * @param input the file to read the input from
 * @param command pointer where to place the command
 * @return 0 on success, 1 otherwise
 */
uint8_t command_read(FILE * input, command_ptr command);

#endif //HASHTABLE_COMMAND_H
