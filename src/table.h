/*
 * table.h
 */

#include <stddef.h>
#include <stdint.h>

#ifndef HASHTABLE_TABLE_H
#define HASHTABLE_TABLE_H

/**
 * An entry in the hash_djb2 table.
 */
typedef struct table_entry_t {
    // the key (hash_djb2) of the entry
    uint64_t key;
    // the inner value of the entry
    char * value;
    // the next entry in the list
    struct table_entry_t * next;
} table_entry_t;

/**
 * Pointer to a table entry.
 */
typedef table_entry_t * table_entry_ptr;

/**
 * A hash_djb2 table.
 */
typedef struct table {
    // pointer to the list of table entries
    table_entry_ptr * inner;
    // the capacity of the list of table entries (capped at table::size)
    size_t capacity;
    // the maximum size of the table
    size_t size;
} table_t;

/**
 * Pointer to a hash_djb2 table.
 */
typedef table_t * table_ptr;

/**
 * Create a new table entry.
 * @param key the key of the table entry
 * @param value the inner value of the table entry
 * @return dynamically-allocated table entry
 */
table_entry_ptr table_entry_new_ptr(uint64_t key, char * value);

/**
 * Free a table entry.
 * @details The pointer is unusable afterwards. The next table entry is freed,
 *          too, if not NULL.
 * @param entry the entry to be freed
 */
void table_entry_free(table_entry_ptr entry);

/**
 * Create a new hast table.
 * @param size the size of the inner table
 * @return a hash_djb2 table
 */
table_t table_new(size_t size);

/**
 * Insert a new value into the table.
 * @param table the table to insert into
 * @param value the value to insert
 */
void table_insert(table_ptr table, char * value);

/**
 * Delete a value from the table
 * @param table the table to delete from
 * @param value the value to delete
 */
void table_delete(table_ptr table, const char * value);

/**
 * Find a value in the table
 * @param table the table to search
 * @param value the value to find
 * @return -1 if not found, otherwise the index in the table
 */
ssize_t table_find(table_ptr table, const char * value);

/**
 * List the contents of the table.
 * @param table pointer to the table
 * @param writer file to write to
 */
void table_list(table_ptr table, FILE * writer);

#endif //HASHTABLE_TABLE_H
