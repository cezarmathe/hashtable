/*
 * table.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "table.h"

table_entry_ptr table_entry_new_ptr(uint64_t key, char * value) {
    table_entry_ptr entry = malloc(sizeof(table_entry_t));
    if (entry == NULL) {
        fprintf(stderr, "fatal: failed to allocate memory for a new table entry");
        exit(EXIT_FAILURE);
    }
    entry->key = key;
    entry->value = value;
    return entry;
}

void table_entry_append(table_entry_ptr base, table_entry_ptr entry) {
    if (base == NULL) {
        fprintf(stderr, "fatal: tried to append to a NULL base table entry");
        exit(EXIT_FAILURE);
    }
    if (entry == NULL) {
        fprintf(stderr, "fatal: tried to append a NULL table entry");
        exit(EXIT_FAILURE);
    }
    table_entry_ptr next = base->next;
    while (next != NULL) {
        next = next->next;
    }
    next->next = entry;
}

void table_entry_delete(table_entry_ptr base, uint64_t key) {
    if (base == NULL) {
        fprintf(stderr, "fatal: tried to delete a key from a NULL base entry");
        exit(EXIT_FAILURE);
    }
    if (base->key == key) {
        fprintf(stderr, "fatal: tried to delete the base entry");
        exit(EXIT_FAILURE);
    }
    table_entry_ptr previous = base;
    table_entry_ptr next = base->next;
    while (next != NULL) {
        if (next->key == key) {
            previous->next = next->next;
            next->next = NULL;
            table_entry_free(next);
            return;
        }
        previous = next;
        next = next->next;
    }
}

uint8_t table_entry_find(table_entry_ptr base, uint64_t key) {
    if (base == NULL) {
        fprintf(stderr, "fatal: tried to find a key in a NULL base entry");
        exit(EXIT_FAILURE);
    }
    table_entry_ptr next = base;
    while (next != NULL) {
        if (next->key == key) {
            return 1;
        }
        next = next->next;
    }
    return 0;
}

void table_entry_free(table_entry_ptr entry) {
    if (entry == NULL) {
        fprintf(stderr, "fatal: tried to free a NULL table entry pointer");
        exit(EXIT_FAILURE);
    }
    free(entry->value);
    if (entry->next != NULL) {
        table_entry_free(entry->next); // recursion bad
    }
    free(entry);
}

table_t table_new() {
    table_t table;
    table.inner = malloc(sizeof(table_entry_ptr));
    memset(&table.inner, 0, sizeof(table_entry_ptr));
    if (table.inner == NULL) {
        fprintf(stderr, "fatal: failed to allocate memory when creating a new hash table");
        exit(EXIT_FAILURE);
    }
    table.capacity = 1;
    return table;
}

void table_insert(table_ptr table, char * value) {
    if (table == NULL) {
        fprintf(stderr, "fatal: tried to insert a value in a NULL table");
        exit(EXIT_FAILURE);
    }
    uint64_t key = hash(value); // let's hope it's an unsigned char
    size_t index = key % TABLE_SIZE; // guaranteed to be less than TABLE_SIZE
    if (index > table->capacity - 1) {
        size_t new_capacity = index + 1;
        table->inner = realloc(table->inner, new_capacity * sizeof(table_entry_ptr));
        if (table->inner == NULL) {
            fprintf(stderr, "fatal: failed to allocate more memory for the hash table");
            exit(EXIT_FAILURE);
        }
        memset(&table->inner[table->capacity], 0, (new_capacity - table->capacity) * sizeof(table_entry_ptr));
        table->capacity = new_capacity;
    }
    table_entry_ptr entry = table_entry_new_ptr(key, value);
    if (table->inner[index] == NULL) {
        table->inner[index] = table_entry_new_ptr(key, value);
    } else {
        table_entry_append(table->inner[index], entry);
    }
}

void table_delete(table_ptr table);

void table_find(table_ptr table);

void table_list(table_ptr table);
