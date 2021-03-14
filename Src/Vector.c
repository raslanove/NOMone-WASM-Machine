#include <stdlib.h>
#include <string.h>

#include <Vector.h>

#define NVECTOR_BOUNDARY_CHECK 1

static struct NVector* create(int32_t initialCapacity, int32_t objectSize) {

    if (objectSize==0) return 0;

    struct NVector* vector = malloc(sizeof(struct NVector));;
    memset(vector, 0, sizeof(struct NVector));

    vector->capacity = initialCapacity;
    vector->objectSize = objectSize;
    vector->objectsCount = 0;

    if (initialCapacity > 0) vector->objects = malloc(initialCapacity * objectSize);

    return vector;
}

static void freeVector(struct NVector* vector) {
    free(vector->objects);
    memset(vector, 0, sizeof(struct NVector));
    free(vector);
}

static bool pushBack(struct NVector* vector, const void *object) {

    // Double the vector capacity if needed,
    if (vector->objectsCount == vector->capacity) {
        if (vector->capacity == 0) {
            vector->objects = malloc(vector->objectSize);
            if (!vector->objects) return false;
            vector->capacity = 1;
        } else {
            int32_t originalSizeBytes = vector->capacity * vector->objectSize;
            void *newArray = malloc(originalSizeBytes << 1);
            if (!newArray) return false;
            memcpy(newArray, vector->objects, originalSizeBytes);
            free(vector->objects);
            vector->objects = newArray;
            vector->capacity <<= 1;
        }
    }

    // Push the value,
    void *newObjectPointer = (void *)(((intptr_t) vector->objects) + (vector->objectsCount * vector->objectSize));
    memcpy(newObjectPointer, object, vector->objectSize);
    vector->objectsCount++;

    return true;
}

static bool popBack(struct NVector* vector, void *outputObject) {
    if (vector->objectsCount==0) return false;

    vector->objectsCount--;
    void *lastObjectPointer = (void *)(((intptr_t) vector->objects) + (vector->objectsCount * vector->objectSize));
    memcpy(outputObject, lastObjectPointer, vector->objectSize);

    return true;
}

static void* get(struct NVector* vector, int32_t index) {
#if NVECTOR_BOUNDARY_CHECK
    if (index >= vector->objectsCount) return 0;
#endif

    return (void *)(((intptr_t) vector->objects) + (index * vector->objectSize));
}

const struct NVector_Interface NVector = {
    .create = create,
    .free = freeVector,
    .pushBack = pushBack,
    .popBack = popBack,
    .get = get
};