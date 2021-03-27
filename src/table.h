/*
 * table.h
 */

#include <stddef.h>
#include <stdint.h>

#ifndef HASHTABLE_TABLE_H
#define HASHTABLE_TABLE_H

#define TABLE_SIZE INT16_MAX

/**
 * An entry in the hash table.
 */
typedef struct table_entry_t {
    // the key (hash) of the entry
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
 * A hash table.
 */
typedef struct table_t {
    // pointer to the list of table entries
    table_entry_ptr * inner;
    // the capacity of the list of table entries (capped at TABLE_SIZE)
    size_t capacity;
} table_t;

/**
 * Pointer to a hash table.
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
 * Append a new entry at the end of the list.
 * @param base the base entry to append to
 * @param entry the entry to append
 */
void table_entry_append(table_entry_ptr base, table_entry_ptr entry);

/**
 * Delete an entry from a table entry list.
 * @details Must not be the key of base!
 * @param base the base entry
 * @param key the key to delete
 */
void table_entry_delete(table_entry_ptr base, uint64_t key);

/**
 * Find a key in a table entry list.
 * @param base the base entry
 * @param key the key to find
 * @return 1 if found, 0 otherwise
 */
uint8_t table_entry_find(table_entry_ptr base, uint64_t key);

/**
 * Free a table entry.
 * @details The pointer is unusable afterwards. The next table entry is freed,
 *          too, if not NULL.
 * @param entry the entry to be freed
 */
void table_entry_free(table_entry_ptr entry);

/**
 * Create a new hast table.
 * @return a hash table
 */
table_t table_new();

/**
 * Insert a new value into the table.
 * @param table the table to insert into
 * @param value the value to insert
 */
void table_insert(table_ptr table, char * value);

void table_delete(table_ptr table);

void table_find(table_ptr table);

void table_list(table_ptr table);

#endif //HASHTABLE_TABLE_H
