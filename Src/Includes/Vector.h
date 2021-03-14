
//////////////////////////////////////////////////////
// Created by Omar El Sayyed on 13th of March 2021.
//////////////////////////////////////////////////////

// Note: this is a generic (slow) implementation. A specialized implementation for 32bit integers
// is thus highly recommended. This can be achieved while maintaining high performance by creating
// a new set of functions and a different instance of the NVector_Interface to handle the 32bit
// specialized case. For example, NVector32.

#pragma once

#include <stdint.h>

#ifndef BOOL
#define BOOL
typedef enum { False=0, True=1 } boolean;
#endif

struct NVector_Interface {
    struct NVector* (*create)(int32_t initialCapacity, int32_t objectSize, struct NVector* outputVector);
    struct NVector* (*createInHeap)(int32_t initialCapacity, int32_t objectSize);
    void (*destroy)(struct NVector* vector);
    boolean (*pushBack)(struct NVector* vector, const void *object);
    boolean (*popBack)(struct NVector* vector, void *outputObject);
    void* (*get)(struct NVector* vector, int32_t index);
    int32_t (*size)(struct NVector* vector);
};

struct NVector {
    // DON'T OVERWRITE. For use by the provided functions only.
    int32_t capacity;
    int32_t objectSize;
    int32_t objectsCount;
    void* objects;
};

extern const struct NVector_Interface NVector;
