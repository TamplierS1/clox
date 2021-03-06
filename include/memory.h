#ifndef MEMORY_H
#define MEMORY_H

#include "common.h"
#include "object.h"

/* === Memory management === */

#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity)*2)

#define GROW_ARRAY(type, pointer, old_count, new_count) \
    (type*)mem_reallocate(pointer, sizeof(type) * (old_count), sizeof(type) * (new_count))

#define FREE_ARRAY(type, pointer, old_count) \
    mem_reallocate(pointer, sizeof(type) * (old_count), 0)

#define ALLOCATE(type, length) (type*)mem_reallocate(NULL, 0, sizeof(type) * (length))

#define FREE(type, pointer) mem_reallocate(pointer, sizeof(type), 0)

void* mem_reallocate(void* pointer, size_t old_size, size_t new_size);
void free_objects();
void free_object(Obj* object);

/* === Memory management === */

#endif  // MEMORY_H
