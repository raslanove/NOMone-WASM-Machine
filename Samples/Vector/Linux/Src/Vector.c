
#include <stdio.h>

#include <Vector.h>

void main(void) {

    struct NVector *vector = NVector.create(0, 4);
    
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
    printf("Poped. Expecting 3, got: %d\n", value);
    
    printf("Get(0): %d\n", *(int *) NVector.get(vector, 0));
    printf("Get(1): %d\n", *(int *) NVector.get(vector, 1));
    printf("Get(2): %d\n", *(int *) NVector.get(vector, 2));
 
    printf("Capacity: %d\n", vector->capacity);
    printf("Objects count: %d\n", vector->objectsCount);
 
    NVector.free(vector);
}
