
#include <stdio.h>
#include <stdlib.h>

#include <ByteVector.h>

void main(void) {

    struct NByteVector *vector = NByteVector.createInHeap(0);
    
    NByteVector.pushBack(vector, 0);
    printf("Pushed 0\n");
    NByteVector.pushBack(vector, 1);
    printf("Pushed 1\n");
    NByteVector.pushBack(vector, 2);
    printf("Pushed 2\n");
    NByteVector.pushBack(vector, 3);
    printf("Pushed 3\n");

    char value = -1;
    NByteVector.popBack(vector, &value);
    printf("Popped. Expecting 3, got: %d\n", (int) value);

    NByteVector.pushBack32Bit(vector, 12345);
    printf("Pushed 12345 (32bit)\n");
    printf("Size. Expecting 7, got: %d\n", vector->size);

    printf("Get(0). Expecting  0, got: %d\n", (int) NByteVector.get(vector, 0));
    printf("Get(1). Expecting  1, got: %d\n", (int) NByteVector.get(vector, 1));
    printf("Get(2). Expecting  2, got: %d\n", (int) NByteVector.get(vector, 2));
    printf("Get(3). Expecting 57, got: %d\n", (int) NByteVector.get(vector, 3));
    printf("Get(4). Expecting 48, got: %d\n", (int) NByteVector.get(vector, 4));
    printf("Get(5). Expecting  0, got: %d\n", (int) NByteVector.get(vector, 5));
    printf("Get(6). Expecting  0, got: %d\n", (int) NByteVector.get(vector, 6));

    int32_t value32Bit = -1;
    NByteVector.popBack32Bit(vector, &value32Bit);
    printf("Popped (32bit). Expecting 12345, got: %d\n", value32Bit);

    printf("Capacity. Expecting 8, got: %d\n", vector->capacity);
    printf("Size. Expecting 3, got: %d\n", vector->size);
 
    NByteVector.destroy(vector);
    free(vector);
}
