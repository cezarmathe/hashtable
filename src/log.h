/*
 * log.h
 */

#include <stdio.h>

/**
 * A log level.
 */
typedef enum log_level {
    FATAL = 0,
    ERROR = 1,
    WARN  = 2,
    INFO  = 3,
    DEBUG = 4,
} log_level_t;

/**
 * Global log level.
 */
extern log_level_t global_log_level;

/**
 * Global file writer.
 */
extern FILE * global_log_writer;

/**
 * Set the maximum allowed log level.
 */
void log_set_level(log_level_t level);

/**
 * Set the file where logs should be written.
 * @param writer file to write logs to
 */
void log_set_file(FILE * writer);

/**
 * Convert a log level to string.
 * @param level the log level to convert
 * @return the string representation
 */
const char * log_level_to_string(log_level_t level);

#define LOG(level, tag, ...) \
    if (level <= global_log_level) { \
        fprintf(global_log_writer, "%s > %s: ", log_level_to_string(level), tag); \
        fprintf(global_log_writer, __VA_ARGS__); \
        fprintf(global_log_writer, "\n"); \
    }
