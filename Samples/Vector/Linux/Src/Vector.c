
#include <stdio.h>
#include <stdlib.h>

#include <Vector.h>

void main(void) {

    struct NVector *vector = NVector.createInHeap(0, 4);
    
    int value=0;
    NVector.pushBack(vector, &value);
    printf("Pushed 0\n");
    value = 1;    
    NVector.pushBack(vector, &value);
    printf("Pushed 1\n");
    value = 2;    
    NVector.pushBack(vector, &value);
    printf("Pushed 2\n");
    value = 3;    
    NVector.pushBack(vector, &value);
    printf("Pushed 3\n");

    value = -1;
    NVector.popBack(vector, &value);
    printf("Popped. Expecting 3, got: %d\n", value);
    
    printf("Get(0): %d\n", *(int *) NVector.get(vector, 0));
    printf("Get(1): %d\n", *(int *) NVector.get(vector, 1));
    printf("Get(2): %d\n", *(int *) NVector.get(vector, 2));
 
    printf("Capacity. Expecting 4, got: %d\n", vector->capacity);
    printf("Size. Expecting 3, got: %d\n", vector->objectsCount);
 
    NVector.destroy(vector);
    free(vector);
}
