/*
 * main.c
 */

#include <stdio.h>
#include <string.h>

#include "command.h"

#define BUFFER_LENGTH 1024

int main() {
    fprintf(stderr, "--> hello, this is hashtable\n\n");

    char buf[BUFFER_LENGTH];
    memset(&buf, 0, sizeof(char) * BUFFER_LENGTH);

    command_t command;
    while (command_read(stdin, &command) == 0) {
        fprintf(stderr, "command: %d, arg: %s\n", command.type, command.arg);
    }

    fprintf(stderr, "\n--> goodbye, this was hashtable\n");
    return 0;
}
