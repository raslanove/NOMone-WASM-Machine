#include <NVector.h>
#include <NSystemUtils.h>

#define NVECTOR_BOUNDARY_CHECK 1

static struct NVector* create(int32_t initialCapacity, int32_t objectSize, struct NVector* outputVector) {

    if (objectSize==0) return 0;

    NSystemUtils.memset(outputVector, 0, sizeof(struct NVector));

    outputVector->capacity = initialCapacity;
    outputVector->objectSize = objectSize;
    outputVector->objectsCount = 0;

    if (initialCapacity > 0) outputVector->objects = NSystemUtils.malloc(initialCapacity * objectSize);

    return outputVector;
}

static struct NVector* createInHeap(int32_t initialCapacity, int32_t objectSize) {

    if (objectSize==0) return 0;

    struct NVector* vector = NSystemUtils.malloc(sizeof(struct NVector));;
    return create(initialCapacity, objectSize, vector);
}

static void destroy(struct NVector* vector) {
    NSystemUtils.free(vector->objects);
    NSystemUtils.memset(vector, 0, sizeof(struct NVector));
}

static void destroyAndFree(struct NVector* vector) {
    destroy(vector);
    NSystemUtils.free(vector);
}

static boolean expand(struct NVector* vector) {
    if (vector->capacity == 0) {
        vector->objects = NSystemUtils.malloc(vector->objectSize);
        if (!vector->objects) return False;
        vector->capacity = 1;
    } else {
        int32_t originalSizeBytes = vector->capacity * vector->objectSize;
        void *newArray = NSystemUtils.malloc(originalSizeBytes << 1);
        if (!newArray) return False;
        NSystemUtils.memcpy(newArray, vector->objects, originalSizeBytes);
        NSystemUtils.free(vector->objects);
        vector->objects = newArray;
        vector->capacity <<= 1;
    }
    return True;
}

static void* emplaceBack(struct NVector* vector) {

    // Double the vector capacity if needed,
    if ((vector->objectsCount == vector->capacity) && !expand(vector)) return 0;

    // Make way for a new entry,
    void *newObjectPointer = (void *)(((intptr_t) vector->objects) + (vector->objectsCount * vector->objectSize));
    vector->objectsCount++;

    return newObjectPointer;
}

static boolean pushBack(struct NVector* vector, const void *object) {

    // Double the vector capacity if needed,
    if ((vector->objectsCount == vector->capacity) && !expand(vector)) return False;

    // Push the value,
    void *newObjectPointer = (void *)(((intptr_t) vector->objects) + (vector->objectsCount * vector->objectSize));
    NSystemUtils.memcpy(newObjectPointer, object, vector->objectSize);
    vector->objectsCount++;

    return True;
}

static boolean popBack(struct NVector* vector, void *outputObject) {
    if (vector->objectsCount==0) return False;

    vector->objectsCount--;
    void *lastObjectPointer = (void *)(((intptr_t) vector->objects) + (vector->objectsCount * vector->objectSize));
    NSystemUtils.memcpy(outputObject, lastObjectPointer, vector->objectSize);

    return True;
}

static void* get(struct NVector* vector, int32_t index) {
#if NVECTOR_BOUNDARY_CHECK
    if (index >= vector->objectsCount) return 0;
#endif

    return (void *)(((intptr_t) vector->objects) + (index * vector->objectSize));
}

static int32_t size(struct NVector* vector) {
    return vector->objectsCount;
}

const struct NVector_Interface NVector = {
    .create = create,
    .createInHeap = createInHeap,
    .destroy = destroy,
    .destroyAndFree = destroyAndFree,
    .emplaceBack = emplaceBack,
    .pushBack = pushBack,
    .popBack = popBack,
    .get = get,
    .size = size
};