
#include <NByteVector.h>
#include <NSystemUtils.h>

void NMain(void) {

    struct NByteVector *vector = NByteVector.createInHeap(0);
    
    NByteVector.pushBack(vector, 0);
    NLOGI("ByteVector Test", "Pushed 0");
    NByteVector.pushBack(vector, 1);
    NLOGI("ByteVector Test", "Pushed 1");
    NByteVector.pushBack(vector, 2);
    NLOGI("ByteVector Test", "Pushed 2");
    NByteVector.pushBack(vector, 3);
    NLOGI("ByteVector Test", "Pushed 3");

    char value = -1;
    NByteVector.popBack(vector, &value);
    NLOGI("ByteVector Test", "Popped. Expecting 3, got: %d", (int) value);

    NByteVector.pushBack32Bit(vector, 12345);
    NLOGI("ByteVector Test", "Pushed 12345 (32bit)");
    NLOGI("ByteVector Test", "Size. Expecting 7, got: %d", vector->size);

    NLOGI("ByteVector Test", "Get(0). Expecting  0, got: %d", (int) NByteVector.get(vector, 0));
    NLOGI("ByteVector Test", "Get(1). Expecting  1, got: %d", (int) NByteVector.get(vector, 1));
    NLOGI("ByteVector Test", "Get(2). Expecting  2, got: %d", (int) NByteVector.get(vector, 2));
    NLOGI("ByteVector Test", "Get(3). Expecting 57, got: %d", (int) NByteVector.get(vector, 3));
    NLOGI("ByteVector Test", "Get(4). Expecting 48, got: %d", (int) NByteVector.get(vector, 4));
    NLOGI("ByteVector Test", "Get(5). Expecting  0, got: %d", (int) NByteVector.get(vector, 5));
    NLOGI("ByteVector Test", "Get(6). Expecting  0, got: %d", (int) NByteVector.get(vector, 6));

    int32_t value32Bit = -1;
    NByteVector.popBack32Bit(vector, &value32Bit);
    NLOGI("ByteVector Test", "Popped (32bit). Expecting 12345, got: %d", value32Bit);

    NLOGI("ByteVector Test", "Capacity. Expecting 8, got: %d", vector->capacity);
    NLOGI("ByteVector Test", "Size. Expecting 3, got: %d", vector->size);
 
    NByteVector.destroy(vector);
    NSystemUtils.free(vector);
}
