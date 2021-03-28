/*
 * main.c
 */

#include <stdio.h>
#include <stdlib.h>

#include "command.h"
#include "log.h"
#include "table.h"

#define TAG "main"

#define LOG_LEVEL INFO

int main() {
    log_set_level(LOG_LEVEL);
    log_set_file(stderr);

    LOG(INFO, TAG, "hello, this is hashtable");

    LOG(DEBUG, TAG, "creating hash_djb2 table");
    table_t table = table_new(100);

    LOG(DEBUG, TAG, "starting to read commands");
    command_t command;
    while (command_read(stdin, &command) == 0) {
        LOG(DEBUG, TAG, "read command, type: %d, arg: %s", command.type, command.arg);

        switch (command.type) {
            case INSERT:
                if (command.arg == NULL) {
                    LOG(ERROR, TAG, "insert requires an argument, not proceeding");
                } else {
                    LOG(DEBUG, TAG, "inserting \'%s\' in the hash_djb2 table", command.arg)
                    table_insert(&table, command.arg);
                }
                break;
            case DELETE:
                if (command.arg == NULL) {
                    LOG(ERROR, TAG, "delete requires an argument, not proceeding");
                } else {
                    table_delete(&table, command.arg);
                    free(command.arg);
                }
                break;
            case FIND:
                if (command.arg == NULL) {
                    LOG(ERROR, TAG, "find requires an argument, not proceeding");
                } else {
                    ssize_t index = table_find(&table, command.arg);
                    if (index == -1) {
                        fprintf(stdout, "no\n");
                    } else {
                        fprintf(stdout, "yes %zd\n", index);
                    }
                    free(command.arg);
                }
                break;
            case LIST:
                table_list(&table, stdout);
                break;
            default:
                LOG(WARN, TAG, "unknown command");
                break;
        }
    }
    LOG(DEBUG, TAG, "finished reading commands");

    LOG(INFO, TAG, "goodbye, this was hashtable");
    return 0;
}
