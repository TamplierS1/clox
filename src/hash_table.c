#include "hash_table.h"
#include "memory.h"

#define TABLE_MAX_LOAD 0.75

static void grow_table(HashTable* table, int capacity);
static HashEntry* find_bucket(HashEntry* buckets, int capacity, ObjString* key);

void htb_init(HashTable* table)
{
    table->count = 0;
    table->capacity = 0;
    table->buckets = NULL;
}

bool htb_add(HashTable* table, ObjString* key, Value value)
{
    if (table->count + 1 > table->capacity * TABLE_MAX_LOAD)
    {
        int capacity = GROW_CAPACITY(table->capacity);
        grow_table(table, capacity);
    }

    HashEntry* bucket = find_bucket(table->buckets, table->capacity, key);
    bool is_new = bucket->key == NULL;
    if (is_new && IS_NIL(bucket->value))
        table->count++;

    bucket->key = key;
    bucket->value = value;

    return is_new;
}

ObjString* htb_find_string(HashTable* table, const char* chars, int length, uint32_t hash)
{
    if (table->count == 0)
        return NULL;

    uint32_t index = hash % table->capacity;
    while (true)
    {
        HashEntry* bucket = &table->buckets[index];
        if (bucket->key == NULL)
        {
            if (IS_NIL(bucket->value))
                return NULL;
        }
        else if (bucket->key->length == length && bucket->key->hash == hash &&
                 memcmp(bucket->key->chars, chars, length) == 0)
        {
            return bucket->key;
        }

        index = (index + 1) % table->capacity;
    }
}

bool htb_delete(HashTable* table, ObjString* key)
{
    if (table->count == 0)
        return false;

    HashEntry* bucket = find_bucket(table->buckets, table->capacity, key);
    if (bucket->key == NULL)
        return false;

    bucket->key = NULL;
    bucket->value = BOOL_VAL(true);

    return true;
}

void htb_copy_table(HashTable* from, HashTable* to)
{
    for (int i = 0; from->capacity; i++)
    {
        HashEntry* bucket = &from->buckets[i];
        if (bucket->key != NULL)
        {
            htb_add(to, bucket->key, bucket->value);
        }
    }
}

Value* htb_get(HashTable* table, ObjString* key)
{
    if (table->count == 0)
        return NULL;

    HashEntry* bucket = find_bucket(table->buckets, table->capacity, key);
    if (bucket->key == NULL)
    {
        return NULL;
    }

    return &bucket->value;
}

void htb_free(HashTable* table)
{
    FREE_ARRAY(HashEntry, table->buckets, table->count);
    htb_init(table);
}

static void grow_table(HashTable* table, int capacity)
{
    // Allocate empty buckets array.
    HashEntry* new_buckets = ALLOCATE(HashEntry, capacity);
    for (int i = 0; i < capacity; i++)
    {
        new_buckets[i].key = NULL;
        new_buckets[i].value = NIL_VAL;
    }

    if (table->count != 0)
    {
        table->count = 0;
        // Copy the existing buckets to the new buckets array.
        for (int i = 0; i < capacity; i++)
        {
            HashEntry* bucket = &table->buckets[i];
            if (bucket->key == NULL)
                continue;

            HashEntry* dest = find_bucket(new_buckets, capacity, bucket->key);
            dest->key = bucket->key;
            dest->value = bucket->value;

            table->count++;
        }
    }

    FREE_ARRAY(HashEntry, table->buckets, table->capacity);

    table->buckets = new_buckets;
    table->capacity = capacity;
}

static HashEntry* find_bucket(HashEntry* buckets, int capacity, ObjString* key)
{
    uint32_t index = key->hash % capacity;
    // Represents buckets that were deleted.
    HashEntry* tombstone = NULL;
    // It'll never go into an infinite loop here, because
    // the table always has empty buckets (we grow it
    // when it becomes 75% full).
    while (true)
    {
        HashEntry* bucket = &buckets[index];
        if (bucket->key == NULL)
        {
            if (IS_NIL(bucket->value))
            {
                // Empty bucket
                return tombstone != NULL ? tombstone : bucket;
            }
            else
            {
                // Found a tombstone
                if (tombstone == NULL)
                    tombstone = bucket;
            }
        }
        else if (bucket->key == key)
        {
            return bucket;
        }

        // The second modulo here wraps the index to the
        // beginning of the array if it exceeds array's capacity.
        index = (index + 1) % capacity;
    }
}
