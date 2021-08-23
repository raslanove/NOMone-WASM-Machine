
//////////////////////////////////////////////////////
// Created by Omar El Sayyed on 17th of July 2021.
//////////////////////////////////////////////////////

#pragma once

#include <NTypes.h>

struct NByteVector {
    // DON'T OVERWRITE. For use by the provided functions only.
    int32_t capacity;
    int32_t size;
    char* objects;
};

struct NByteVector_Interface {
    struct NByteVector* (*create)(int32_t initialCapacity, struct NByteVector* outputVector);
    struct NByteVector* (*createInHeap)(int32_t initialCapacity);
    void (*destroy)(struct NByteVector* vector);
    struct NByteVector* (*clear)(struct NByteVector* vector);
    boolean (*pushBack)(struct NByteVector* vector, char value);  // True if successful.
    boolean (*popBack)(struct NByteVector* vector, char *output);   // True if successful.
    boolean (*pushBack32Bit)(struct NByteVector* vector, int32_t value);
    boolean (*popBack32Bit)(struct NByteVector* vector, int32_t *output);
    char (*get)(struct NByteVector* vector, int32_t index);
    boolean (*set)(struct NByteVector* vector, int32_t index, char value);
    int32_t (*size)(struct NByteVector* vector);
};

extern const struct NByteVector_Interface NByteVector;
