#include <NByteVector.h>
#include <NSystemUtils.h>
#include <NError.h>

#define NBYTEVECTOR_BOUNDARY_CHECK 1

static struct NByteVector* create(int32_t initialCapacity,struct NByteVector* outputVector) {

    NSystemUtils.memset(outputVector, 0, sizeof(struct NByteVector));

    if ((initialCapacity>0) && (initialCapacity<4)) initialCapacity = 4; // Make sure that expands can accommodate a 32bit push.

    outputVector->capacity = initialCapacity;
    outputVector->size = 0;

    if (initialCapacity > 0) outputVector->objects = NSystemUtils.malloc(initialCapacity);

    return outputVector;
}

static struct NByteVector* createInHeap(int32_t initialCapacity) {
    struct NByteVector* vector = NSystemUtils.malloc(sizeof(struct NByteVector));;
    return create(initialCapacity, vector);
}

static void destroy(struct NByteVector* vector) {
    NSystemUtils.free(vector->objects);
    NSystemUtils.memset(vector, 0, sizeof(struct NByteVector));
}

static struct NByteVector* clear(struct NByteVector* vector) {
    vector->size = 0;
    return vector;
}

static boolean expand(struct NByteVector* vector) {
    if (vector->capacity == 0) {
        vector->objects = NSystemUtils.malloc(4);    // It's a waste to allocate less than 1 word, this also makes
                                        // sure we can push 32bits at a time.
        if (!vector->objects) return False;
        vector->capacity = 4;
    } else {
        void *newArray = NSystemUtils.malloc(vector->capacity << 1);
        if (!newArray) return False;
        NSystemUtils.memcpy(newArray, vector->objects, vector->capacity);
        NSystemUtils.free(vector->objects);
        vector->objects = newArray;
        vector->capacity <<= 1;
    }
    return True;
}

static boolean pushBack(struct NByteVector* vector, char value) {

    // Double the vector capacity if needed,
    if ((vector->size == vector->capacity) && !expand(vector)) return False;

    // Push the value,
    vector->objects[vector->size++] = value;
    return True;
}

static boolean popBack(struct NByteVector* vector, char *output) {
    if (vector->size == 0) return False;

    *output = vector->objects[--(vector->size)];
    return True;
}

static boolean pushBack32Bit(struct NByteVector* vector, int32_t value) {

    // Double the vector capacity if needed,
    if ((vector->size + 3 >= vector->capacity) && !expand(vector)) return False;

    // Push the value,
    *((int32_t *) &vector->objects[vector->size]) = value;
    vector->size += 4;
    return True;
}

static boolean popBack32Bit(struct NByteVector* vector, int32_t *output) {
    if (vector->size < 4) return False;

    vector->size -= 4;
    *output = *((int32_t *) &vector->objects[vector->size]);
    return True;
}

static char get(struct NByteVector* vector, int32_t index) {
#if NBYTEVECTOR_BOUNDARY_CHECK
    if (index >= vector->size) {
        NERROR("NByteVector.get()", "Index out of bound: %d", index);
        return 0;
    }
#endif

    return vector->objects[index];
}

static boolean set(struct NByteVector* vector, int32_t index, char value) {
#if NBYTEVECTOR_BOUNDARY_CHECK
    if (index >= vector->size) {
        NERROR("NByteVector.get()", "Index out of bound: %d", index);
        return False;
    }
#endif

    vector->objects[index] = value;
    return True;
}

static int32_t size(struct NByteVector* vector) {
    return vector->size;
}

const struct NByteVector_Interface NByteVector = {
    .create = create,
    .createInHeap = createInHeap,
    .destroy = destroy,
    .clear = clear,
    .pushBack = pushBack,
    .popBack = popBack,
    .pushBack32Bit = pushBack32Bit,
    .popBack32Bit = popBack32Bit,
    .get = get,
    .set = set,
    .size = size
};