#ifndef HASH_TABLE_OPEN_H
#define HASH_TABLE_OPEN_H

#include "value.h"
#include "object.h"

typedef struct
{
    Value value;
    ObjString* key;
} HashEntry;

typedef struct
{
    int count;
    int capacity;
    HashEntry* buckets;
} HashTable;

void htb_init(HashTable* table);
bool htb_add(HashTable* table, ObjString* key, Value value);
ObjString* htb_find_string(HashTable* table, const char* chars, int length,
                           uint32_t hash);
bool htb_delete(HashTable* table, ObjString* key);
void htb_copy_table(HashTable* from, HashTable* to);
Value* htb_get(HashTable* table, ObjString* key);
void htb_free(HashTable* table);

#endif  // HASH_TABLE_OPEN_H
