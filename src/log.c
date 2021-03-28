/*
 * log.c
 */

#include <stdio.h>

#include "log.h"

log_level_t global_log_level = INFO;

FILE * global_log_writer = NULL;

void log_set_level(log_level_t level) {
    global_log_level = level;
}

void log_set_file(FILE * writer) {
    global_log_writer = writer;
}

const char * log_level_to_string(log_level_t level) {
    switch (level) {
        case FATAL:
            return "fatal";
        case ERROR:
            return "error";
        case WARN:
            return "warn ";
        case INFO:
            return "info ";
        case DEBUG:
            return "debug";
    }
}
