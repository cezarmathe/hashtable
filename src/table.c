/*
 * table.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "log.h"
#include "table.h"

#define TAG_TABLE "table"
#define TAG_TABLE_ENTRY "table entry"

table_entry_ptr table_entry_new_ptr(uint64_t key, char * value) {
    table_entry_ptr entry = malloc(sizeof(table_entry_t));
    if (entry == NULL) {
        LOG(FATAL, TAG_TABLE_ENTRY, "failed to allocate memory for a new table entry");
        exit(EXIT_FAILURE);
    }
    entry->key = key;
    entry->value = value;
    return entry;
}

void table_entry_free(table_entry_ptr entry) {
    if (entry == NULL) {
        LOG(FATAL, TAG_TABLE_ENTRY, "tried to free a NULL table entry pointer");
        exit(EXIT_FAILURE);
    }
    free(entry->value);
    if (entry->next != NULL) {
        table_entry_free(entry->next); // recursion bad
    }
    free(entry);
}

table_t table_new(size_t size) {
    if (size == 0) {
        LOG(FATAL, TAG_TABLE, "cannot allocate a hash_djb2 table with size 0");
        exit(EXIT_FAILURE);
    }
    table_t table;
    table.inner = malloc(sizeof(table_entry_ptr));
    memset(table.inner, 0, sizeof(table_entry_ptr));
    if (table.inner == NULL) {
        LOG(FATAL, TAG_TABLE, "failed to allocate memory when creating a new hash_djb2 table");
        exit(EXIT_FAILURE);
    }
    table.capacity = 1;
    table.size = size;
    return table;
}

void table_insert(table_ptr table, char * value) {
    if (table == NULL) {
        LOG(FATAL, TAG_TABLE, "tried to insert a value in a NULL table");
        exit(EXIT_FAILURE);
    }
    if (value == NULL) {
        LOG(FATAL, TAG_TABLE, "tried to insert a NULL value");
        exit(EXIT_FAILURE);
    }
    if (table_find(table, value) >= 0) {
        LOG(WARN, TAG_TABLE, "value \'%s\' has been inserted before", value);
        return;
    }
    uint64_t key = hash_djb2(value); // let's hope it's an unsigned char
    size_t index = key % table->size; // guaranteed to be less than table::size
    if (index > table->capacity - 1) {
        size_t new_capacity = index + 1;
        table->inner = realloc(table->inner, new_capacity * sizeof(table_entry_ptr));
        if (table->inner == NULL) {
            LOG(FATAL, TAG_TABLE, "failed to allocate more memory for the hash_djb2 table");
            exit(EXIT_FAILURE);
        }
        memset(&table->inner[table->capacity], 0, (new_capacity - table->capacity) * sizeof(table_entry_ptr));
        table->capacity = new_capacity;
    }
    table_entry_ptr entry = table_entry_new_ptr(key, value);
    if (table->inner[index] == NULL) {
        table->inner[index] = table_entry_new_ptr(key, value);
    } else {
        table_entry_ptr base = table->inner[index];
        while (base->next != NULL) {
            base = base->next;
        }
        base->next = entry;
    }
}

void table_delete(table_ptr table, const char * value) {
    if (table == NULL) {
        LOG(FATAL, TAG_TABLE, "tried to delete from a NULL table");
        exit(EXIT_FAILURE);
    }
    if (value == NULL) {
        LOG(FATAL, TAG_TABLE, "tried to delete a NULL value");
        exit(EXIT_FAILURE);
    }
    ssize_t index = table_find(table, value);
    if (index == -1 || index > table->capacity - 1) {
        LOG(WARN, TAG_TABLE, "value \'%s\' does not exist", value);
        return;
    }
    uint64_t key = hash_djb2(value); // let's hope it's unsigned
    table_entry_ptr previous = table->inner[index];
    if (previous->key == key) {
        table->inner[index] = previous->next;
        previous->next = NULL;
        table_entry_free(previous);
        return;
    }
    table_entry_ptr next = previous->next;
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

ssize_t table_find(table_ptr table, const char * value) {
    if (table == NULL) {
        LOG(FATAL, TAG_TABLE, "tried to find in a NULL table");
        exit(EXIT_FAILURE);
    }
    if (value == NULL) {
        LOG(FATAL, TAG_TABLE, "tried to find a NULL value");
        exit(EXIT_FAILURE);
    }
    uint64_t key = hash_djb2(value); // let's hope it's unsigned
    size_t index = key % table->size;
    if (index > table->capacity - 1) {
        return -1;
    }
    table_entry_ptr next = table->inner[index];
    ssize_t sindex = -1;
    while (next != NULL) {
        if (next->key == key) {
            sindex = index;
            break;
        }
        next = next->next;
    }
    return sindex;
}

void table_list(table_ptr table, FILE * writer) {
    if (table == NULL) {
        LOG(FATAL, TAG_TABLE, "tried to list a NULL table");
        exit(EXIT_FAILURE);
    }
    if (writer == NULL) {
        LOG(FATAL, TAG_TABLE, "tried to list table contents to a NULL file");
    }
    for (size_t i = 0; i < table->capacity; i++) {
        if (table->inner[i] == NULL) {
            continue;
        }
        fprintf(writer, "table slot %zu\n", i);
        table_entry_ptr next = table->inner[i];
        while (next != NULL) {
            fprintf(writer, "- key: %llu, value: \'%s\'\n", next->key, next->value);
            next = next->next;
        }
    }
}
