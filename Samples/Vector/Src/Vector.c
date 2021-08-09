
#include <NSystemUtils.h>
#include <NVector.h>

void NMain(void) {

    struct NVector *vector = NVector.createInHeap(0, 4);

    int value=0;
    NVector.pushBack(vector, &value);
    NLOGI("Vector Test", "Pushed 0");
    value = 1;    
    NVector.pushBack(vector, &value);
    NLOGI("Vector Test", "Pushed 1");
    value = 2;    
    NVector.pushBack(vector, &value);
    NLOGI("Vector Test", "Pushed 2");
    value = 3;    
    NVector.pushBack(vector, &value);
    NLOGI("Vector Test", "Pushed 3");

    value = -1;
    NVector.popBack(vector, &value);
    NLOGI("Vector Test", "Popped. Expecting 3, got: %d", value);
    
    NLOGI("Vector Test", "Get(0): %d", *(int *) NVector.get(vector, 0));
    NLOGI("Vector Test", "Get(1): %d", *(int *) NVector.get(vector, 1));
    NLOGI("Vector Test", "Get(2): %d", *(int *) NVector.get(vector, 2));
 
    NLOGI("Vector Test", "Capacity. Expecting 4, got: %d", vector->capacity);
    NLOGI("Vector Test", "Size. Expecting 3, got: %d", vector->objectsCount);
 
    NVector.destroy(vector);
    NSystemUtils.free(vector);
}
